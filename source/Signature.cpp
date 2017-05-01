#include "Signature.hpp"
#include "TextFileReader.hpp"
#include "SequentialFileWriter.hpp"
#include "Hasher.hpp"

fsig::Signature::Signature(const fsig::Params && params)
    : mParams(params)
    , mThreadPool(std::thread::hardware_concurrency())
{
    fsig::IReaderSPtr reader = create_reader(mParams);
    fsig::IWriterSPtr writer = create_writer(mParams);

    const size_t blockSize = mParams.getBlockSize();
    const size_t maxIndex = mParams.getMaxIndex();

    for(size_t i = 0; i <= maxIndex; ++i)
    {
        mThreadPool.add([this, i, blockSize, reader, writer]()
        {
            try
            {
                //Read
                std::unique_ptr<char[]> buf(new char[blockSize]);
                const size_t readed = reader->read_data(blockSize * i, buf.get(), blockSize);

                //Calc hash
                std::string data(buf.get(), readed);

                fsig::IDataProcessorSPtr processor = create_dataprocessor(mParams);
                std::string hash = processor->process_data(data);

                //Write hash
                writer->write_data(i, hash);
            }
            catch (std::exception & e)
            {
                mExceptionManager.add(e);
                stop();
            }
        });
    }
}

void fsig::Signature::wait()
{
    mThreadPool.wait_all();
    mExceptionManager.throwAll();
}

void fsig::Signature::stop()
{
    mThreadPool.stop();
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
