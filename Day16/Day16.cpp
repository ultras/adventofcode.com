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


typedef ::std::map<::std::string, int> Properties;

bool isSubSet(const Properties & set, const Properties & sub)
{
    for (const auto & iter : sub)
    {
        const auto setValue = set.find(iter.first)->second;
        if (setValue != iter.second)
        {
            return false;
        }
    }

    return true;
}

bool isSubSet2(const Properties & set, const Properties & sub)
{
    for (const auto & iter : sub)
    {
        const auto & key = iter.first;
        const auto setValue = set.find(key)->second;
        const auto value = iter.second;
        if ("cats" == key || "trees" == key)
        {
            if (!(setValue < value))
            {
                return false;
            }
        }
        else if ("pomeranians" == key || "goldfish" == key)
        {
            if (!(setValue > value))
            {
                return false;
            }
        }
        else
        {
            if (setValue != value)
            {
                return false;
            }
        }
    }

    return true;
}


int _tmain(int argc, _TCHAR * argv[])
{
    Properties properties;
    Properties pattern;
    pattern["children"] = 3;
    pattern["cats"] = 7;
    pattern["samoyeds"] = 2;
    pattern["pomeranians"] = 3;
    pattern["akitas"] = 0;
    pattern["vizslas"] = 0;
    pattern["goldfish"] = 5;
    pattern["trees"] = 3;
    pattern["cars"] = 2;
    pattern["perfumes"] = 1;

    {
        ::std::ifstream infile("../Day16/input.txt");
        ::std::string line;
        Tokens tokens;
        while (::std::getline(infile, line))
        {
            tokenize(line, tokens);

            properties.clear();
            for (unsigned int i = 2; i < tokens.size(); i += 2)
            {
                auto name = tokens[i];
                name.pop_back();
                const auto value = ::std::atoi(tokens[i + 1].c_str());
                properties.emplace(::std::make_pair(name, value));
            }

            if (isSubSet(pattern, properties))
            {
                ::std::cout << tokens[0] << ' ' << tokens[1] << ::std::endl;
            }
        }
    }

    ::std::cout << "*** part 2" << ::std::endl;
    {
        ::std::ifstream infile("../Day16/input.txt");
        ::std::string line;
        Tokens tokens;
        while (::std::getline(infile, line))
        {
            tokenize(line, tokens);

            properties.clear();
            for (unsigned int i = 2; i < tokens.size(); i += 2)
            {
                auto name = tokens[i];
                name.pop_back();
                const auto value = ::std::atoi(tokens[i + 1].c_str());
                properties.emplace(::std::make_pair(name, value));
            }

            if (isSubSet2(pattern, properties))
            {
                ::std::cout << tokens[0] << ' ' << tokens[1] << ::std::endl;
            }
        }
    }


    return 0;
}
