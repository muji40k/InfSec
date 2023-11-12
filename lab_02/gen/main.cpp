#include <fstream>
#include <vector>
#include <stdint.h>
#include <time.h>

int main(int argc, char **argv)
{
    std::srand(time(nullptr));
    size_t amount, limit;

    std::ofstream stream (argv[1], std::ios::binary | std::ios::out);

    if (3 == argc)
        limit = amount = std::stoull(argv[2]);
    else if (4 == argc)
    {
        amount = std::stoull(argv[2]);
        limit = std::stoull(argv[3]);
    }
    else
        return 1;

    std::vector visited (limit, false);

    for (size_t i = 0, val, start; amount > i; i++)
    {
        start = val = ((double)std::rand() / RAND_MAX) * limit;

        if (visited[val])
        {
            val = (val + 1) % limit;
            for (; start != val && visited[val]; val = (val + 1) % limit);

            if (start == val)
                for (size_t i = 0; limit > i; i++)
                    visited[i] = false;
        }

        stream.write((char *)&val, sizeof(size_t));
        visited[val] = true;
    }

    stream.close();

    return 0;
}

