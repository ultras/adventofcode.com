#include <SDKDDKVer.h>
#include <tchar.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <cassert>


typedef ::std::vector<::std::string> Tokens;

struct ReindeerData
{
    const int speed;
    const int speedTime;
    const int pause;

    ReindeerData(int speed_, int speedTime_, int pause_)
        : speed(speed_)
        , speedTime(speedTime_)
        , pause(pause_)
    {
    }
};

typedef ::std::vector<ReindeerData> ReindeerDatas;



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


int distance(int duration, int speed, int speedTime, int pause)
{
    int distance = 0;
    while (0 < duration)
    {
        distance += ::std::min(speedTime, duration) * speed;
        duration -= speedTime;
        duration -= pause;
    }
    return distance;
}

int distance(int duration, const ReindeerData & data)
{
    return distance(duration, data.speed, data.speedTime, data.pause);
}


int _tmain(int argc, _TCHAR * argv[])
{
    assert(1120 == distance(1000, 14, 10, 127));
    assert(1056 == distance(1000, 16, 11, 162));

    {
        ::std::ifstream infile("../Day14/input.txt");
        ::std::string line;
        Tokens tokens;
        int maxResult = 0;
        while (::std::getline(infile, line))
        {
            tokenize(line, tokens);
            const auto speed = ::std::atoi(tokens[3].c_str());
            const auto speedTime = ::std::atoi(tokens[6].c_str());
            const auto pause = ::std::atoi(tokens[13].c_str());
            const auto result = distance(2503, speed, speedTime, pause);
            maxResult = ::std::max(maxResult, result);

            ::std::cout << result << ::std::endl;
        }
        ::std::cout << "max: " << maxResult << ::std::endl;
    }

    {
        Tokens tokens;
        ReindeerDatas reindeerDatas;

        ::std::ifstream infile("../Day14/input.txt");
        ::std::string line;       
        while (::std::getline(infile, line))
        {
            tokenize(line, tokens);

            reindeerDatas.push_back(ReindeerData(::std::atoi(tokens[3].c_str()),
                                                 ::std::atoi(tokens[6].c_str()),
                                                 ::std::atoi(tokens[13].c_str())));
        }

        const auto count = reindeerDatas.size();
        ::std::vector<int> results(count, 0);
        ::std::vector<int> points(count, 0);

        for (unsigned int i = 1; i <= 2503; ++i)
        {
            int maxResult = 0;
            for (unsigned int j = 0; j < count; ++j)
            {
                const auto & data = reindeerDatas[j];
                results[j] = distance(i, data);
                maxResult = ::std::max(maxResult, results[j]);
            }

            for (unsigned int j = 0; j < count; ++j)
            {
                if (maxResult == results[j])
                {
                    ++points[j];
                }
            }
        }

        int maxResult = 0;
        for (auto point : points)
        {
            maxResult = ::std::max(maxResult, point);
            ::std::cout << point << ::std::endl;
        }
        ::std::cout << "max: " << maxResult << ::std::endl;
    }

    return 0;
}
