#include <iostream>
#include <bitset>
#include <vector>
#include "main.h"
#include "engine.h"

char flags = 0;

int main(int argc, char *argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.begin() == args.end() || *args.begin() == "help") {
        showHelp();
    } else if (*args.begin() == "run") {
        std::string *file;
        for (auto i = args.begin(); i != args.end(); i++) {
            if (*i == "-i" || *i == "--stdin") {
                flags |= FLAG_STDIN;
            } else if (*i == "-f" || *i == "--file") {
                if (++i == args.end()) {
                    std::cout << "Error: No file provided." << std::endl;
                    return 1;
                }
                file = &*i;
            } else if (*i == "-v" || *i == "--verbose") {
                flags |= FLAG_VERBOSE;
            }
        }

        Engine engine;
        if (flags & FLAG_STDIN) {
            engine.loadStdin();
        } else {
            engine.load(file);
        }
        engine.run();
    }

    return 0;
}

void showHelp() {
    std::cout << "LMINCO v" << VERSION << std::endl <<
              "lminco <run>  = Specify operation mode." << std::endl <<
              "-h (--help)  = Display this information." << std::endl << std::endl <<
              "run  = Execute compiled code." << std::endl <<
              "  -i (--stdin)  = Take input from stdin." << std::endl <<
              "  -f (--file) <path>  = Take input from specified file." << std::endl <<
              "  -v (--verbose)  = Output increased detail and insights." << std::endl;
}
