#include "trie.h"

#include <vector>

Node::Node() {
    l = nullptr;
    r = nullptr;
    terminate = -1;
}

Node::Node(int x) : Node() {
    terminate = x;
}

Trie::Trie() {
    std::unique_ptr<Node> tmp(new Node());
    head_ = std::move(tmp);
}

Trie::Trie(int x) {
    std::unique_ptr<Node> tmp(new Node(x));
    head_ = std::move(tmp);
}

void Trie::Merge(Trie* t1, Trie* t2) {
    head_->l = std::move(t1->head_);
    head_->r = std::move(t2->head_);
}

void Trie::IterateOverallTrie(const std::unique_ptr<Node>& root, std::string cur_path,
                              std::vector<std::pair<int, std::string>>& ans) const {
    if (root->terminate != -1) {
        ans.push_back({root->terminate, cur_path});
    }
    if (root->l != nullptr) {
        IterateOverallTrie(root->l, cur_path + '0', ans);
    }
    if (root->r != nullptr) {
        IterateOverallTrie(root->r, cur_path + '1', ans);
    }
}

std::vector<std::pair<int, std::string>> Trie::GetAllSymbols() const {
    std::vector<std::pair<int, std::string>> ans;
    IterateOverallTrie(head_, "", ans);
    return ans;
}

void Trie::Add(const std::unique_ptr<Node>& root, int num_bit, std::pair<int, std::string> symbol) {
    if (root->terminate != -1) {
        throw std::logic_error("trie: several symbols are assigned to one node");
    }
    if (num_bit >= symbol.second.size()) {
        root->terminate = symbol.first;
        return;
    }
    if (symbol.second[num_bit] == '0') {
        if (root->l == nullptr) {
            std::unique_ptr<Node> tmp(new Node());
            root->l = std::move(tmp);
        }
        Add(root->l, num_bit + 1, symbol);
        return;
    }
    if (root->r == nullptr) {
        std::unique_ptr<Node> tmp(new Node());
        root->r = std::move(tmp);
    }
    Add(root->r, num_bit + 1, symbol);
}

Trie::Trie(const std::vector<std::pair<int, std::string>>& symbols) : Trie() {
    for (auto& u : symbols) {
        Add(head_, 0, u);
    }
}

const std::unique_ptr<Node>& Trie::ReadBitByBit() const {
    return head_;
}

const std::unique_ptr<Node>& Trie::ReadBitByBit(const std::unique_ptr<Node>& from, int type_bit) const {
    if (type_bit == 0) {
        if (from->l == nullptr) {
            throw std::runtime_error("trie: moving along a non-existent edge in ReadBitByBit(l)");
        }
        return from->l;
    }
    if (from->r == nullptr) {
        throw std::runtime_error("trie: moving along a non-existent edge in ReadBitByBit(r)");
    }
    return from->r;
}