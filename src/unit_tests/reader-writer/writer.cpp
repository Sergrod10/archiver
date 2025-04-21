#include "writer.h"

const int BYTE = 8;
const int MAX_INT = 30;

Writer::Writer(std::ostream &out) : str_(out) {
    buffer_ = 0;
    cur_byte_ = 0;
}

Writer::~Writer() {
    str_ << cur_byte_;
}

void Writer::WriteBit(int type) {
    if (buffer_ == BYTE) {
        str_ << cur_byte_;
        buffer_ = 0;
        cur_byte_ = 0;
    }
    if (type) {
        cur_byte_ |= (1 << buffer_);
    }
    buffer_++;
}

void Writer::WriteBits(int bits, int val) {
    if (bits > MAX_INT) {
        throw std::runtime_error("writer: overflow");
    }
    for (int i = 0; i < bits; i++) {
        WriteBit(((1 << i) & val) != 0);
    }
}

void Writer::WriteBinaryString(const std::string &st) {
    for (auto u : st) {
        WriteBit(u - '0');
    }
}