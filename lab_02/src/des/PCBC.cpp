#include "PCBC.h"

#include <string.h>

PCBC::PCBC(std::list<std::shared_ptr<DES>> des,
           const char *const initial_value)
    : des(des)
{
    if (nullptr == initial_value)
        throw;

    for (auto ptr : des)
        if (nullptr == ptr)
            throw;

    memmove(this->initial_value, initial_value, 8);
}

std::string PCBC::encode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    size_t rest = length % 8;

    if (0 != rest)
        length += 8 - rest;

    std::string out, cur_str;
    char cary_buf[8], tmp_buf[8];
    out.reserve(length);
    cur_str.reserve(8);
    cur_str.resize(8);

    memmove(cary_buf, this->initial_value, 8);

    BitRange cary (cary_buf, 0, 64);
    BitRange tmp (tmp_buf, 0, 64);
    BitRange current (cur_str.data(), 0, 64);
    auto iter = this->des.begin();

    for (size_t i = 0; length > i; i += 8, ++iter)
    {
        if (this->des.end() == iter)
            iter = this->des.begin();

        std::string c = origin.substr(i, 8);
        c.copy(cur_str.data(), c.length());

        if (8 != c.length())
            for (size_t i = c.length(); 8 > i; cur_str[i++] = 0);

        memmove(tmp_buf, cur_str.data(), 8);

        current ^= cary;
        std::string enc_str = (*iter)->encode(cur_str);

        BitRange enc_rng (enc_str.data(), 0, 64);
        tmp ^= enc_rng;

        memmove(cary_buf, tmp_buf, 8);

        out += enc_str;
    }

    return out;
}

std::string PCBC::decode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    if (0 != length % 8)
        throw;

    std::string out, cur_str(8, 0);
    char cary_buf[8], tmp_buf[8];
    out.reserve(length);

    memmove(cary_buf, this->initial_value, 8);

    BitRange cary (cary_buf, 0, 64);
    BitRange tmp (tmp_buf, 0, 64);
    BitRange current (cur_str.data(), 0, 64);
    auto iter = this->des.begin();

    for (size_t i = 0; origin.length() > i; i += 8, ++iter)
    {
        if (this->des.end() == iter)
            iter = this->des.begin();

        cur_str = origin.substr(i, 8);
        memmove(tmp_buf, cur_str.data(), 8);

        std::string enc_str = (*iter)->decode(cur_str);
        BitRange enc_rng (enc_str.data(), 0, 64);

        enc_rng ^= cary;
        tmp ^= enc_rng;

        memmove(cary_buf, tmp_buf, 8);

        out += enc_str;
    }

    return out;
}

