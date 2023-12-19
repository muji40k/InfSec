#include "LZW.h"

#include <stdexcept>
#include <unordered_map>
#include <vector>

class WTable
{
    public:
        bool add(std::string seq)
        {
            if (1 == seq.size() || this->limit == this->last)
                return false;

            auto iter = this->map.find(seq);

            if (this->map.end() != iter)
                return false;

            this->map.emplace(seq, this->last++);
            return true;
        }

        bool present(std::string seq)
        {
            if (1 == seq.size())
                return true;

            auto iter = this->map.find(seq);

            if (this->map.end() == iter)
                return false;

            return true;
        }

        unsigned short get(std::string seq)
        {
            if (1 == seq.size())
                return (unsigned char)seq[0];

            auto iter = this->map.find(seq);

            if (this->map.end() == iter)
                throw std::runtime_error("Value for sequence not found");

            return (*iter).second;
        }

    private:
        static constexpr const unsigned short limit = 4096;
        unsigned short last = 256;
        std::unordered_map<std::string, unsigned short> map;
};

class BufferWriter
{
    public:
        BufferWriter(std::string &out) : out(out), half(0)
        {
            for (size_t i = 0; 3 > i; this->buffer[i++] = 0);
        }

        void add(unsigned short value)
        {
            value &= 4095;

            switch (this->half)
            {
                case 0:
                    this->buffer[0] = value & 255;
                    this->buffer[1] = (value & 3840) >> 8;
                    break;
                case 1:
                    this->buffer[1] |= (value & 15) << 4;
                    this->buffer[2] = (value & 4080) >> 4;
                    this->full_flush();
                    break;
            }

            this->half = (this->half + 1) % 2;
        }

        void flush(void)
        {
            if (0 == this->half)
                return;

            this->out += std::string((char *)this->buffer, 2);

            for (size_t i = 0; 3 > i; this->buffer[i++] = 0);
        }

    private:
        void full_flush(void)
        {
            this->out += std::string((char *)this->buffer, 3);

            for (size_t i = 0; 3 > i; this->buffer[i++] = 0);
        }

    private:
        std::string &out;
        int half;
        unsigned char buffer[3];
};

std::string LZW::compress(const std::string &origin)
{
    if (origin.empty())
        return origin;

    std::string out;
    WTable table;
    BufferWriter writer(out);
    std::string sequence = "", last = "";

    for (char c : origin)
    {
        last = sequence;
        sequence += c;

        if (!table.present(sequence))
        {
            writer.add(table.get(last));
            table.add(sequence);
            sequence = c, last = "";
        }
    }

    writer.add(table.get(sequence));
    writer.flush();

    return out;
}

class RTable
{
    public:
        RTable(void) : map(RTable::limit - 256) {}

        bool add(std::string seq)
        {
            if (1 == seq.size() || this->limit == this->last)
                return false;

            this->map[(this->last++) - 256] = seq;

            return true;
        }

        bool present(unsigned short value)
        {
            if (this->last > value)
                return true;

            return false;
        }

        std::string get(unsigned short value)
        {
            if (256 > value)
                return std::string(1, char(value));

            if (!this->present(value))
                throw std::runtime_error("Sequence for value not found");

            return this->map[value - 256];
        }

    private:
        static constexpr const unsigned short limit = 4096;
        unsigned short last = 256;
        std::vector<std::string> map;
};

class BufferReader
{
    public:
        BufferReader(const std::string &in)
            : iter(in.begin()), end(in.end()), half(0)
        {
            if (1 == in.size() % 3)
                throw std::logic_error("Incorrect compressed data");
        }

        bool rest(void) const
        {
            return this->end != this->iter;
        }

        unsigned short read(void)
        {
            unsigned short out = 0;

            switch (this->half)
            {
                case 0:
                    out = (unsigned char)*iter;
                    this->last = (unsigned char)*(++iter);
                    out |= (this->last & 15) << 8;
                    break;
                case 1:
                    out = (this->last & 240) >> 4;
                    unsigned short tmp = (unsigned char)*iter;
                    out |= tmp << 4;
                    break;
            }

            ++iter;
            this->half = (this->half + 1) % 2;

            return out;
        }


    private:
        std::string::const_iterator iter, end;
        int half;
        unsigned short last;
};

std::string LZW::decompress(const std::string &origin)
{
    if (origin.empty())
        return origin;

    std::string out;
    RTable table;
    BufferReader reader(origin);
    unsigned short code = reader.read();
    std::string previous, current;

    previous = table.get(code);
    out += previous;

    while (reader.rest())
    {
        code = reader.read();

        if (table.present(code))
        {
            current = table.get(code);
            table.add(previous + current[0]);
        }
        else // Handle mirror sequence c.*c.*c (skip of index)
        {
            current = previous + previous[0];
            table.add(current);
        }

        out += current;
        previous = current;
    }

    return out;
}

