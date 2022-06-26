/*
 * Name: CSE 100 Staff
 * Email: cs100sp22@ucsd.edu
 *
 * Sources Used: None.
 *
 * This file provides the template implementations for read and write in
 * FancyInputStream/FancyOutputStream.
 */


template<typename T>
void FancyOutputStream::write(const T &data) {
    if (buffer_index != 0) {
        error("Attempting to write byte when bitwise buffer is not empty");
    }
    output_file.write((char*)&data, sizeof(T));
}

template<typename T>
T FancyInputStream::read() {
    if (buffer_index != 8) {
        error("Attempt to read when bitwise buffer is not empty");
    }
    T num;
    input_file.read((char *) &num,sizeof(T));
    return num;
}
