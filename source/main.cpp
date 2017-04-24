#include <iostream>
#include <thread>
#include <functional>
#include <fstream>
#include <array>
#include "ThreadPool.hpp"

#include <boost/signals2.hpp>

#include <openssl/sha.h>
#include <openssl/engine.h>

struct ReadWorker
{
    ReadWorker(size_t index, size_t offset, size_t size, std::string & filename, std::function<void(ReadWorker &)> callback)
        : index(index)
        , offset(offset)
        , size(size)
        , filename(filename)
        , mCallback(callback)
    {
    }

    size_t index;
    size_t offset;
    size_t size;

    std::string data;

    bool result = false;
    std::string & filename;

    std::function<void(ReadWorker &)> mCallback;


    void operator()()
    {
        std::ifstream ifs;

        //open as text file
        ifs.open(filename, std::ifstream::in);
        if(ifs)
        {
            std::cout << "Can't open file " << filename;
            return;
        }

        //offset, way
        ifs.seekg(offset, std::ios_base::beg);

        std::string read = read_data(ifs, size);
        std::string hash = calc_hash(std::move(read));

        result = true;

        mCallback(*this);
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
        char outputBuffer[65];
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

        return outputBuffer;
    }
};

int main(void)
{
    fsig::ThreadPool tp(std::thread::hardware_concurrency());

    const std::string filename = "input";
    const std::string fileout = "output";
    const size_t size = 10;

    auto callback = [](ReadWorker & worker)
    {
    };

    for(size_t i = 0; i < 5; ++i)
    {
        auto w = [i, &filename, &fileout, size, callback]()
        {
            size_t offset = i * size;

            //size_t index, size_t offset, size_t size, std::string & filename, std::function<void(ReadWorker &)> callback)
            ReadWorker reader(i, offset, size, filename, callback);
        };

        tp.add(w);
    }

    tp.wait_all();
}
