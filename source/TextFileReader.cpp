#include <fstream>
#include <cstring>

#include "TextFileReader.hpp"

namespace fsig
{
namespace constants
{
    constexpr size_t defaultreadBufferSize = 10 * 1024 * 1024;
}
}

fsig::TextFileReader::TextFileReader(const std::string & filename)
    : mStream(filename, std::ifstream::in)
    , mBuffer(new char[fsig::constants::defaultreadBufferSize])
{
    if(!mStream)
        throw std::runtime_error("Can't open file to read: " + filename);

    mStream.rdbuf()->pubsetbuf(mBuffer.get(), fsig::constants::defaultreadBufferSize);
}

size_t fsig::TextFileReader::read_data(const size_t offset, char * buf, const size_t size)
{
    std::lock_guard<std::mutex> lock(mReadMutex);

    mStream.seekg(offset, std::ios_base::beg);
    mStream.read(buf, size);

    return mStream.gcount();
}
