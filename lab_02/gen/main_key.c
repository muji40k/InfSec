#include <fstream>
#include <vector>
#include <stdint.h>
#include <time.h>

int main(int argc, char **argv)
{
    std::srand(time(nullptr));
    char key[8];
    const unsigned char max = 1 << 7;

    if (2 != argc)
        return 1;

    for (unsigned char i = 0, cnt, val, start; 8 > i; i++)
    {
        val = start = std::rand() % max;
        cnt = 0;

        for (size_t i = 0; 7 > i; i++, start >>= 1)
            if (start & 1)
                cnt++;

        if (cnt % 2)
            val |= max;
        else
            val &= ~max;

        key[i] = val;
    }

    std::ofstream stream (argv[1], std::ios::binary | std::ios::out);
    stream.write(key, 8);
    stream.close();

    return 0;
}
