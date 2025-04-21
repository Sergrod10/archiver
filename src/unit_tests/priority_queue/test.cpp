#include "priority_queue.h"

#include <fstream>
#include <iostream>

const int ERROR = 111;

struct Compare {
    bool operator()(int a, int b) {
        return a > b;
    }
};

int main(int argc, char** argv) {
    std::string dir = argv[1];
    dir += '/';
    std::ifstream in(dir + "test.txt");
    PriorityQueue<int, Compare> pq;
    int n;
    in >> n;
    std::vector<int> anses;
    while (n--) {
        int type;
        in >> type;
        if (type == 1) {
            int x;
            in >> x;
            pq.Push(x);
        } else if (type == 2) {
            pq.Pop();
        } else if (type == 3) {
            anses.push_back(pq.Top());
        } else {
            anses.push_back(pq.Size());
        }
    }
    std::ifstream check(dir + "expected.txt");
    int id = 0;
    while (!check.eof()) {
        int x;
        check >> x;
        if (id >= anses.size() || anses[id] != x) {
            std::cout << "TEST FAILED" << std::endl;
            exit(ERROR);
        }
        id++;
    }
    if (id != anses.size()) {
        std::cout << "TEST FAILED" << std::endl;
        exit(ERROR);
    }
}