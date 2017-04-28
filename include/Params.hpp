#pragma once

#include <string>

namespace fsig
{

struct Params
{
    Params(int argc, char ** argv);

    std::string getSourcePath() const;
    std::string getResultPath() const;
    size_t getBlockSize() const;
    size_t getSourceFileSize() const;
    size_t getMaxIndex() const;

private:
    void check_path(const std::string & path);

private:
    std::string mSourcePath;
    std::string mResultPath;
    size_t mBlockSize;
    size_t mMaxIndex;

    size_t mSourceFileSize;
};

} //ns fsig
