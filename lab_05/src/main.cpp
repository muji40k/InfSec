#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>

#include "LZW.h"

int main(int argc, char **argv)
{
    if (4 != argc)
        return 1;

    using Method = std::string (Archiver::*)(const std::string &);
    Method method = &Archiver::compress;
    std::string op = argv[1];

    if ("decompress" == op)
        method = &Archiver::decompress;
    else if ("compress" != op)
        return 2;

    std::shared_ptr<Archiver> archiver = std::make_shared<LZW>();
    std::string str, enc, dec;

    std::ifstream fin;
    std::ofstream fout;
    fin.open(argv[2], std::ios::binary | std::ios::in);
    fout.open(argv[3], std::ios::binary | std::ios::out);

    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string instr = buffer.str();

    std::string res = ((archiver.get())->*method)(instr);
    fout.write(res.c_str(), res.length());

    if ("compress" == op)
    {
        if (0 == instr.length())
            std::cout << "Compression: empty" << std::endl;
        else
            std::cout << "Compression: "
                      << (double)res.length() / instr.length() * 100 << '%'
                      << std::endl;
    }

    fin.close();
    fout.close();

    return 0;
}

