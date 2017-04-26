#pragma once

#include "IDataProcessor.hpp"

namespace fsig
{

struct Hasher : IDataProcessor
{
    virtual std::string process_data(const std::string & data) override;
};

} //ns fsig
