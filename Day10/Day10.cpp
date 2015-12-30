// Day10.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <tchar.h>

#include <vector>
#include <iostream>
#include <cstdint>
#include <cassert>


typedef ::std::vector<uint8_t> Numbers;

Numbers lookAndSay(const Numbers & numbers)
{
    Numbers result;
    
    for (unsigned int i = 0; i < numbers.size(); ++i)
    {
        const auto number = numbers[i];
        unsigned int count = 1;

        // check do we have more the same?
        for (auto j = i + 1; j < numbers.size() && number == numbers[j]; ++j, ++i, ++count);

        assert(10 > count);
        result.push_back(count);
        result.push_back(number);
    }

    return result;
}


int _tmain(int argc, _TCHAR * argv[])
{
    const char * data = "1113222113";

    assert(Numbers({ 1, 1 }) == lookAndSay(Numbers(1, 1)));
    assert(Numbers({ 2, 1 }) == lookAndSay(Numbers({ 1, 1 })));
    assert(Numbers({ 1, 2, 1, 1 }) == lookAndSay(Numbers({ 2, 1 })));
    assert(Numbers({ 1, 1, 1, 2, 2, 1 }) == lookAndSay(Numbers({ 1, 2, 1, 1 })));
    assert(Numbers({ 3, 1, 2, 2, 1, 1 }) == lookAndSay(Numbers({ 1, 1, 1, 2, 2, 1 })));    

    Numbers numbers;
    for (auto iter = data; *iter; ++iter)
    {
        numbers.push_back(* iter - '0');
    }

    for (unsigned int i = 0; i < 40; ++i)
    {
        numbers = lookAndSay(numbers);
    }
    ::std::cout << numbers.size() << ::std::endl;

    for (unsigned int i = 0; i < 10; ++i) // overall 50
    {
        numbers = lookAndSay(numbers);
    }
    ::std::cout << numbers.size() << ::std::endl;

	return 0;
}
