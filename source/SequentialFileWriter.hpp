#pragma once

#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "IWriter.hpp"

namespace fsig
{

struct SequentialFileWriter : IWriter
{
    SequentialFileWriter(const std::string & filename, size_t sequencesSize);
    ~SequentialFileWriter();

    virtual bool write_data(const size_t index, const std::string & data) override;

private:
    void try_write();

private:
    const std::string mFileName;
    const size_t mSequencesSize;
    size_t mLastIndex = 0;

    std::deque<std::string> mStorage;

    std::recursive_mutex mStorageMutex;
    std::mutex mWriteMutex;
};

} //ns fsig
