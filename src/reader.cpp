#include "reader.h"
#include <iostream>

const int BYTE = 8;
const int MAX_INT = 30;

Reader::Reader(std::istream& stream) : str_(stream) {
    buffer_ = BYTE;
    is_file_ended_ = false;
}

void Reader::Updatebuffer() {
    if (is_file_ended_) {
        return;
    }
    if (buffer_ == BYTE) {
        if (str_.eof()) {
            is_file_ended_ = true;
            return;
        }
        cur_byte_ = str_.get();
        buffer_ = 0;
    }
}

bool Reader::Tryreadfile() {
    Updatebuffer();
    return !is_file_ended_;
}

int Reader::Readbits(int count) {
    if (!Tryreadfile()) {
        throw std::runtime_error("reader: try read a non-existent symbol");
    }
    if (count > MAX_INT) {
        throw std::runtime_error("reader: overflow");
    }
    int ans = 0;
    for (int i = 0; i < count; i++) {
        Updatebuffer();
        int is_one = ((cur_byte_ & (1 << buffer_)) != 0);
        if (is_one) {
            ans += (1 << i);
        }
        buffer_++;
    }
    Updatebuffer();
    return ans;
}