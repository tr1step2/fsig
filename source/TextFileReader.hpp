#pragma once

#include "IReader.hpp"

namespace fsig
{

struct TextFileReader : IReader
{
    TextFileReader(const std::string & filename);
    virtual size_t read_data(const size_t offset, char *buf, const size_t size) override;

private:
    std::string mFilename;
};

} //ns fsig
