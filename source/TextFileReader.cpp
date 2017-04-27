#include <fstream>
#include <cstring>

#include "TextFileReader.hpp"

fsig::TextFileReader::TextFileReader(const std::string & filename)
    : mFilename(filename)
{
}

size_t fsig::TextFileReader::read_data(const size_t offset, char * buf, const size_t size)
{
    std::ifstream ifs(mFilename, std::ifstream::in);
    if(!ifs)
        throw std::runtime_error("Can't open file to read: " + mFilename);

    ifs.seekg(offset, std::ios_base::beg);
    ifs.read(buf, size);

    return ifs.gcount();
}
