#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

/* Prints the usage message
 *
 * @param error case of error to print out
 */
void printErrors(int error)
{
    switch (error)
    {
    case 1:
        cerr << "Usage: ./compress <original_file> "
            << "compressed_file>" << endl;
        exit(1);
    case 2:
        cerr << "Error on Original File. The stream's error"
            << "state flags (eofbit, failbit and badbit) is set" << endl;
        exit(1);
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    // Check command line arguments and print out errors
    if (argc != 3)
        printErrors(1);

    //Open input and output files for reading and writing
    FancyInputStream original_file("dna.txt");
    FancyOutputStream compressed_file("dnaCompressed.txt");

    // Check files and print out errors
    if (!original_file.good())
        printErrors(2);

    if (original_file.filesize() == 0)
        return 0;

    vector<int> frequency(256, 0);
    int count = 0;

    // Read header file
    while (true)
    {
        auto read_byte = original_file.read<unsigned char>();
        if (!original_file.good())
            break;
        frequency[read_byte]++;
        count++;
    }

    original_file.reset();

    //Build Huffman Tree
    HCTree huffmanTree;
    huffmanTree.build(frequency);

    // Write header file
    huffmanTree.serialize(compressed_file);

    for (int i = 0; i < count; i++)
    {
        auto read_byte = original_file.read<unsigned char>();
        huffmanTree.encode(read_byte, compressed_file);
    }

    return 0;
}