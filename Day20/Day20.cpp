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


unsigned int countHouse(unsigned int house)
{
	unsigned int result = 0;
	for (unsigned int elf = 1; elf <= house; ++elf)
	{
		if (0 == (house % elf))
		{
			result += elf * 10;
		}		
	}
	return result;
}


unsigned int countHouse2(unsigned int house)
{
	unsigned int result = 0;
	for (unsigned int elf = 1; elf <= house; ++elf)
	{
		if (0 != (house % elf))
		{
			continue;
		}

		unsigned int maxHouse = elf * 50;
		if (maxHouse < house)
		{
			continue;
		}

		result += elf * 11;
	}
	return result;
}


int _tmain(int argc, _TCHAR * argv[])
{
	assert(10 == countHouse(1));
	assert(30 == countHouse(2));
	assert(40 == countHouse(3));
	assert(70 == countHouse(4));
	assert(60 == countHouse(5));
	assert(120 == countHouse(6));
	assert(80 == countHouse(7));
	assert(150 == countHouse(8));
	assert(130 == countHouse(9));


	// no up to 500000
	// no up to 600000
	// yes: 665280, 702240
	for (unsigned int i = 665280;; ++i) // 1000000
	{
		const auto result = countHouse(i);
		if (29000000 <= result)
		{
			::std::cout << "found " << i << " " << result << ::std::endl << ::std::endl;
			break;
		}
		if (0 == (i % 5000))
		{
			::std::cout << i << " "
						<< result << " "
						<< result / 29000000. << ::std::endl;
		}
	}

	// yse: 705600, 1000008
	for (unsigned int i = 0;; ++i)
	{
		const auto result = countHouse2(i);
		if (29000000 <= result)
		{
			::std::cout << "found " << i << " " << result << ::std::endl << ::std::endl;
			break;
		}
		if (0 == (i % 5000))
		{
			::std::cout << i << " "
				<< result << " "
				<< result / 29000000. << ::std::endl;
		}
	}

	return 0;
}
