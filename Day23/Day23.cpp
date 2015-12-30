#include <SDKDDKVer.h>
#include <tchar.h>

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <functional>
#include <algorithm>
#include <cstdint>
#include <cassert>


typedef ::std::vector<::std::string> Tokens;
void tokenize(const ::std::string & line, Tokens & tokens, const ::std::set<char> & delimiters)
{
    tokens.clear();

    ::std::string token;
    for (auto iter : line)
    {
        if (delimiters.cend() != delimiters.find(iter))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
        {
            token.push_back(iter);
        }
    }

    if (!token.empty())
    {
        tokens.push_back(token);
    }
}


typedef ::std::map<::std::string, unsigned int> Registers;

void print(const Registers & registers)
{
    for (const auto & iter : registers)
    {
        ::std::cout << iter.first << " = " << iter.second << ::std::endl;
    }
}



class Command
{
public:
    enum class Type
    {
        hlf,
        tpl,
        inc,
        jmp,
        jie,
        jio,
    };

public:
    Command(Tokens && tokens)
        : mParams(::std::move(tokens))
    {
        assert(0 < mParams.size());

        const static ::std::map<::std::string, Type> c_commands =
        {
            { "hlf", Type::hlf },
            { "tpl", Type::tpl },
            { "inc", Type::inc },
            { "jmp", Type::jmp },
            { "jie", Type::jie },
            { "jio", Type::jio },
        };

        auto commandIter = c_commands.find(mParams[0]);
        assert(c_commands.cend() != commandIter);
        mType = commandIter->second;
        mParams.erase(mParams.begin());
    }

    void execute(Registers & registers, unsigned int & commandIndex) const
    {
        ++commandIndex;
        switch (mType)
        {
            case Type::hlf:
            {
                assert(1 == mParams.size());
                const auto & registerName = mParams[0];
                registers[registerName] /= 2;                
            }   break;

            case Type::tpl:
            {
                assert(1 == mParams.size());
                const auto & registerName = mParams[0];
                registers[registerName] *= 3;
            }   break;

            case Type::inc:
            {
                assert(1 == mParams.size());
                const auto & registerName = mParams[0];
                registers[registerName] += 1;
            }   break;

            case Type::jmp:
            {
                assert(1 == mParams.size());
                const auto offset = ::std::atoi(mParams[0].c_str());
                assert(0 <= offset || 0 <= static_cast<int>(commandIndex) + offset);
                commandIndex -= 1; // from top
                commandIndex += offset;
            }   break;

            case Type::jie:
            {
                assert(2 == mParams.size());

                const auto & registerName = mParams[0];
                if (0 == (registers[registerName] % 2))
                {
                    const auto offset = ::std::atoi(mParams[1].c_str());
                    assert(0 <= offset || 0 <= static_cast<int>(commandIndex)+offset);
                    commandIndex -= 1; // from top
                    commandIndex += offset;
                }
            }   break;

            case Type::jio:
            {
                assert(2 == mParams.size());

                const auto & registerName = mParams[0];
                if (1 == registers[registerName])
                {
                    const auto offset = ::std::atoi(mParams[1].c_str());
                    assert(0 <= offset || 0 <= static_cast<int>(commandIndex)+offset);
                    commandIndex -= 1; // from top
                    commandIndex += offset;
                }
            }   break;
        }
    }

private:
    Type        mType;
    Tokens      mParams;

}; // class Command
typedef ::std::vector<Command> Commands;


void run(Registers & registers, const Commands & commands)
{
    for (unsigned int index = 0; index < commands.size();)
    {
        commands.at(index).execute(registers, index);
    }
}


int _tmain(int argc, _TCHAR * argv[])
{
    Commands commands;
    Registers registers = {
        { "a", 0 },
        { "b", 0 },
    };

    ::std::ifstream infile("../Day23/input.txt");
    ::std::string line;
    const ::std::set<char> c_delimiters({' ', ','});
    Tokens tokens;
    while (::std::getline(infile, line))
    {
        tokenize(line, tokens, c_delimiters);
        commands.push_back(Command(::std::move(tokens)));
    }

    run(registers, commands);
    print(registers);

    registers = Registers({
        { "a", 1 },
        { "b", 0 },
    });
    run(registers, commands);
    print(registers);

    return 0;
}
