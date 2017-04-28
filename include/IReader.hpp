#pragma once

#include <string>
#include <memory>

namespace fsig
{

struct IReader
{
    virtual ~IReader(){};
    virtual size_t read_data(const size_t offset, char * buf, const size_t size) = 0;
};

using IReaderSPtr = std::shared_ptr<IReader>;

} //ns fsig
