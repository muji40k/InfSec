#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "RSA.h"
#include "MD5.h"

using nlohmann::json;

void print_md5(std::string hash)
{
    for (size_t i = 0; 16 > i; i++)
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)((unsigned char)hash[i]);
}

std::shared_ptr<Encoder> read_rsa_ds_encoder(std::string filename)
{
    std::ifstream stream (filename);
    json data = json::parse(stream);
    stream.close();

    std::shared_ptr<RSA> rsa = std::make_shared<RSA>(data.at("n").get<unsigned long long>());
    rsa->setPublicKey(data.at("d").get<unsigned long long>());

    return rsa;
}

std::shared_ptr<Encoder> read_rsa_ds_decoder(std::string filename)
{
    std::ifstream stream (filename);
    json data = json::parse(stream);
    stream.close();

    std::shared_ptr<RSA> rsa = std::make_shared<RSA>(data.at("n").get<unsigned long long>());
    rsa->setPrivateKey(data.at("e").get<unsigned long long>());

    return rsa;
}

std::shared_ptr<Encoder> read_rsa(std::string filename)
{
    std::ifstream stream (filename);
    json data = json::parse(stream);
    stream.close();

    unsigned long long n = data.at("n").get<unsigned long long>();

    std::shared_ptr<RSA> rsa = std::make_shared<RSA>(n);

    if (data.contains("e"))
        rsa->setPrivateKey(data.at("e").get<unsigned long long>());

    if (data.contains("d"))
        rsa->setPrivateKey(data.at("d").get<unsigned long long>());

    return rsa;
}

int verify(std::shared_ptr<HashFunction> hash, std::shared_ptr<Encoder> encoder,
           std::string filename, std::string sfilename)
{
    std::ifstream fin;
    fin.open(sfilename, std::ios::binary | std::ios::in);

    std::stringstream buffer;
    buffer << fin.rdbuf();
    fin.close();

    std::string signature = encoder->decode(buffer.str());
    signature.resize(16);

    std::cout << "Signature: ";
    print_md5(signature);
    std::cout << std::endl;

    fin.open(filename, std::ios::binary | std::ios::in);
    buffer.str("");
    buffer << fin.rdbuf();
    fin.close();

    std::string res = hash->apply(buffer.str());
    std::cout << "Given:     ";
    print_md5(res);
    std::cout << std::endl;

    return signature == res ? 0 : 1;
}

int sign(std::shared_ptr<HashFunction> hash, std::shared_ptr<Encoder> encoder,
         std::string filename, std::string sfilename)
{
    std::ifstream fin;
    std::ofstream fout;
    fin.open(filename, std::ios::binary | std::ios::in);
    fout.open(sfilename, std::ios::binary | std::ios::out);

    std::stringstream buffer;
    buffer << fin.rdbuf();
    fin.close();

    std::string res = hash->apply(buffer.str());
    print_md5(res);
    std::cout << std::endl;
    res = encoder->encode(res);

    fout.write(res.data(), res.length());
    fout.close();

    return 0;
}

int main(int argc, char **argv)
{
    if (4 != argc)
        return 1;

    using Builder = std::shared_ptr<Encoder> (*)(std::string);
    using Method = int (*)(std::shared_ptr<HashFunction>, std::shared_ptr<Encoder>, std::string, std::string);
    std::string config = "public.json";
    Builder builder = read_rsa_ds_decoder;
    Method method = verify;

    std::string op = argv[1];

    if ("sign" == op)
    {
        config = "private.json";
        builder = read_rsa_ds_encoder;
        method = sign;
    }
    else if ("verify" != op)
        return 2;

    std::shared_ptr<HashFunction> hash = std::make_shared<MD5>();
    std::shared_ptr<Encoder> encoder = builder(config);

    return method(hash, encoder, argv[2], argv[3]);

    // std::string str, enc, dec;
    //  
    // std::ifstream fin;
    // fin.open(argv[1], std::ios::binary | std::ios::in);
    //  
    // std::stringstream buffer;
    // buffer << fin.rdbuf();
    // fin.close();

    // std::string res = hash->apply(buffer.str());

    // for (size_t i = 0; 16 > i; i++)
    //     std::cout << std::hex << std::setw(2) << std::setfill('0')
    //               << (int)((unsigned char)res[i]);
    //  
    // std::cout << std::endl;
}

