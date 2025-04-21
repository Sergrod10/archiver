#pragma once

#include <vector>
#include <string>
#include <memory>

struct Node {
    Node();

    explicit Node(int x);

    std::unique_ptr<Node> l;
    std::unique_ptr<Node> r;
    int terminate;
};

class Trie {

public:
    Trie();

    explicit Trie(int x);

    explicit Trie(const std::vector<std::pair<int, std::string>>& symbols);

    void Merge(Trie* t1, Trie* t2);

    const std::unique_ptr<Node>& ReadBitByBit(const std::unique_ptr<Node>& from, int type_bit) const;

    const std::unique_ptr<Node>& ReadBitByBit() const;

    std::vector<std::pair<int, std::string>> GetAllSymbols() const;

private:
    std::unique_ptr<Node> head_;

    void IterateOverallTrie(const std::unique_ptr<Node>& root, std::string cur_path,
                            std::vector<std::pair<int, std::string>>& ans) const;

    void Add(const std::unique_ptr<Node>& root, int num_bit, std::pair<int, std::string> symbol);
};
