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
#include <random>

typedef unsigned long long int Code;


Code findCode(unsigned int x, unsigned int y)
{
	Code result = 20151125;
	for (unsigned int i = 1, j = 1;;)
	{
		if (x == i && y == j)
		{
			return result;
		}

		if (1 == i)
		{
			i = j + 1;
			j = 1;
			result = (result * 252533) % 33554393;
		}
		else
		{
			--i;
			++j;
			result = (result * 252533) % 33554393;
		}
	}
}


int _tmain(int argc, _TCHAR * argv[])
{
	assert(20151125 == findCode(1, 1));
	assert(21629792 == findCode(2, 2));
	assert(28094349 == findCode(5, 3));
	assert(31663883 == findCode(5, 6));

	::std::cout << "(100, 100) = " << findCode(100, 100) << ::std::endl;
	::std::cout << "(2947, 3029) = " << findCode(2947, 3029) << ::std::endl;

	return 0;
}
