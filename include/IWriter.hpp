#pragma once

#include <string>
#include <memory>

namespace fsig
{

struct IWriter
{
    virtual ~IWriter(){};
    virtual bool write_data(const size_t index, const std::string & data) = 0;
};

using IWriterSPtr = std::shared_ptr<IWriter>;

} // ns fsig
