#pragma once

#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "IWriter.hpp"
#include "CycleWorker.hpp"

namespace fsig
{

struct SequentialFileWriter : IWriter
{
    SequentialFileWriter(const std::string & filename, size_t sequencesSize);

    virtual bool write_data(const size_t index, const std::string & data) override;

private:
    void writeToDisk();
    void createThread();

private:
    const std::string mFileName;
    const size_t mSequencesSize;
    size_t mLastIndex = 0;

    std::deque<std::string> mStorage;

    std::mutex mStorageMutex;
    std::mutex mWriteMutex;

    CycleWorker mWriteWorker;
};

} //ns fsig
