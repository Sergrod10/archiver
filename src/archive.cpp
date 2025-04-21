#include "archive.h"
#include "reader.h"
#include "writer.h"
#include "trie.h"
#include "priority_queue.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <stdexcept>

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;
const int BYTE = 8;
const int SZ = 9;
const int MAX_SYMBOL = 258;

void Archive::AddOneToString(std::string& s) const {
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; i--) {
        if (s[i] == '0') {
            s[i] = '1';
            for (int j = i + 1; j < s.size(); j++) {
                s[j] = '0';
            }
            return;
        }
    }
    throw std::logic_error("made +1 to 111...111");
}

std::vector<std::pair<int, std::string>> Archive::Haffman(const std::unordered_map<int, int>& freq) const {
    PriorityQueue<std::pair<std::pair<int, int>, Trie*>, ComparatorForPQ> pq;
    for (auto u : freq) {
        Trie* now = new Trie(u.first);
        pq.Push({{u.second, u.first}, now});
    }
    while (pq.Size() > 1) {
        auto triel = pq.Top();
        pq.Pop();
        auto trier = pq.Top();
        pq.Pop();
        Trie* merged = new Trie();
        (*merged).Merge(triel.second, trier.second);
        delete trier.second;
        delete triel.second;
        pq.Push({{triel.first.first + trier.first.first, std::min(triel.first.second, trier.first.second)}, merged});
    }
    Trie* root = pq.Top().second;
    auto ans = (*root).GetAllSymbols();
    delete root;
    return ans;
}

std::vector<std::pair<int, std::string>> Archive::MakeCanonical(std::vector<std::pair<int, std::string>> codes) const {
    std::sort(codes.begin(), codes.end(), [](std::pair<int, std::string>& lhs, std::pair<int, std::string>& rhs) {
        if (lhs.second.size() != rhs.second.size()) {
            return lhs.second.size() < rhs.second.size();
        }
        return lhs.first < rhs.first;
    });
    std::vector<std::pair<int, std::string>> canonical;
    std::string cur_code;
    for (int i = 0; i < codes.size(); i++) {
        while (cur_code.size() < codes[i].second.size()) {
            cur_code += '0';
        }
        canonical.push_back({codes[i].first, cur_code});
        if (i != codes.size() - 1) {
            AddOneToString(cur_code);
        }
    }
    return canonical;
}

void Archive::Compress(const std::string& where_compress, const std::vector<std::string>& to_compress) {
    std::ofstream clear_file = CreateOutStreamWithFile(where_compress, 0);
    std::ofstream out_file = CreateOutStreamWithFile(where_compress, 1);
    Writer out(out_file);
    for (int i = 0; i < to_compress.size(); i++) {
        CompressFile(to_compress[i], i == to_compress.size() - 1, out);
    }
}

std::vector<std::pair<int, std::string>> Archive::EncodeCanonical(const std::vector<int>& symbols,
                                                                  const std::vector<int>& lens) const {
    int symbols_count = static_cast<int>(symbols.size());
    int sum = 0;
    for (auto u : lens) {
        sum += u;
    }
    if (sum != symbols_count) {
        throw std::runtime_error("Encode Canonical: symbols_count != sum(lens)");
    }
    std::vector<std::pair<int, std::string>> canon(symbols_count);
    std::string cur_code;
    int cur_len = -1;
    int pref_cnt = 0;
    for (int i = 0; i < symbols_count; i++) {
        canon[i].first = symbols[i];
        while (pref_cnt <= i) {
            cur_len++;
            pref_cnt += lens[cur_len];
        }
        while (cur_code.size() <= cur_len) {
            cur_code += '0';
        }
        canon[i].second = cur_code;
        if (i != symbols_count - 1) {
            AddOneToString(cur_code);
        }
    }
    return canon;
}

void Archive::Decompress(const std::string& what_decompress) {
    std::ifstream input = CreateInStreamWithFile(what_decompress);
    Reader reader(input);
    int ok = 1;
    while (ok) {
        ok = DecompressFile(reader);
    }
}

void Archive::CompressFile(std::string name, int is_last, Writer& out) {
    std::string only_name = name;
    for (int i = static_cast<int>(name.size()) - 1; i >= 0; i--) {
        if (name[i] == '/') {
            only_name = name.substr(i + 1, name.size() - (i + 1));
            break;
        }
    }
    std::unordered_map<int, int> freq;
    freq[FILENAME_END] = 1;
    freq[ONE_MORE_FILE] = 1;
    freq[ARCHIVE_END] = 1;
    for (auto ch : only_name) {
        freq[ch]++;
    }
    std::ifstream input = CreateInStreamWithFile(name);
    Reader reader(input);
    int cnt_symb = 0;
    while (reader.Tryreadfile()) {
        int cur_symb = reader.Readbits(BYTE);
        cnt_symb++;
        freq[cur_symb]++;
    }
    auto codes = Haffman(freq);
    codes = MakeCanonical(codes);

    std::unordered_map<int, std::string> symbol_code;
    for (auto& u : codes) {
        symbol_code[u.first] = u.second;
    }

    out.WriteBits(SZ, static_cast<int>(symbol_code.size()));

    for (auto& u : codes) {
        out.WriteBits(SZ, u.first);
    }

    std::vector<int> length(codes[codes.size() - 1].second.size() + 1, 0);
    for (auto& u : codes) {
        length[u.second.size()]++;
    }
    for (int i = 1; i < length.size(); i++) {
        out.WriteBits(SZ, length[i]);
    }

    for (auto u : only_name) {
        out.WriteBinaryString(symbol_code[u]);
    }
    out.WriteBinaryString(symbol_code[FILENAME_END]);
    input = CreateInStreamWithFile(name);
    Reader read_to_encode(input);
    int cur_cnt_symb = 0;
    while (read_to_encode.Tryreadfile()) {
        cur_cnt_symb++;
        if (cur_cnt_symb == cnt_symb) {
            break;
        }
        int symb = read_to_encode.Readbits(BYTE);
        out.WriteBinaryString(symbol_code[symb]);
    }
    if (!is_last) {
        out.WriteBinaryString(symbol_code[ONE_MORE_FILE]);
    } else {
        out.WriteBinaryString(symbol_code[ARCHIVE_END]);
    }
}

int Archive::DecompressFile(Reader& in) {
    int symbols_count = in.Readbits(SZ);
    std::vector<int> symbols(symbols_count);
    for (int i = 0; i < symbols_count; i++) {
        symbols[i] = in.Readbits(SZ);
    }
    std::vector<int> length;
    int read = 0;
    while (read < symbols_count) {
        int cnt_for_cur_len = in.Readbits(SZ);
        length.push_back(cnt_for_cur_len);
        read += cnt_for_cur_len;
    }
    auto canon = EncodeCanonical(symbols, length);
    Trie trie(canon);
    const std::unique_ptr<Node>* ptr = nullptr;
    int symb = 0;
    std::string file_for_out;
    bool is_name_of_file = true;
    while (true) {
        int cur_bit = in.Readbits(1);
        if (ptr == nullptr) {
            ptr = &trie.ReadBitByBit();
        }
        ptr = &trie.ReadBitByBit((*ptr), cur_bit);
        if ((*ptr)->terminate != -1) {
            symb = (*ptr)->terminate;
            if (symb > MAX_SYMBOL) {
                throw std::runtime_error("Entered wrong symbol(it's more, than the maximum possible");
            }
            if (symb == ARCHIVE_END) {
                return 0;
            } else if (symb == FILENAME_END) {
                is_name_of_file = false;
                std::ofstream clear_out_file = CreateOutStreamWithFile(file_for_out, 0);
            } else if (symb == ONE_MORE_FILE) {
                return 1;
            } else if (is_name_of_file) {
                file_for_out += static_cast<char>(symb);
            } else {
                std::ofstream write = CreateOutStreamWithFile(file_for_out, 1);
                write << static_cast<unsigned char>(symb);
            }
            ptr = nullptr;
        }
    }
}

std::ifstream Archive::CreateInStreamWithFile(std::string file) {
    std::ifstream input(file, std::ios::binary);
    if (input.fail()) {
        throw std::runtime_error("NO INPUT FILE");
    }
    return input;
}

std::ofstream Archive::CreateOutStreamWithFile(std::string file, int is_app) {
    if (is_app) {
        std::ofstream output(file, std::ios::app);
        return output;
    }
    std::ofstream output(file);
    return output;
}

void Archive::Help() {
    std::cout << "there are 3 commands:" << std::endl;
    std::cout << "1) archiver -c archive_name file1 [file2 ...]" << std::endl;
    std::cout << "this command compresses files file1, file2, ... to file arhive_name. file1, file2, ... must be "
                 "pathes to the files"
              << std::endl;
    std::cout << "2) archiver -d archive_name" << std::endl;
    std::cout << "this command decompresses files from archive_name to current directory" << std::endl;
    std::cout << "3) archiver -h" << std::endl;
    std::cout << "display the help for using the program." << std::endl;
}