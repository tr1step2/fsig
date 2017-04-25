#pragma once

#include <string>
#include <memory>

namespace fsig
{

struct IDataProcessor
{
    virtual ~IDataProcessor(){};
    virtual bool process_data(const std::string & data, std::string & data_out) = 0;
};

using IDataProcessorSPtr = std::shared_ptr<IDataProcessor>;

} //ns fsig
