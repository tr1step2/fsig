#include "Constants.hpp"
#include "Params.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

fsig::Params::Params(int argc, char **argv)
{
    if(argc < 3)
        throw std::runtime_error("Not enough arguments.");
    try
    {
        mSourcePath = argv[1];
        mResultPath = argv[2];
        mBlockSize = argc > 3 ? boost::lexical_cast<size_t>(argv[3]) :
                                fsig::constants::defaultBlockSize;

        check_path(mSourcePath);
        mSourceFileSize = boost::filesystem::file_size(mSourcePath);

        mMaxIndex = mSourceFileSize / mBlockSize;
    }
    catch(const boost::bad_lexical_cast &)
    {
        throw std::runtime_error("Bad size argument.");
    }
}

std::string fsig::Params::getSourcePath() const
{
    return mSourcePath;
}

std::string fsig::Params::getResultPath() const
{
    return mResultPath;
}

size_t fsig::Params::getBlockSize() const
{
    return mBlockSize;
}

size_t fsig::Params::getSourceFileSize() const
{
    return mSourceFileSize;
}

size_t fsig::Params::getMaxIndex() const
{
    return mMaxIndex;
}

void fsig::Params::check_path(const std::string & path)
{
    if (!boost::filesystem::exists(path))
    {
        boost::filesystem::path full_path(boost::filesystem::current_path());
        full_path.concat("\\" + path);
        if (!boost::filesystem::exists(full_path))
            throw std::runtime_error("File " + path + " should exists");
    }
}
