#include "CFB.h"

#include <cstring>

#include "BitRange.h"

CFB::CFB(std::list<std::shared_ptr<Encoder>> encoders,
         std::vector<size_t> offsets, std::string initial_value,
         const size_t block_size)
    : encoders(encoders), offsets(offsets), initial_value(initial_value),
      block_size(block_size)
{
    if (initial_value.length() != block_size)
        throw;

    if (encoders.size() != offsets.size())
        throw;

    for (auto ptr : encoders)
        if (nullptr == ptr)
            throw;
}

std::string CFB::encode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    size_t rest = length % this->block_size;

    if (0 != rest)
        length += this->block_size - rest;

    std::string out, cur_msg_str, cur_str;
    out.reserve(length);
    cur_str.reserve(this->block_size);
    cur_str.resize(this->block_size);
    cur_msg_str.reserve(this->block_size);
    cur_msg_str.resize(this->block_size);

    this->initial_value.copy(cur_str.data(), this->block_size);

    char tmp_buf[this->block_size];
    BitRange current_msg (cur_msg_str.data(), 0, this->block_size * 8);
    BitRange current (cur_str.data(), 0, this->block_size * 8);
    BitRange tmp (tmp_buf, 0, this->block_size * 8);

    auto iter = this->encoders.begin();
    auto oiter = this->offsets.begin();

    for (size_t i = 0; length > i; i += this->block_size, ++iter, ++oiter)
    {
        if (this->encoders.end() == iter)
        {
            iter = this->encoders.begin();
            oiter = this->offsets.begin();
        }

        std::string c = origin.substr(i, this->block_size);
        c.copy(cur_msg_str.data(), c.length());

        if (this->block_size != c.length())
            for (size_t i = c.length(); this->block_size > i; cur_msg_str[i++] = 0);

        std::string enc_str = (*iter)->encode(cur_str);
        enc_str.copy(cur_str.data(), this->block_size);

        current ^= current_msg;
        out += cur_str;

        for (Bit bit : tmp)
        {
            size_t index = bit.index();

            if (*oiter > index)
                index = this->block_size * 8 - *oiter + index;
            else
                index -= *oiter;

            bit = current[index];
        }

        current.copy(tmp);
    }

    return out;
}

std::string CFB::decode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    if (0 != length % this->block_size)
        throw;

    std::string out, cur_msg_str, cur_str;
    out.reserve(length);
    cur_str.reserve(this->block_size);
    cur_str.resize(this->block_size);
    cur_msg_str.reserve(this->block_size);
    cur_msg_str.resize(this->block_size);

    this->initial_value.copy(cur_str.data(), this->block_size);

    BitRange current_msg (cur_msg_str.data(), 0, this->block_size * 8);
    BitRange current (cur_str.data(), 0, this->block_size * 8);

    auto iter = this->encoders.begin();
    auto oiter = this->offsets.begin();

    for (size_t i = 0; length > i; i += this->block_size, ++iter, ++oiter)
    {
        if (this->encoders.end() == iter)
        {
            iter = this->encoders.begin();
            oiter = this->offsets.begin();
        }

        std::string c = origin.substr(i, this->block_size);
        c.copy(cur_msg_str.data(), c.length());

        if (this->block_size != c.length())
            for (size_t i = c.length(); this->block_size > i; cur_msg_str[i++] = 0);

        std::string enc_str = (*iter)->encode(cur_str);
        enc_str.copy(cur_str.data(), this->block_size);

        current ^= current_msg;
        out += cur_str;

        for (Bit bit : current)
        {
            size_t index = bit.index();

            if (*oiter > index)
                index = this->block_size * 8 - *oiter + index;
            else
                index -= *oiter;

            bit = current_msg[index];
        }
    }

    return out;
}

