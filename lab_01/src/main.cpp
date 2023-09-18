#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <climits>
#include <filesystem>

#include "Encoder.h"
#include "Enigma.h"
#include "PlainRotor.h"
#include "PlainReflector.h"
#include "PlainIO.h"
#include "LogRotor.h"
#include "LogReflector.h"
#include "LogIO.h"
#include "RotateRotorDecorator.h"

#include "CharMapMap.h"
#include "CharAlphabet.h"

static const std::shared_ptr<Alphabet<char>> alphabet = std::make_shared<CharAlphabet>();

std::list<std::pair<size_t, size_t>> getMap(void)
{
    const size_t size = alphabet->size();
    std::list<std::pair<size_t, size_t>> out;
    std::vector<bool> visited (size, false);

    for (size_t i = 0, j = 0; size > i; i++)
    {
        j = std::rand() % size;

        while (visited[j])
            j = (j + 1) % size;

        visited[j] = true;
        out.push_back(std::make_pair(i, j));
    }

    return out;
}

std::list<std::pair<size_t, size_t>> getReflector(void)
{
    const size_t size = alphabet->size();
    std::list<std::pair<size_t, size_t>> out;
    std::vector<bool> visited (size, false);

    for (size_t i = 0, j = 0; size > i; i++)
        if (!visited[i])
        {
            visited[i] = true;
            j = std::rand() % size;

            while (visited[j])
                j = (j + 1) % size;

            visited[j] = true;
            out.push_back(std::make_pair(i, j));
            out.push_back(std::make_pair(j, i));
        }

    return out;
}

int generateMap(const char *filename)
{
    if (nullptr == filename)
        return -1;

    std::list<std::pair<size_t, size_t>> map = getMap();
    char c;

    std::ofstream fout;
    fout.open(filename, std::ios::binary | std::ios::out);

    for (auto pair : map)
    {
        c = alphabet->letter(pair.first);
        fout.write(&c, sizeof(char));
        c = alphabet->letter(pair.second);
        fout.write(&c, sizeof(char));
    }

    fout.close();

    return 0;
}

int generateReflector(const char *filename)
{
    if (nullptr == filename)
        return -1;

    std::list<std::pair<size_t, size_t>> map = getReflector();
    char c;

    std::ofstream fout;
    fout.open(filename, std::ios::binary | std::ios::out);

    for (auto pair : map)
    {
        c = alphabet->letter(pair.first);
        fout.write(&c, sizeof(char));
        c = alphabet->letter(pair.second);
        fout.write(&c, sizeof(char));
    }

    fout.close();

    return 0;
}

std::shared_ptr<CharMap> readMap(const char *filename)
{
    if (nullptr == filename)
        throw;

    std::list<std::pair<size_t, size_t>> map;
    char c;

    std::ifstream fin;
    fin.open(filename, std::ios::binary | std::ios::in);

    while (!fin.eof())
    {
        std::pair<size_t, size_t> pair;
        fin.read(&c, sizeof(char));
        pair.first = alphabet->order(c);
        fin.read(&c, sizeof(char));
        pair.second = alphabet->order(c);
        map.push_back(pair);
    }

    fin.close();

    return std::make_shared<CharMapMap>(map);
}

std::shared_ptr<EnigmaRotor> getRotor(std::shared_ptr<CharMap> map)
{
    return std::make_shared<PlainRotor>(map);
}

std::shared_ptr<RotateRotorDecorator> getWrap(std::list<std::shared_ptr<EnigmaRotor>>::iterator iter)
{
    return std::make_shared<RotateRotorDecorator>(*iter);
}

std::shared_ptr<Encoder> readEnigma(bool silent = false)
{
    std::shared_ptr<CharMap> io_map = readMap("config/plugboard");

    std::shared_ptr<EnigmaIO> io = std::make_shared<PlainIO>(io_map);

    if (!silent)
        io = std::make_shared<LogIO>("plugboard", io, alphabet);

    std::shared_ptr<CharMap> reflector_map = readMap("config/reflector");

    std::shared_ptr<EnigmaReflector> reflector = std::make_shared<PlainReflector>(reflector_map);

    if (!silent)
        reflector = std::make_shared<LogReflector>("ref", reflector, alphabet);

    std::list<std::shared_ptr<EnigmaRotor>> rotors;

    for (const auto &entry : std::filesystem::directory_iterator("config/rotors"))
    {
        std::shared_ptr<CharMap> map = readMap(entry.path().c_str());
        rotors.push_back(getRotor(map));
    }

    auto iter = rotors.begin();
    std::shared_ptr<RotateRotorDecorator> p = getWrap(iter++);
    std::list<std::shared_ptr<EnigmaRotor>> rotors_wrapped;

    std::shared_ptr<EnigmaRotor> w = p;

    if (!silent)
        w = std::make_shared<LogRotor>("1", p, alphabet);

    rotors_wrapped.push_back(w);

    for (size_t i = 2; rotors.end() != iter; ++i, ++iter)
    {
        std::shared_ptr<RotateRotorDecorator> c = getWrap(iter);
        c->stack(p);
        w = c;

        if (!silent)
            w = std::make_shared<LogRotor>(std::to_string(i), c, alphabet);

        rotors_wrapped.push_back(w);
        p = c;
    }

    return std::make_shared<Enigma>(alphabet, io, rotors_wrapped, reflector);
}

int enigma(const char *const infile, const char *const outfile, bool silent=false)
{
    std::shared_ptr<Encoder> encoder = readEnigma(silent);

    char buf[100];
    std::string str, res;

    std::ifstream fin;
    std::ofstream fout;
    fin.open(infile, std::ios::binary | std::ios::in);
    fout.open(outfile, std::ios::binary | std::ios::out);

    while (!fin.eof())
    {
        fin.read(buf, sizeof(buf));
        str.assign(buf, fin.gcount());
        res = encoder->encode(str);
        fout.write(res.c_str(), fin.gcount());
    }

    fin.close();
    fout.close();

    return 0;
}

int generator(char **files, const size_t size, int (*creator)(const char *const))
{
    int rc = 0;

    for (size_t i = 0; 0 == rc && size > i; i++)
        rc = creator(files[i]);

    return rc;
}

int main(int argc, char **argv)
{
    if (1 == argc)
        return 1;

    std::srand(std::time(nullptr));
    std::string arg;
    arg.assign(argv[1]);

    if ("-gmap" == arg)
        return generator(argv + 2, argc - 2, generateMap);

    if ("-gref" == arg)
        return generator(argv + 2, argc - 2, generateReflector);

    if (4 == argc && "--silent" == arg)
        return enigma(argv[2], argv[3], true);

    if (3 == argc)
        return enigma(argv[1], argv[2], false);

    return 1;
}

