#include <boost/crc.hpp>

#include "Hasher.hpp"

std::string fsig::Hasher::process_data(const std::string & data)
{
    boost::crc_32_type result;

    for(size_t i = 0; i < 10; ++i)
    {
        result.process_bytes(data.data(), data.length());
    }

    //return std::to_string(result.checksum());
    return data;
}
