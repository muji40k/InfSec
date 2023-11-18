#include <fstream>
#include <vector>
#include <stdint.h>
#include <time.h>

int main(int argc, char **argv)
{
    std::srand(time(nullptr));
    size_t amount;
    char item;

    std::ofstream stream (argv[1], std::ios::binary | std::ios::out);

    if (3 == argc)
        amount = std::stoull(argv[2]);
    else
        return 1;

    for (size_t i = 0; amount > i; i++)
    {
        item = std::rand() % 256;
        stream.write((char *)&item, sizeof(char));
    }

    stream.close();

    return 0;
}

