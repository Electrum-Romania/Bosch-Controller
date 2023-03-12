#!/usr/bin/env python3

import argparse
import re
import subprocess
import sys
import os

from CppHeaderParser import CppHeaderParser
from sctokenizer import Source, TokenType
from typing import Iterable, Generator, Dict, List


def file_name_to_class_name(file_name: str) -> str:
    parts = os.path.basename(file_name).split('.')
    name_part = parts[0]

    words = name_part.split('_')

    capitalized_words = [word.capitalize() for word in words]
    class_name = ''.join(capitalized_words)

    if class_name == 'Ptasks':
        return 'Ptask'

    return class_name


def find_files(directory: str, requested: str, use_re: bool = False) -> Generator[str, None, None]:
    if not use_re:
        requested = re.escape(requested)

    requested_re = re.compile(requested)

    for root, _, files in os.walk(directory):
        for file in files:
            if requested_re.fullmatch(file):
                yield os.path.join(root, file)


def create_used_maps() -> (Dict[str, List[str]], Dict[str, List[str]]):
    option_used_by = {}
    pdata_used_by = {}

    for file in find_files('.', r'.*\.cpp', use_re=True):
        src = Source.from_file(file, lang='cpp')
        tokens = src.tokenize()

        for i in range(0, len(tokens) - 2):
            token = tokens[i]
            next_token = tokens[i + 1]
            next_next_token = tokens[i + 2]

            if token.token_value == 'pdata' and token.token_type == TokenType.IDENTIFIER and \
                    next_token.token_value == '->' and next_token.token_type == TokenType.OPERATOR and \
                    next_next_token.token_type == TokenType.IDENTIFIER:

                option_name = next_next_token.token_value
                class_name = file_name_to_class_name(file)

                if option_name not in pdata_used_by:
                    pdata_used_by[option_name] = [class_name]
                elif class_name not in pdata_used_by[option_name]:
                    pdata_used_by[option_name] += [class_name]

            if token.token_value == 'options_manager' and token.token_type == TokenType.IDENTIFIER and \
                    next_token.token_value == '->' and next_token.token_type == TokenType.OPERATOR and \
                    next_next_token.token_type == TokenType.IDENTIFIER:

                option_name = next_next_token.token_value
                class_name = file_name_to_class_name(file)

                if option_name not in option_used_by:
                    option_used_by[option_name] = [class_name]
                elif class_name not in option_used_by[option_name]:
                    option_used_by[option_name] += [class_name]

    return pdata_used_by, option_used_by


class Options:
    _static_options_declaration_file_header = '''///
/// \\file static_options.gen.h
/// \\brief StaticOptions struct

///
/// \\brief Compilation constants
struct StaticOptions {
'''

    _static_options_declaration_file_footer = '};'

    _options_declaration_file_header = '''///
/// \\file options_manager.gen.h
/// \\brief Options struct

///
/// \\brief Hot swappable-options_manager
struct Options {
'''

    _options_declaration_file_footer = '};'

    def __init__(self, input_files: Iterable[str], static: bool = False, pdata: bool = False,
                 used_by: Dict[str, List[str]] = None, command_line_options: Dict[str, str] = None):
        self._options = []
        self._static = static
        self._pdata = pdata

        # TODO: pass *.inc headers

        for file in input_files:
            parsed_file = CppHeaderParser.CppHeader(file)

            # TODO: Check that the file doesn't contain anything we don't want

            for variable in parsed_file.variables:
                variable_name = variable['name']

                # TODO: Check that the variable doesn't contain anything we don't want

                if not self._pdata:
                    if self._static and variable_name in command_line_options:
                        variable['value'] = command_line_options[variable_name]
                    elif 'default' in variable:
                        variable['value'] = variable['default']
                    else:
                        raise Exception(f'option {variable_name} in {file} has no default value and no explicit value '
                                        'was given')

                variable['creator'] = file_name_to_class_name(os.path.dirname(file))

                if not self._static:
                    try:
                        used_by_list = used_by[variable_name]
                        try:
                            used_by_list.remove(variable['creator'])
                        except ValueError:
                            print(f'Warn: option {variable_name} not used by creating class ' + variable['creator'],
                                  file=sys.stderr)
                    except KeyError:
                        print(f'Warn: option {variable_name} in file {file} unused', file=sys.stderr)
                        used_by_list = []
                else:
                    used_by_list = []

                variable['also-used-by'] = used_by_list

                self._options += [variable]

    def print_declaration_file(self, header_file_name: str):
        if self._static:
            declaration_file_header = Options._static_options_declaration_file_header
            declaration_file_footer = Options._static_options_declaration_file_footer
        else:
            declaration_file_header = Options._options_declaration_file_header
            declaration_file_footer = Options._options_declaration_file_footer

        with open(header_file_name, "w") as output_file:
            print(declaration_file_header, file=output_file)

            for option in self._options:
                if 'doxygen' in option:
                    print(option['doxygen'], file=output_file)

                if 'default' in option:
                    print('/// \\note Default value is `' + option['default'] + '`', file=output_file)

                first_used_by = True

                if self._pdata:
                    print('/// \\note Generated by: ' + option['creator'], file=output_file, end='')
                else:
                    print('/// \\note Used by: ' + option['creator'], file=output_file, end='')
                    first_used_by = False

                if self._pdata and len(option['also-used-by']) > 0:
                    print('.\n/// \\note Used by: ', file=output_file, end='')

                for i in option['also-used-by']:
                    if not first_used_by:
                        print(', ', file=output_file, end='')
                    else:
                        first_used_by = False

                    print(f'{i}', file=output_file, end='')

                print('.', file=output_file)

                if self._static:
                    print('static', file=output_file, end=' ')
                    print('constexpr', file=output_file, end=' ')

                if self._static and option['type'] == 'std::string':
                    print('char ' + option['name'] + '[]', file=output_file, end=' ')
                else:
                    print(option['type'], file=output_file, end=' ')
                    print(option['name'], file=output_file, end='')

                if self._static:
                    print(' =', file=output_file, end=' ')
                    print(option['value'], file=output_file, end='')

                print(';', file=output_file)

            print(declaration_file_footer, file=output_file)

    def print_set_default(self, output_file_name: str):
        with open(output_file_name, "w") as output_file:
            for option in self._options:
                option_name = option['name']
                default_value = option['default']

                print(f'def->{option_name} = {default_value};', file=output_file)

    def print_register(self, output_file_name: str):
        with open(output_file_name, "w") as output_file:
            for option in self._options:
                option_name = option['name']

                print(f'cs.register_command("{option_name}", this, parse_new_option_wrapper);', file=output_file)

    _integer_parser = '''
    try {{
        current->{0} = std::stoi(arg);
    }} catch (const std::invalid_argument& e) {{
        std::cerr << "Invalid argument when parsing {0}: " << e.what() << std::endl;
    }} catch (const std::out_of_range& e) {{
        std::cerr << \"Out of range when parsing {0}: " << e.what() << std::endl;
    }}'''

    _integer_types = ['std::int8_t', 'std::uint8_t', 'std::int16_t', 'std::uint16_t',
                      'std::int32_t', 'std::uint32_t', 'std::int64_t', 'std::uint64_t',
                      'short', 'unsigned short', 'int', 'unsigned int', 'unsigned',
                      'long', 'unsigned long', 'long long', 'unsigned long long']

    _double_parser = '''
    try {{
        current->{0} = std::stod(arg);
    }} catch (const std::invalid_argument& e) {{
        std::cerr << "Invalid argument when parsing {0}: " << e.what() << std::endl;
    }} catch (const std::out_of_range& e) {{
        std::cerr << \"Out of range when parsing {0}: " << e.what() << std::endl;
    }}'''

    _double_types = ['float', 'double', 'long double']

    _string_parser = '''current->{0} = std::move(arg);'''

    _string_types = ['std::string']

    def print_parser(self, output_file_name: str):
        with open(output_file_name, "w") as output_file:
            for option in self._options:
                option_name = option['name']
                option_type = option['type']

                print(f'if (command == "{option_name}") ' + '{', file=output_file)

                if option_type in Options._integer_types:
                    print(Options._integer_parser.format(option_name), file=output_file)
                elif option_type in Options._double_types:
                    print(Options._double_parser.format(option_name), file=output_file)
                elif option_type in Options._string_types:
                    print(Options._string_parser.format(option_name), file=output_file)
                else:
                    raise Exception(f'Unsupported type {option_type} of value {option_name}')

                print('}', file=output_file)

    def print_using_options(self):
        print('Configured using static options_manager:\n')
        for option in self._options:
            print('    ', end='')
            print(option['name'], end=' ')
            print('=', end=' ')
            print(option['value'], end=';\n')


def concatenate_files(file_list, output_file):
    with open(output_file, "w") as outfile:
        print("struct Pdata {", file=outfile)
        for file_path in file_list:
            with open(file_path) as infile:
                outfile.write(infile.read())
        print("};", file=outfile)


def run_script_on_files(file_list, outfile, script):
    for file_path in file_list:
        with open(file_path) as infile:
            res = subprocess.run(script, stdin=infile, stdout=outfile, stderr=sys.stderr)
            if res.returncode != 0:
                print("script failed with error code ", res.returncode, file=sys.stderr)
                sys.exit(1)


def main():
    try:
        os.mkdir('generated', 0o755)
    except FileExistsError:
        pass

    (pdata_used_by, option_used_by) = create_used_maps()

    options = Options(find_files('.', 'options_manager.inc'), used_by=option_used_by)

    options.print_declaration_file('generated/options_manager.gen.h')
    options.print_set_default('generated/options_defaults.gen.inc')
    options.print_register('generated/options_register.gen.inc')
    options.print_parser('generated/options_parse.gen.inc')

    pdata = Options(find_files('.', 'pdata.inc'), pdata=True, used_by=pdata_used_by)

    pdata.print_declaration_file('generated/pdata.gen.h')

    argument_parser = argparse.ArgumentParser()
    argument_parser.add_argument('-D', action='append')
    arguments = argument_parser.parse_args()

    command_line_options = {}

    if arguments.D is not None:
        for definition in arguments.D:
            x = definition.split('=', 2)

            command_line_options[x[0]] = x[1]

    static_options = Options(find_files('.', 'static_options.inc'), static=True,
                             command_line_options=command_line_options)

    static_options.print_declaration_file('generated/static_options.gen.h')
    static_options.print_using_options()


if __name__ == "__main__":
    main()
