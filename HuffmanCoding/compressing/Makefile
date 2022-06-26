# use g++ with C++11 support
CXX=g++
CXXFLAGS?=-Wall -pedantic -g -O0 -std=c++11
OUTFILES=compress decompress

all: $(OUTFILES)

compress: compress.cpp Helper.cpp HCTree.cpp Helper.hpp HCTree.hpp
	$(CXX) $(CXXFLAGS) -o compress compress.cpp Helper.cpp HCTree.cpp

decompress: decompress.cpp Helper.cpp HCTree.cpp Helper.hpp HCTree.hpp
	$(CXX) $(CXXFLAGS) -o decompress decompress.cpp Helper.cpp HCTree.cpp

clean:
	rm -f $(OUTFILES) *.o

