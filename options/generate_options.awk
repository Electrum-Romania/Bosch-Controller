#!/bin/awk -f

BEGIN {
    FS=" *=? *|;"

}

NF == 4 {
    type = $1
    variable_name = $2
    default_value = $3

    switch (target) {
        case "declaration":
            print type, variable_name ";"
            break
        case "default":
            print "def->" variable_name " = " default_value ";"
            break
        case "parse":
            print "if (command == \"" variable_name "\") {"
            switch (type) {
                case "int8_t":  case "int16_t":  case "int32_t":  case "int64_t":
                case "uint8_t": case "uint16_t": case "uint32_t": case "uint64_t":
                case "int":     case "long":     case "unsigned": case "short":

                    print "try {"
                    print "current->" variable_name " = std::stoi(arg);"
                    print "} catch (const std::invalid_argument& e) {"
                    print "std::cerr << \"Invalid argument when parsing " variable_name ": \" << e.what() << std::endl;"
                    print "} catch (const std::out_of_range& e) {"
                    print "std::cerr << \"Out of range when parsing " variable_name ": \" << e.what() << std::endl;"
                    print "}"

                    break

                case "std::string":

                    print "current->" variable_name " = std::move(arg);"

                    break

                default:

                    print "Unsupported type " type > "/dev/stderr"
            }
            print "}"
            break
        case "register":
            print "cs.register_command(\"" variable_name "\", this, parse_new_option_wrapper);"
    }
}

NF == 0 {}
