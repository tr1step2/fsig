#pragma once

#include <thread>

#include "Params.hpp"
#include "ThreadPool.hpp"
#include "ExceptionManager.hpp"

#include "IReader.hpp"
#include "IWriter.hpp"
#include "IDataProcessor.hpp"

namespace fsig
{

struct Signature
{
    Signature(const fsig::Params && params);

    void wait();

private:
    void stop();

    IReaderSPtr create_reader(const Params & params);
    IWriterSPtr create_writer(const Params & params);
    IDataProcessorSPtr create_dataprocessor(const Params & params);

private:
    Params mParams;
    ThreadPool mThreadPool;
    ExceptionManager mExceptionManager;
};

} //ns fsig
