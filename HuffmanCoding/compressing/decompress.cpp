//
//
//#include "HCTree.hpp"
//#include "Helper.hpp"
//#include <iostream>
//#include <string>
//#include <vector>
//#include <stdio.h>
//using namespace std;
//
///* Prints the usage message
// *
// * @param error case of error to print out
// */
//void printErrors(int error)
//{
//    switch (error)
//    {
//    case 1:
//        cerr << "Usage: ./compress <original_file> "
//            << "compressed_file>" << endl;
//        exit(1);
//    case 2:
//        cerr << "Error on Original File. The stream's error"
//            << "state flags (eofbit, failbit and badbit) is set" << endl;
//        exit(1);
//    default:
//        break;
//    }
//}
//
//int main(int argc, char* argv[])
//{
//    // Check command line arguments and print out errors
//    if (argc < 3)
//        printErrors(1);
//
//    //Open input and output files for reading and writing
//    FancyInputStream compressed_file("dnaCompressed.txt");
//    FancyOutputStream decompressed_file("dnaDecompressed.txt");
//
//    //Open input and output files
//    if (!compressed_file.good())
//        printErrors(2);
//
//    if (compressed_file.filesize() == 0)
//        return 0;
//
//    vector<int> frequency(256, 0);
// 
//    //Build Huffman Tree
//    HCTree huffmanTree;
//    huffmanTree.deserialize(compressed_file);
//
//    //Decode and write to the output file
//    for (auto i = 0; i < huffmanTree.root->count; i++)
//    {
//        if (!compressed_file.good())
//            break;
//        auto decode = huffmanTree.decode(compressed_file);
//        decompressed_file.write<unsigned char>(decode);
//        //decompressed_file.flush();
//    }
//
//    return 0;
//}