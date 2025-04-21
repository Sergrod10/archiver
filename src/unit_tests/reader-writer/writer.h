#pragma once

#include <iostream>
#include <string>

class Writer {
public:
    explicit Writer(std::ostream& out);

    ~Writer();

    void WriteBits(int bits, int val);

    void WriteBinaryString(const std::string& st);

private:
    std::ostream& str_;
    int buffer_;
    unsigned char cur_byte_;

    void WriteBit(int type);
};