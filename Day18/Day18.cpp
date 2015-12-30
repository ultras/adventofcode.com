#include <SDKDDKVer.h>
#include <tchar.h>

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <functional>
#include <algorithm>
#include <cstdint>
#include <cassert>


const unsigned int c_size = 100;
const unsigned int c_steps = 100;

unsigned int countNeighbors(unsigned int x, unsigned int y, bool field[c_size + 2][c_size + 2])
{
    unsigned int result = 0;
    for (unsigned int i = x - 1; i <= x + 1; ++i)
    {
        for (unsigned int j = y - 1; j <= y + 1; ++j)
        {
            if (field[i][j])
            {
                ++result;
            }
        }
    }
    if (field[x][y])
    {
        --result;
    }
    return result;
}


void iterate(bool field[c_size + 2][c_size + 2], bool result[c_size + 2][c_size + 2])
{
    for (unsigned int i = 1; i <= c_size; ++i)
    {
        for (unsigned int j = 1; j <= c_size; ++j)
        {
            const auto neighbors = countNeighbors(i, j, field);
            if (field[i][j])
            {
                result[i][j] = (2 == neighbors) || (3 == neighbors);
            }
            else
            {
                result[i][j] = (3 == neighbors);
            }
        }
    }
}

void print(bool field[c_size + 2][c_size + 2])
{
    for (unsigned int i = 1; i <= c_size; ++i)
    {
        for (unsigned int j = 1; j <= c_size; ++j)
        {
            if (field[i][j])
            {
                ::std::cout << '#';
            }
            else
            {
                ::std::cout << '.';
            }
        }
        ::std::cout << ::std::endl;
    }
    ::std::cout << ::std::endl;
}


int _tmain(int argc, _TCHAR * argv[])
{   
    bool field[c_size + 2][c_size + 2];
    bool field2[c_size + 2][c_size + 2];
    bool fieldStep2[c_size + 2][c_size + 2];
    bool field2Step2[c_size + 2][c_size + 2];
    for (unsigned int i = 0; i < c_size + 2; ++i)
    {
        for (unsigned int j = 0; j < c_size + 2; ++j)
        {
            field[i][j] = field2[i][j] = fieldStep2[i][j] = field2Step2[i][j] = false;
        }
    }

    ::std::ifstream infile("../Day18/input.txt");
    ::std::string line;
    {
        unsigned int i = 1;
        while (::std::getline(infile, line))
        {
            for (unsigned int j = 0; j < c_size; ++j)
            {
                field[i][j + 1] = fieldStep2[i][j + 1] = ('#' == line[j]);
            }
            ++i;
        }
    }

    {
        for (unsigned int i = 0; i < c_steps; ++i)
        {
            if (0 == (i % 2))
            {
                iterate(field, field2);
                //print(field2);
            }
            else
            {
                iterate(field2, field);
                //print(field);
            }
        }
        print(field);

        unsigned int count = 0;
        for (unsigned int i = 1; i <= c_size; ++i)
        {
            for (unsigned int j = 1; j <= c_size; ++j)
            {
                if (field[i][j])
                {
                    ++count;
                }
            }
        }
        ::std::cout << count << ::std::endl;
    }

    {
        for (unsigned int i = 0; i < c_steps; ++i)
        {
            if (0 == (i % 2))
            {
                iterate(fieldStep2, field2Step2);
                field2Step2[1][1] = field2Step2[c_size][1] = field2Step2[1][c_size] = field2Step2[c_size][c_size] = true;
            }
            else
            {
                iterate(field2Step2, fieldStep2);
                fieldStep2[1][1] = fieldStep2[c_size][1] = fieldStep2[1][c_size] = fieldStep2[c_size][c_size] = true;
            }
        }
        print(fieldStep2);

        unsigned int count = 0;
        for (unsigned int i = 1; i <= c_size; ++i)
        {
            for (unsigned int j = 1; j <= c_size; ++j)
            {
                if (fieldStep2[i][j])
                {
                    ++count;
                }
            }
        }
        ::std::cout << count << ::std::endl;
    }


    return 0;
}
