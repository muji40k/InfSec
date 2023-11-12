#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "DES.h"
#include "DESBlocks.h"

#include "PCBC.h"

using nlohmann::json;

std::vector<size_t> read_map(std::string filename)
{
    std::vector<size_t> map;
    size_t c;

    std::ifstream fin;
    fin.open(filename, std::ios::binary | std::ios::in);

    while (!fin.eof())
    {
        fin.read((char *)&c, sizeof(size_t));

        if (sizeof(size_t) == fin.gcount())
            map.push_back(c);
    }

    fin.close();

    return map;
}

void read_s_block(std::string filename, std::array<std::array<char, 16>, 4> &arr)
{
    std::ifstream fin;
    fin.open(filename, std::ios::binary | std::ios::in);

    for (size_t i = 0, current; 4 > i; i++)
        for (size_t j = 0; 16 > j; j++)
        {
            fin.read((char *)&current, sizeof(size_t));

            if (fin.eof() || sizeof(size_t) != fin.gcount())
                throw;

            arr[i][j] = current;
        }

    fin.close();
}

std::shared_ptr<FBlock> read_f_block(std::string path, json data)
{
    path += data.at("path").get<std::string>() + '/';

    std::shared_ptr<EBlock> e_block = std::make_shared<BasicEBlock>();
    std::vector<std::shared_ptr<SBlock>> s_blocks (8);
    auto arr = data.at("s_blocks");
    auto iter = arr.begin();
    std::array<std::array<char, 16>, 4> tmp;

    for (size_t i = 0; 8 > i; i++, ++iter)
    {
        read_s_block(path + (*iter).get<std::string>(), tmp);
        s_blocks[i] = std::make_shared<BasicSBlock>(tmp);
    }

    std::shared_ptr<FPBlock> fp_block = \
        std::make_shared<BasicFPBlock>(std::make_shared<BitTable>(read_map(path + data.at("p_block").get<std::string>()), 32));

    return std::make_shared<FBlock>(e_block, s_blocks, fp_block);
}

std::shared_ptr<KeyBlock> read_key_block(std::string path, json data)
{
    path += data.at("path").get<std::string>() + '/';

    std::shared_ptr<PKeyBlock> pk_block = \
        std::make_shared<BasicPKeyBlock>(std::make_shared<BitTable>(read_map(path + data.at("p_block").at("c").get<std::string>()), 56),
                                         std::make_shared<BitTable>(read_map(path + data.at("p_block").at("d").get<std::string>()), 56));

    std::shared_ptr<HKeyBlock> hk_block = \
        std::make_shared<BasicHKeyBlock>(std::make_shared<BitTable>(read_map(path + data.at("h_block").get<std::string>()), 56));

    std::vector<std::shared_ptr<ShiftKeyBlock>> shift_blocks (16);
    char key[8];

    std::ifstream stream (path + data.at("shift_blocks").get<std::string>(),
                          std::ios::binary | std::ios::in);

    for (size_t i = 0, shift; 16 > i; i++)
    {
        stream.read((char *)&shift, sizeof(size_t));

        if (stream.eof() || sizeof(size_t) != stream.gcount())
            throw;

        shift_blocks[i] = std::make_shared<BasicShiftKeyBlock>(std::make_shared<BitShifter>(28, shift));
    }

    stream.close();

    stream.open(path + data.at("key").get<std::string>(),
                std::ios::binary | std::ios::in);
    stream.read(key, 8);

    if (8 != stream.gcount())
        throw;

    stream.close();

    BitRange rng ((char *)key, 0, 64);

    return std::make_shared<KeyBlock>(rng, pk_block, shift_blocks, hk_block);
}

std::shared_ptr<DES> read_des(std::string filename)
{
    std::string path = filename.substr(0, filename.find_last_of('/') + 1);

    std::ifstream stream (filename);
    json data = json::parse(stream);
    stream.close();

    std::shared_ptr<FBlock> f_block = read_f_block(path, data.at("f_block"));
    std::shared_ptr<KeyBlock> key_block = read_key_block(path, data.at("key_block"));

    std::shared_ptr<IPBlock> ip_block = \
        std::make_shared<BasicIPBlock>(std::make_shared<BitTable>(read_map(path + data.at("ip_block").get<std::string>()), 64));

    return std::make_shared<DES>(ip_block, f_block, key_block);
}

std::shared_ptr<PCBC> read_pcbc(std::string filename)
{
    std::ifstream stream (filename);
    json data = json::parse(stream);
    stream.close();

    char iv[8];

    stream.open(data.at("initial_value"), std::ios::binary | std::ios::in);
    stream.read(iv, 8);

    if (8 != stream.gcount())
    {
        stream.close();
        throw;
    }

    stream.close();
    std::list<std::shared_ptr<DES>> des_list;

    for (auto des : data.at("des_blocks"))
        des_list.push_back(read_des(des.get<std::string>()));

    return std::make_shared<PCBC>(des_list, iv);
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

    std::shared_ptr<Encoder> encoder = read_pcbc("config.json");

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

