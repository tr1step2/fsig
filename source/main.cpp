#include <iostream>
#include <thread>
#include <functional>
#include <fstream>
#include <array>
#include <mutex>
#include <condition_variable>

#include <boost/signals2.hpp>

#include <openssl/sha.h>
#include <openssl/engine.h>

#include "ThreadPool.hpp"

struct ReadWorker
{
    ReadWorker(size_t index, size_t offset, size_t size, std::string & filename, std::function<void(ReadWorker &)> callback)
        : index(index)
        , offset(offset)
        , size(size)
        , filename(filename)
        , callback(callback)
    {
    }

    ReadWorker()
    {

    }

    size_t index;
    size_t offset;
    size_t size;

    std::string data;

    bool result = false;
    std::string filename;

    std::function<void(ReadWorker &)> callback;


    void operator()()
    {
        std::ifstream ifs(filename, std::ifstream::in);
        if(!ifs.is_open())
        {
            std::cout << std::this_thread::get_id() << " | Can't open file " << filename << std::endl;
            return;
        }

        //offset, way
        ifs.seekg(offset, std::ios_base::beg);

        std::string read = read_data(ifs, size);
        data = calc_hash(std::move(read));


        result = true;

        callback(*this);
    }

private:
    std::string read_data(std::ifstream & ifs, const size_t size)
    {
        char buf[size] = {};

        ifs.read(buf, size);
        return {buf, size};
    }

    std::string calc_hash(std::string && data)
    {
        return generateSHA256(std::move(data));
    }

    std::string generateSHA256(std::string && value)
    {
        /*char outputBuffer[65];
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, value.c_str(), value.length());
        SHA256_Final(hash, &sha256);

        for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        {
            sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
        }

        outputBuffer[64] = 0;

        return outputBuffer;*/
        return value;
    }
};

int main(void)
{
    fsig::ThreadPool tp(std::thread::hardware_concurrency());

    const std::string filename = "input";
    const std::string fileout = "output";
    const size_t size = 10;

    std::mutex mutex;

    std::vector<std::string> data(5);

    auto callback = [&mutex, &data](ReadWorker worker)
    {
        std::lock_guard<std::mutex> lock(mutex);
        data[worker.index] = worker.data;
    };

    for(size_t i = 0; i < 5; ++i)
    {
        auto w = [i, &filename, &fileout, size, callback]()
        {
            size_t offset = i * size;

            //size_t index, size_t offset, size_t size, std::string & filename, std::function<void(ReadWorker &)> callback)
            ReadWorker reader;
            reader.index = i;
            reader.offset = offset;
            reader.size = size;
            reader.filename = filename;
            reader.callback = callback;

            reader();
        };

        tp.add(w);
    }

    tp.wait_all();

    for(const auto & d : data)
    {
        std::cout << d << std::endl;
    }
}
