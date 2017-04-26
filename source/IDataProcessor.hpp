#pragma once

#include <string>
#include <memory>

namespace fsig
{

struct IDataProcessor
{
    virtual ~IDataProcessor(){};
    virtual std::string process_data(const std::string & data) = 0;
};

using IDataProcessorSPtr = std::shared_ptr<IDataProcessor>;

} //ns fsig
