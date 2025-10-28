#include <iostream>
#include <cstring>
#include <string>
#include "include.h"

void ShowHelp()
{
    std::cout << "Usage: \n"
              << "memfill.exe <size in MB/GB> <MB/GB>\n"
              << std::endl;
    std::cout << "Example: \n"
              << "memfill.exe 1 GB" << std::endl;
    std::cout << "memfill.exe 100 MB" << std::endl;
    std::cout << "* size in MB/GB must be a positive integer *" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        ShowHelp();
        return 1;
    }

    long long unsigned size = 0;
    char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        bool is_digit = false;
        for (int j = 0; j < 10; j++)
        {
            if (argv[1][i] == numbers[j])
            {
                is_digit = true;
                break;
            }
        }
        if (!is_digit)
        {
            ShowHelp();
            return 1;
        }
    }
    size = std::stoi(argv[1]);

    int unit = 0;
    if (std::string(argv[2]) == "MB")
    {
        unit = MB;
    }
    else if (std::string(argv[2]) == "GB")
    {
        unit = GB;
    }
    else
    {
        ShowHelp();
        return 1;
    }

    size_t final_size = size * unit;
    std::vector<char> largeMemory(final_size);

    std::fill(largeMemory.begin(), largeMemory.end(), 0);

    std::cout << "Press Enter to free the memory and exit..." << std::endl;
    getchar();

    return 0;
}