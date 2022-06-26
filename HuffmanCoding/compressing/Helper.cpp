
#include "Helper.hpp"

 // error function implementation
void error(const string& message) {
    throw std::logic_error(message);
}

// FancyInputStream function implementations
FancyInputStream::FancyInputStream(const string& filename) : FILENAME(filename),
input_file(ifstream(filename, ios::binary)), buffer(0),
buffer_index(8) {}

bool FancyInputStream::good() const {
    return input_file.good();
}

long FancyInputStream::filesize() const {
    return ifstream(FILENAME, ios::ate | ios::binary).tellg();
}

void FancyInputStream::reset() {
    input_file.clear();  // clear EOF flag
    input_file.seekg(0); // move to begining of file
    buffer = 0;          // clear bitwise buffer
    buffer_index = 8;    // move bitwise buffer index back to beginning
}

char FancyInputStream::read_bit() {
    // if there are no more bits to read in the buffer,
    if (buffer_index == 8) {
        char const& tmp = read<char>(); // try to read the next byte

        // we read a byte successfully, so update our buffer
        buffer = tmp;
        buffer_index = 0;
    }

    // read the next bit from the bitwise buffer
    return (buffer >> (7 - buffer_index++)) & 1;
}

// FancyOutputStream function implementations
FancyOutputStream::FancyOutputStream(const string& filename) : output_file(
    ofstream(filename, ios::binary)), buffer(0), buffer_index(0) {}

FancyOutputStream::~FancyOutputStream() {
    flush();
}

bool FancyOutputStream::good() const {
    return output_file.good();
}

void FancyOutputStream::write_bit(const char& bit) {
    // crash if invalid input
    if (bit != 0 && bit != 1) {
        error("Trying to write invalid bit");
    }

    // add bit to bitwise buffer
    buffer |= (bit << (7 - buffer_index++));

    // if the bitwise buffer is full,
    if (buffer_index == 8) {
        flush_bitwise(); // flush it
    }
}

void FancyOutputStream::flush_bitwise() {
    // if we have bits in our bitwise buffer,
    if (buffer_index != 0) {
        buffer_index = 0;        // reset the buffer index
        this->write<char>(buffer); // write the bitwise buffer
        buffer = 0;              // reset the buffer
    }
}

void FancyOutputStream::flush() {
    flush_bitwise();     // try to flush the bitwise buffer
    output_file.flush(); // flush the ofstream
}

// HCNode function implementations
HCNode::HCNode(int count, unsigned char symbol) : count(count), symbol(symbol),
c0(nullptr), c1(nullptr),
p(nullptr) {}

bool HCNode::operator<(const HCNode& other) const {
    // if the counts are different, compare counts
    if (count != other.count) {
        return count > other.count;
    }

    // if the counts are equal, use symbol to break tie
    return symbol > other.symbol;
}

bool HCNodePtrComp::operator()(HCNode*& lhs, HCNode*& rhs) const {
    return *lhs < *rhs;
}
