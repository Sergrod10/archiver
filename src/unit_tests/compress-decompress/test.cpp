#include "archive.h"
#include "CmdParser.h"

#include <iostream>
#include <fstream>

const int ERROR = 111;

struct SaveFile {
    std::string name;
    std::vector<unsigned char> inside;

    explicit SaveFile(std::string file) {
        std::ifstream in(file, std::ios::binary);
        name = file;
        while (!in.eof()) {
            unsigned char x = in.get();
            inside.push_back(x);
        }
    }

    bool IsEqual(std::string file_for_compare) {
        std::ifstream in(file_for_compare, std::ios::binary);
        if (name != file_for_compare) {
            return false;
        }
        int cur = 0;
        while (!in.eof()) {
            unsigned char x = in.get();
            if (cur >= inside.size() || inside[cur] != x) {
                std::cout << inside.size() << " " << cur << " " << (int)inside[cur] << " " << (int)x << std::endl;
                return false;
            }
            cur++;
        }
        return cur >= inside.size();
    }

    ~SaveFile() {
        std::cout << name << std::endl;
        std::ofstream return_origin_state(name, std::ios::binary);
        for (int i = 0; i < inside.size() - 1; i++) {
            return_origin_state << inside[i];
        }
    }
};

int main(int argc, char** argv) {
    std::string dir = argv[1];
    std::vector<SaveFile> sv;
    sv.reserve(1000);
    std::vector<std::string> files;
    for (const auto& u : std::filesystem::directory_iterator(dir)) {
        files.push_back(u.path());
        sv.push_back(SaveFile(u.path()));
    }
    Archive arc;
    arc.Compress("c", files);
    arc.Decompress("c");
    int cnt = 0;
    for (const auto& u : std::filesystem::directory_iterator(dir)) {
        std::string path = u.path();
        cnt++;
        int ok = 0;
        for (auto& y : sv) {
            if (y.name == path) {
                ok = 1;
                if (!y.IsEqual(path)) {
                    std::cout << path << std::endl;
                    std::cout << "TEST FAILED" << std::endl;
                    exit(ERROR);
                }
            }
        }
        if (!ok) {
            std::cout << "TEST FAILED" << std::endl;
            exit(ERROR);
        }
    }
    if (cnt != sv.size()) {
        std::cout << "TEST FAILED" << std::endl;
        exit(ERROR);
    }
}