#include "writer.h"
#include "reader.h"

#include <fstream>
#include <iostream>

const int ERROR = 111;

int main(int argc, char** argv) {
    std::vector<std::pair<int, int>> qu;
    if (1) {
        std::string dir = argv[1];
        dir += '/';
        std::ifstream in(dir + "test.txt");
        int n;
        in >> n;
        std::ofstream out("tmp_out.txt");
        Writer write(out);
        qu.resize(n);
        for (int i = 0; i < n; i++) {
            int x, b;
            in >> x >> b;
            qu[i] = {b, x};
        }
        for (auto u : qu) {
            write.WriteBits(u.first, u.second);
        }
    }
    std::ifstream inn("tmp_out.txt");
    Reader read(inn);
    for (auto u : qu) {
        int x = read.Readbits(u.first);
        if (x != u.second) {
            std::cout << "TEST FAILED" << std::endl;
            exit(ERROR);
        }
    }
}