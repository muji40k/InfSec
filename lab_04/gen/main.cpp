#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

static constexpr const unsigned long long min = 268435456ull;
static constexpr const unsigned long long max = 4294967295ull;
static constexpr const unsigned long long rng = max - min;

bool prime(unsigned long long num)
{
    if (0 == num % 2)
        return false;

    const unsigned long long root = sqrt(num);

    for (unsigned long long i = 3; root >= i; i += 2)
        if (0 == num % i)
            return false;

    return true;
}

unsigned long long get_rand_prime(void)
{
    unsigned long long val = std::rand() % rng;

    for (; !prime(min + val); val = (val + 1) % rng);

    return min + val;
}

struct ext_long_long
{
    unsigned long long num;
    char sign;
};

struct ext_long_long ell_minus(const struct ext_long_long a, const struct ext_long_long b)
{
    if (a.sign == b.sign)
    {
        if (a.num > b.num)
            return {a.num - b.num, a.sign};

        return {b.num - a.num, (char)((a.sign + 1) % 2)};
    }

    return {a.num + b.num, a.sign};
}

struct ext_long_long ell_min(const struct ext_long_long a, const struct ext_long_long b)
{
    if (a.sign != b.sign)
    {
        if (1 == a.sign)
            return a;

        return b;
    }

    if (1 == a.sign)
    {
        if (a.num > b.num)
            return a;

        return b;
    }

    if (a.num > b.num)
        return b;

    return a;
}

unsigned long long get_invert(unsigned long long e, unsigned long long phi)
{
    unsigned long long a = phi;
    unsigned long long b = e;

    unsigned long long q = 0, r = 0;

    struct ext_long_long x, y,
                         x1 = {0, 0}, x2 = {1, 0},
                         y1 = {1, 0}, y2 = {0, 0};

    while (0 < b)
    {
        q = a / b;
        r = a - q * b;

        x = {x1.num * q, x1.sign};
        y = {y1.num * q, y1.sign};
        x = ell_minus(x2, x);
        y = ell_minus(y2, y);

        a = b;
        b = r;

        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }

    if (1 < a)
        return 0;

    if (y2.sign)
        return phi - y2.num;

    return y2.num;
}

unsigned long long get_e(const unsigned long long phi)
{
    unsigned long long e = (2 << 3);

    for (; phi > e && (!prime(e + 1) || 0 == phi % (e + 1)); e <<= 1);

    return e + 1;
}

int main(int argc, char **argv)
{
    if (2 != argc)
        return 1;

    std::srand(std::time(nullptr));

    unsigned long long p = get_rand_prime(), q = get_rand_prime();
    unsigned long long n = p * q, phi = (p - 1) * (q - 1);
    unsigned long long e = get_e(phi);
    unsigned long long d = get_invert(e, phi);

    if (0 == d)
    {
        std::cout << "Ошибка" << std::endl;
        return 0;
    }

    std::ofstream stream (argv[1], std::ios::out);
    stream << "{\"e\": " << e << ", \"d\": " << d << ", \"n\": " << n << "}\n";
    stream.close();

    return 0;
}

