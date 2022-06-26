

#ifndef HELPER_HPP
#define HELPER_HPP

#include <fstream>
#include <iostream>

using namespace std;

/**
 * Conveniently crash with error messages
 *
 * @param message error messages
 */
void error(const string& message);

/**
 * Handle reading from a file.
 */
class FancyInputStream {
private:
    // member variables (aka instance variables)
    string FILENAME;       // input file's name
    ifstream input_file;   // input stream from which to read
    char buffer;           // bitwise buffer (holds 8 bits = 1 byte)
    char buffer_index;     // current index of bitwise buffer

public:
    /**
     * Constructor, which initializes a FancyInputStream object to read from the
     * given file
     *
     * @param filename path to the file
     */
    explicit FancyInputStream(const string& filename);

    /**
     * See: https://www.cplusplus.com/reference/ios/ios/good/
     *
     * @return true if none of the stream's error state flags (eofbit, failbit
     * and badbit) is set.
     */
    bool good() const;

    /**
     * Return the size of the input file
     *
     * @return size of the input file
     */
    long filesize() const;

    /**
     * Move back to the beginning of the input file and clear bitwise buffer
     */
    void reset();

    /**
     * Read a generic data type from the file,
     * or crash if there are not enough bytes left in the file
     *
     * @example read a char: char data = inFile.read<char>();
     * @example read a short: short data = inFile.read<short>();
     * @example read a int: int data = inFile.read<int>();
     *          inFile is an instance of FancyInputStream
     *
     * @tparam T type, can be int(4 bytes), short(2 bytes), or char(1 byte)
     * @return data of type T from file
     */
    template<typename T> T read();

    /**
     * Read a single bit from the file as an int that is either 0 or 1,
     * or crash if there are not enough bytes left in the file
     *
     * @return a single bit, 1 or 0.
     */
    char read_bit();
};

/**
 * Handle writing to a file.
 */
class FancyOutputStream {
private:
    // member variables (aka instance variables)
    ofstream output_file; // output stream to which to write
    char buffer; // bitwise buffer (holds 8 bits = 1 byte)
    int buffer_index;     // current index of bitwise buffer

public:
    /**
     * Constructor, which initializes a FancyOutputStream object to write to
     * the given file
     *
     * @param filename path to the file
     */
    explicit FancyOutputStream(const string& filename);

    /**
     * Destructor, which flushes everything
     */
    ~FancyOutputStream();

    /**
     * See: https://www.cplusplus.com/reference/ios/ios/good/
     *
     * @return true if none of the stream's error state flags (eofbit, failbit
     * and badbit) is set.
     */
    bool good() const;

    /**
     * Write a generic data type to the file.
     *
     * @example write a char:  inFile.write<char>(dat);
     * @example write a short: inFile.write<short>(dat);
     * @example write a int:   inFile.write<int>(dat);
     *          inFile is an instance of FancyOutputStream
     *
     * @tparam T type, can be int(4 bytes), short(2 bytes), or char(1 byte)
     * @param data data to be written
     */
    template<typename T> void write(const T& data);

    /**
     * Write a single bit to the file
     *
     * @param bit a single bit, 1 or 0.
     */
    void write_bit(char const& bit);

    /**
     * Flush the bitwise buffer to the ofstream
     */
    void flush_bitwise();

    /**
     * Flush everything to the file
     */
    void flush();
};

#include "Helper.tcc" // template implementations need to be visible to
// the compiler in the header file

/**
 * Represent nodes in an HCTree (Huffman Tree) object
 */
class HCNode {
public:
    // member variables (aka instance variables)
    int count;            // count of this node
    unsigned char symbol; // symbol of this node
    HCNode* c0;           // pointer to '0' child
    HCNode* c1;           // pointer to '1' child
    HCNode* p;            // pointer to parent

    /**
     * Constructor, which initializes an HCNode object with a given count and
     * symbol.
     *
     * @param count see above
     * @param symbol see above
     */
    HCNode(int count, unsigned char symbol);

    /**
     * Less-than operator to compare HCNodes deterministically
     *
     * @param other the other node to compare
     */
    bool operator<(const HCNode& other) const;
};

/**
 * A 'function class' for use as the Compare class in a priority_queue<HCNode*>.
 * For this to work, operator< must be defined to do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    /**
     * compare two nodes
     * @param lhs left-hand-side node
     * @param rhs right-hand-side node
     * @return lhs < rhs
     */
    bool operator()(HCNode*& lhs, HCNode*& rhs) const;
};

#endif // HELPER_HPP

