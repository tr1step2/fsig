#include <boost/crc.hpp>

#include "Hasher.hpp"

std::string fsig::Hasher::process_data(const std::string & data)
{
    /*boost::crc_32_type result;
    result.process_bytes(data.data(), data.length());

    return std::to_string(result.checksum());
	*/
	return data;
}
