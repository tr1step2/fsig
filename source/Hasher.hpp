#pragma once

#include "IDataProcessor.hpp"

namespace fsig
{

struct Hasher : IDataProcessor
{
    virtual bool process_data(const std::string & data, std::string & data_out) override;
};

} //ns fsig
