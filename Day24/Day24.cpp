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


typedef ::std::vector<unsigned int> Weights;
typedef ::std::set<unsigned int> WeightSet;


template <typename T>
void print(const T & items)
{
	for (auto iter : items)
	{
		::std::cout << iter << " ";
	}
	::std::cout << ::std::endl;
}


template <typename T>
bool in(const T & items, const typename T::value_type & value)
{
	return items.cend() != items.find(value);
}


bool iterate(unsigned int iterations, unsigned int iteration, unsigned int target,
			 Weights & weights,
			 ::std::function<bool(const WeightSet &)> callback)
{
	if (::std::accumulate(weights.begin(), weights.end(), 0) != iteration * target)
	{
		return false;
	}

	if (1 == iteration) // found
	{
		return true;
	}

	const auto size = weights.size();
	const WeightSet weightSet(weights.begin(), weights.end());
	::std::vector<bool> flags(size, false);
	WeightSet usedWeights;
	Weights leftWeights;

	unsigned int subSum = 0;
	for (bool stop = false; !stop;)
	{
		// find the expected sum
		for (unsigned int i = 0; i <= size; ++i)
		{
			if (i == size)
			{
				stop = true;
				break;
			}

			auto & flag = flags[i];
			const auto wieght = weights[i];
			if (flag = !flag)
			{
				subSum += wieght;
				usedWeights.insert(wieght);
				break;
			}
			else
			{
				subSum -= wieght;
				usedWeights.erase(wieght);
			}
		}
		if (stop || subSum > target)
		{
			continue;
		}

		const auto rest = target - subSum;
		if (usedWeights.cend() != usedWeights.find(rest) || weightSet.cend() == weightSet.find(rest))
		{
			continue;
		}

		// check can be rest items distributed?
		usedWeights.insert(rest);		
		leftWeights.clear();
		::std::copy_if(weights.cbegin(), weights.cend(),
					   ::std::back_inserter(leftWeights),
					   [&usedWeights](const unsigned int & iter)
				       {
						   return !in(usedWeights, iter);
				       });
		// NOTES: leftWeights is sorted because weights is sorted.
		if (iterate(iterations, iteration - 1, target, leftWeights, callback))
		{
			// the callback should be called by the top iteration to provide the searched parameters
			if (iterations != iteration)
			{
				return true;
			}

			if (!callback(usedWeights))
			{
				break;
			}
		}
		usedWeights.erase(rest);
	}

	return false;
}


void iterate(unsigned int chunks, Weights & weights, ::std::function<bool(const WeightSet &)> callback)
{
	auto target = ::std::accumulate(weights.begin(), weights.end(), 0);
	assert(0 == target % chunks);
	target /= chunks;

	// greedy!
	::std::sort(weights.begin(), weights.end(), ::std::greater<int>());
	iterate(chunks, chunks, target, weights, callback);
}


int _tmain(int argc, _TCHAR * argv[])
{
	Weights weights;

	::std::ifstream infile("../Day24/input.txt");
	::std::string line;
	while (::std::getline(infile, line))
	{
		const auto weight = ::std::atoi(line.c_str());
		assert(0 < weight);
		weights.push_back(weight);
	}

	//weights = Weights({ 1, 2, 3, 4, 5, 7, 8, 9, 10, 11 });

	unsigned int minSize = 0;
	unsigned long long int minQuantum = ULLONG_MAX;
	Weights result;

	iterate(3, weights,
		[&weights, &minSize, &minQuantum, &result](const WeightSet & weight) -> bool
		{
			if (0 == minSize)
			{
				minSize = weight.size();
			}
			else if (weight.size() > minSize)
			{
				return false;
			}

			unsigned long long int quantum = 1;
			for (auto iter : weight)
			{
				quantum *= iter;
			}

			if (quantum < minQuantum)
			{
				minQuantum = quantum;
				::std::cout << "quantum: " << quantum << " size: " << minSize << ::std::endl;
				result.assign(weight.cbegin(), weight.cend());
				print(result);
			}

			return true;
		});

	::std::cout << "minSize: " << minSize << ::std::endl;
	::std::cout << "minQuantum: " << minQuantum << ::std::endl;
	for (auto iter : result)
	{
		::std::cout << iter << " ";
	}
	::std::cout << ::std::endl;

	return 0;
}
