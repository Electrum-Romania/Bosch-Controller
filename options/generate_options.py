#!/usr/bin/env python3

import os
import subprocess
import sys


def find_files(directory, requested):
    for root, _, files in os.walk(directory):
        for file in files:
            if file == requested:
                yield os.path.join(root, file)


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

if __name__ == "__main__":
    pdata_files = find_files(".", "pdata.inc")
    concatenate_files(pdata_files, "options/pdata.gen.inc")

    options_files = find_files(".", "options.inc")
    with open("options/options_parse.gen.inc", "w") as outfile:
        run_script_on_files(options_files, outfile, ["options/generate_options.awk", "target=parse"])
    options_files = find_files(".", "options.inc")
    with open("options/options.gen.inc", "w") as outfile:
        print("struct Options {", file=outfile)
        outfile.flush()
        run_script_on_files(options_files, outfile, ["options/generate_options.awk", "target=declaration"])
        print("};", file=outfile)
    options_files = find_files(".", "options.inc")
    with open("options/options_defaults.gen.inc", "w") as outfile:
        run_script_on_files(options_files, outfile, ["options/generate_options.awk", "target=default"])

    options_files = find_files(".", "options.inc")
    with open("options/options_register.gen.inc", "w") as outfile:
        run_script_on_files(options_files, outfile, ["options/generate_options.awk", "target=register"])
