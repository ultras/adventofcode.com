#include <SDKDDKVer.h>
#include <tchar.h>

#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>


bool checkValid(const ::std::string & password)
{
    unsigned int increasingStraight = 0;
    unsigned pairs = 0;

    char previous = '\x0';
    char previousPair = '\x0';
    for (auto ch : password)
    {
        switch (ch)
        {
            case 'i':
            case 'o':
            case 'l':
                return false;
        }

        if (2 > increasingStraight)
        {
            if (previous + 1 == ch)
            {
                ++increasingStraight;
            }
            else
            {
                increasingStraight = 0;
            }
        }

        if (ch == previousPair)
        {
            ++pairs;
            previousPair = '\x0';
        }
        else
        {
            previousPair = ch;
        }

        previous = ch;        
    }

    return 2 <= increasingStraight && 2 <= pairs;
}


void nextPassword(::std::string & password)
{
    for (;;)
    {
        // first iterate
        for (auto iter = password.rbegin(); iter != password.rend(); ++iter)
        {
            if ('z' == *iter)
            {
                *iter = 'a';
            }
            else
            {
                ++*iter;
                break;
            }
        }

        if (checkValid(password))
        {
            break;
        }
    }
}


::std::string testPassword(const char * value)
{
    ::std::string password(value);
    nextPassword(password);
    return password;
}


int _tmain(int argc, _TCHAR * argv[])
{
    ::std::string password("hepxcrrq");

    assert("abcdffaa" == testPassword("abcdefgh"));
    ::std::cout << "test1" << ::std::endl;
    assert("ghjaabcc" == testPassword("ghijklmn"));
    ::std::cout << "test2" << ::std::endl;

    nextPassword(password);
    ::std::cout << password.c_str() << ::std::endl;

    nextPassword(password);
    ::std::cout << password.c_str() << ::std::endl;

    return 0;
}
