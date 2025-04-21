#pragma once

#include <vector>
#include "reader.h"
#include "writer.h"
#include "trie.h"
#include <string>
#include <unordered_map>

struct ComparatorForPQ {
    bool operator()(std::pair<std::pair<int, int>, Trie*>& lhs, std::pair<std::pair<int, int>, Trie*>& rhs) {
        return lhs.first.first > rhs.first.first ||
               (lhs.first.first == rhs.first.first && lhs.first.second > rhs.first.second);
    }
};

class Archive {

public:
    void Compress(const std::string& where_compress, const std::vector<std::string>& to_compress);

    void Decompress(const std::string& what_decompress);

    void Help();

private:
    void CompressFile(std::string name, int is_last, Writer& out);

    int DecompressFile(Reader& in);

    void AddOneToString(std::string& s) const;

    std::vector<std::pair<int, std::string>> Haffman(const std::unordered_map<int, int>& freq) const;

    std::vector<std::pair<int, std::string>> MakeCanonical(std::vector<std::pair<int, std::string>> codes) const;

    std::vector<std::pair<int, std::string>> EncodeCanonical(const std::vector<int>& symbols,
                                                             const std::vector<int>& lens) const;

    std::ifstream CreateInStreamWithFile(std::string file);

    std::ofstream CreateOutStreamWithFile(std::string file, int is_app);
};