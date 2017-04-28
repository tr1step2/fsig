#include <fstream>
#include <cstring>

#include "Constants.hpp"
#include "TextFileReader.hpp"

fsig::TextFileReader::TextFileReader(const std::string & filename)
    : mStream(filename, std::ifstream::binary)
    , mBuffer(new char[fsig::constants::defaultReadBufferSize])
{
    if(!mStream)
        throw std::runtime_error("Can't open file to read: " + filename);

    mStream.rdbuf()->pubsetbuf(mBuffer.get(), fsig::constants::defaultReadBufferSize);
}

size_t fsig::TextFileReader::read_data(const size_t offset, char * buf, const size_t size)
{
    std::lock_guard<std::mutex> lock(mReadMutex);

    mStream.seekg(offset, std::ios_base::beg);
    mStream.read(buf, size);

    return mStream.gcount();
}
