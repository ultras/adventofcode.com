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


void iterateBinaryIn(unsigned int count, ::std::function<void(const ::std::vector<bool> &)> callback)
{
    ::std::vector<bool> in(count, false);

    for (;;)
    {
        callback(in);

        for (unsigned int i = 0; i <= count; ++i)
        {
            if (count == i)
            {
                return;
            }

            in[i] = !in[i];
            if (false != in[i])
            {
                break;
            }
        }
    }
}



int _tmain(int argc, _TCHAR * argv[])
{
    ::std::vector<int> containers;
    ::std::map<int, int> containerCountWays;

    ::std::ifstream infile("../Day17/input.txt");
    ::std::string line;
    while (::std::getline(infile, line))
    {
        containers.push_back(::std::atoi(line.c_str()));
    }

    unsigned int fit150 = 0;
    iterateBinaryIn(containers.size(),
        [&containers, &fit150, &containerCountWays](const ::std::vector<bool> & in)
        {
            int sum = 0;
            int containerCount = 0;
            for (unsigned int i = 0; i < containers.size(); ++i)
            {
                if (in[i])
                {
                    ++containerCount;
                    sum += containers[i];
                }
            }
            if (150 == sum)
            {
                ++fit150;
                auto containerCountIter = containerCountWays.find(containerCount);
                if (containerCountIter == containerCountWays.end())
                {
                    containerCountWays.emplace(::std::make_pair(containerCount, 1)).first;
                }
                else
                {
                    ++containerCountIter->second;
                }
            }
        });

    ::std::cout << fit150 << ::std::endl;
    ::std::cout << containerCountWays.cbegin()->first << ' ' << containerCountWays.cbegin()->second << ::std::endl;
    ::std::cout << containerCountWays.crbegin()->first << ' ' << containerCountWays.crbegin()->second << ::std::endl;

    return 0;
}
