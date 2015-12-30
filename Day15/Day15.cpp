#include <SDKDDKVer.h>
#include <tchar.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <functional>
#include <algorithm>
#include <cstdint>
#include <cassert>


typedef ::std::vector<::std::string> Tokens;
void tokenize(const ::std::string & line, Tokens & tokens, const char delimiter = ' ')
{
    tokens.clear();
    ::std::string::size_type start = 0;
    for (;;)
    {
        auto pos = line.find(delimiter, start);
        if (::std::string::npos != pos)
        {
            tokens.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }
        else
        {
            tokens.push_back(line.substr(start));
            break;
        }
    }
}



struct Teaspoon
{
    //const int capacity;
    //const int durability;
    //const int flavor;
    //const int texture;
    //const int calories;
    const ::std::vector<int> values;

    Teaspoon(int capacity_, int durability_, int flavor_, int texture_, int calories_)
        //: capacity(capacity_)
        //, durability(durability_)
        //, flavor(flavor_)
        //, texture(texture_)
        //, calories(calories_)
        : values({ capacity_, durability_, flavor_, texture_, calories_ })
    {
    }
};
typedef ::std::vector<Teaspoon> Teaspoons;


void iterateUpValue(int value, int count, ::std::function<void(const ::std::vector<int> &)> callback)
{
    ::std::vector<int> items(count, 0);
    
    for (;;)
    {
        const auto sum = ::std::accumulate(items.cbegin(), --items.cend(), 0);
        auto rest = value - sum;
        if (0 <= rest)
        {
            *items.rbegin() = rest;
            callback(items);
        }

        for (int i = 0; i < count; ++i)
        {
            if (i == count - 1)
            {
                return;
            }

            if (value < ++items[i])
            {
                items[i] = 0;
            }
            else
            {
                break;
            }
        }
    }
}


int _tmain(int argc, _TCHAR * argv[])
{
    Teaspoons teaspoons;

    ::std::ifstream infile("../Day15/input.txt");
    ::std::string line;
    Tokens tokens;
    while (::std::getline(infile, line))
    {
        tokenize(line, tokens);
        teaspoons.push_back(Teaspoon(::std::atoi(tokens[2].c_str()),
                                     ::std::atoi(tokens[4].c_str()),
                                     ::std::atoi(tokens[6].c_str()),
                                     ::std::atoi(tokens[8].c_str()),
                                     ::std::atoi(tokens[10].c_str())));
    }

    {
        long int maxValue = 0;
        int count = teaspoons.size();
        iterateUpValue(100, count,
            [&teaspoons, &maxValue, count](const ::std::vector<int> & items)
        {
            long int result = 1;
            for (int i = 0; i < 4; ++i)
            {
                int sum = 0;
                for (int j = 0; j < count; ++j)
                {
                    sum += items[j] * teaspoons[j].values[i];
                }
                sum = ::std::max(sum, 0);
                result *= sum;
            }

            maxValue = ::std::max(maxValue, result);
        });
        ::std::cout << "max: " << maxValue << ::std::endl;
    }

    {
        long int maxValue = 0;
        int count = teaspoons.size();
        iterateUpValue(100, count,
            [&teaspoons, &maxValue, count](const ::std::vector<int> & items)
        {
            long int result = 1;
            for (int i = 4; 0 <= i; --i)
            {
                int sum = 0;
                for (int j = 0; j < count; ++j)
                {
                    sum += items[j] * teaspoons[j].values[i];
                }

                if (4 == i)
                {
                    if (500 != sum)
                    {
                        return;
                    }
                    continue;
                }

                sum = ::std::max(sum, 0);
                result *= sum;
            }

            maxValue = ::std::max(maxValue, result);
        });
        ::std::cout << "max: " << maxValue << ::std::endl;
    }

    return 0;
}
