#include "Signature.hpp"
#include "TextFileReader.hpp"
#include "SequentialFileWriter.hpp"
#include "Hasher.hpp"

fsig::Signature::Signature(const fsig::Params && params)
    : mParams(params)
    , mThreadPool(std::thread::hardware_concurrency() * 2)
{
    fsig::IWriterSPtr writer = create_writer(mParams);

    const size_t blockSize = mParams.getBlockSize();
    const size_t fileSize = mParams.getSourceFileSize();
    const size_t maxIndex = mParams.getMaxIndex();

    for(size_t i = 0; i <= maxIndex; ++i)
    {
        mThreadPool.add([this, i, blockSize, writer]()
        {
            //Read
            std::unique_ptr<char[]> buf(new char[blockSize]);
            fsig::IReaderSPtr reader = create_reader(mParams);

            size_t readed = reader->read_data(blockSize * i, buf.get(), blockSize);

            //Calc hash
            std::string data(buf.get(), readed);
            std::string hash;

            fsig::IDataProcessorSPtr processor = create_dataprocessor(mParams);
            processor->process_data(data, hash);

            //Write hash
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
    return std::make_shared<fsig::SequentialFileWriter>(params.getResultPath(),
                                                        params.getMaxIndex() + 1);
}

fsig::IDataProcessorSPtr fsig::Signature::create_dataprocessor(const Params & params)
{
    return std::make_shared<fsig::Hasher>();
}
