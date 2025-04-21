#pragma once

#include <iostream>

class Reader {
public:
    explicit Reader(std::istream& stream);

    int Readbits(int count);

    bool Tryreadfile();

private:
    std::istream& str_;
    unsigned char cur_byte_;
    int buffer_;
    bool is_file_ended_;

    void Updatebuffer();
};