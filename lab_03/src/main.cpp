#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "AES.h"

#include "CFB.h"

using nlohmann::json;

std::shared_ptr<Encoder> read_aes(std::string keyfile)
{
    std::ifstream stream (keyfile);
    std::stringstream sstream;
    sstream << stream.rdbuf();
    stream.close();
    std::string iv = sstream.str();

    return std::make_shared<AES>(iv);
}

std::shared_ptr<Encoder> read_cfb(std::string filename)
{
    std::ifstream stream (filename);
    json data = json::parse(stream);
    stream.close();

    stream.open(data.at("initial_value"), std::ios::binary | std::ios::in);
    std::stringstream sstream;
    sstream << stream.rdbuf();
    stream.close();
    std::string iv = sstream.str();

    std::list<std::shared_ptr<Encoder>> aes_list;
    std::vector<size_t> offsets;

    for (auto aes : data.at("aes"))
    {
        aes_list.push_back(read_aes(aes.at("key").get<std::string>()));
        offsets.push_back(aes.at("offset").get<size_t>());
    }

    return std::make_shared<CFB>(aes_list, offsets, iv, iv.length());
}

int main(int argc, char **argv)
{
    if (4 != argc)
        return 1;

    using Method = std::string (Encoder::*)(const std::string &);
    Method method = &Encoder::encode;
    std::string op = argv[1];

    if ("dec" == op)
        method = &Encoder::decode;
    else if ("enc" != op)
        return 2;

    std::shared_ptr<Encoder> encoder = read_cfb("config.json");

    std::string str, enc, dec;

    std::ifstream fin;
    std::ofstream fout;
    fin.open(argv[2], std::ios::binary | std::ios::in);
    fout.open(argv[3], std::ios::binary | std::ios::out);

    std::stringstream buffer;
    buffer << fin.rdbuf();

    std::string res = ((encoder.get())->*method)(buffer.str());
    fout.write(res.c_str(), res.length());

    fin.close();
    fout.close();

    return 0;
}

