#include <fstream>
#include <iostream>
#include "CmdParser.h"

const int ERROR = 111;

int main(int argc, char** argv) {
    std::string dir = argv[1];
    dir += '/';
    std::ifstream in(dir + "test.txt");
    std::string console;
    std::vector<std::string> v;
    while (in >> console) {
        v.push_back(console);
    }
    std::vector<char*> cstrings;
    cstrings.reserve(v.size());
    for (auto& s : v)
        cstrings.push_back(&s[0]);
    CmdParser pars(cstrings.size(), cstrings.data());
    auto arg = pars.GetAllArgs();
    std::ifstream compare(dir + "expected.txt");
    for (int i = 0; i < arg.size(); i++) {
        auto x = pars.GetListByArg(arg[i]);
        std::string cmp1;
        compare >> cmp1;
        if (cmp1 != arg[i]) {
            std::cout << arg[i] << std::endl;
            std::cout << "TEST FAILED" << std::endl;
            exit(ERROR);
        }
        for (auto u : x) {
            std::string cmp;
            compare >> cmp;
            if (cmp != u) {
                std::cout << "TEST FAILED" << std::endl;
                exit(ERROR);
            }
        }
        std::string end;
        compare >> end;
        if ((i != arg.size() - 1 && end != ".") || (i == arg.size() - 1 && end != "---")) {
            std::cout << "TEST FAILED" << std::endl;
            exit(ERROR);
        }
    }
    auto free_strings = pars.GetAllString();
    for (auto& u : free_strings) {
        std::string comp;
        compare >> comp;
        if (comp != u) {
            std::cout << "TEST FAILED" << std::endl;
            exit(ERROR);
        }
    }
    unsigned char x = compare.get();
    if (!compare.eof() && x != 255) {
        std::cout << "TEST FAILED" << std::endl;
        exit(ERROR);
    }
}