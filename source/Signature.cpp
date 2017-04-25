#include <iostream>

#include "Signature.hpp"
#include "TextFileReader.hpp"
#include "SequentialFileWriter.hpp"
#include "Hasher.hpp"

fsig::Signature::Signature(const fsig::Params && params)
    : mParams(params)
    , mThreadPool(std::thread::hardware_concurrency() * 2)
{
    fsig::IWriterSPtr writer = create_writer(mParams);

    fsig::IDataProcessorSPtr processor = create_dataprocessor(mParams);

    const size_t blockSize = mParams.getBlockSize();
    const size_t fileSize = mParams.getSourceFileSize();
    const size_t maxIndex = mParams.getMaxIndex();

    for(size_t i = 0; i <= maxIndex; ++i)
    {
        mThreadPool.add([this, i, blockSize, processor, writer]()
        {
            fsig::IReaderSPtr reader = create_reader(mParams);

            std::unique_ptr<char[]> buf(new char[blockSize]);
            reader->read_data(blockSize * i, buf.get(), blockSize);

            std::string hash;
            processor->process_data(std::string(buf.get(), blockSize), hash);

            writer->write_data(i, hash);
        });
    }
}

void fsig::Signature::wait()
{
    mThreadPool.wait_all();
}

fsig::IReaderSPtr fsig::Signature::create_reader(const Params & params)
{
    return std::make_shared<fsig::TextFileReader>(params.getSourcePath());
}

fsig::IWriterSPtr fsig::Signature::create_writer(const Params & params)
{
    return std::make_shared<fsig::SequentialFileWriter>(mParams.getResultPath(), mParams.getMaxIndex());
}

fsig::IDataProcessorSPtr fsig::Signature::create_dataprocessor(const Params & params)
{
    return std::make_shared<fsig::Hasher>();
}