#include "archive.h"
#include "CmdParser.h"

#include <iostream>

const int ERROR = 111;

int main(int argc, char** argv) {
    CmdParser pars_console(argc, argv);
    Archive arc;
    if (pars_console.IsExistsArg("c")) {
        try {
            auto cur_in = pars_console.GetListByArg("c");
            std::string to = cur_in[0];
            cur_in.erase(cur_in.begin(), cur_in.begin() + 1);
            arc.Compress(to, cur_in);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            exit(ERROR);
        }
    }
    if (pars_console.IsExistsArg("d")) {
        try {
            arc.Decompress(pars_console.GetListByArg("d")[0]);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            exit(ERROR);
        }
    }
    if (pars_console.IsExistsArg("h")) {
        try {
            arc.Help();
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            exit(ERROR);
        }
    }
}