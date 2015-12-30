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


typedef ::std::map<::std::string, ::std::vector<::std::string>> Replacements;
typedef ::std::vector<::std::pair<::std::string, ::std::string>> ReplacementList;


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


void populate(const ::std::string & key, const ::std::string & molecule, unsigned int index,
		      const Replacements & replacements, ::std::set<::std::string> & molecules)
{
	const auto replacement = replacements.find(key);
	if (replacements.cend() == replacement)
	{
		return;
	}

	for (auto & iter : replacement->second)
	{
		::std::string newMolecule;
		newMolecule.reserve(molecule.size() + iter.size());
		newMolecule.assign(molecule.substr(0, index));
		newMolecule += iter;
		newMolecule += molecule.substr(index + key.size());
		molecules.insert(newMolecule);
	}
}

unsigned int countReplacements(const ::std::string & molecule, const Replacements & replacements)
{
	unsigned int maxReplacementSize = 0;
	for (auto & iter : replacements)
	{
		maxReplacementSize = ::std::max(maxReplacementSize, iter.first.size());
	}

	::std::set<::std::string> molecules;

	for (unsigned int i = 0; i < molecule.size(); ++i)
	{
		::std::string key(1, molecule[i]);
		populate(key, molecule, i, replacements, molecules);

		for (unsigned int j = i + 1; j - i < maxReplacementSize && j < molecule.size(); ++j)
		{
			key += molecule[j];
			populate(key, molecule, i, replacements, molecules);
		}
	}

	return molecules.size();
}

void populate(const ::std::string & source, const Replacements & replacements, unsigned int maxReplacementSize, ::std::set<::std::string> & molecules)
{
	for (const auto & replacement : replacements)
	{
		const auto & key = replacement.first;
		::std::string::size_type start = 0;
		for (;;)
		{
			const auto pos = source.find(key, start);
			if (::std::string::npos == pos)
			{
				break;
			}

			for (const auto & newValue : replacement.second)
			{
				::std::string newMolecule;
				newMolecule.reserve(source.size() + newValue.size());
				newMolecule.assign(source.substr(0, pos));
				newMolecule += newValue;
				newMolecule += source.substr(pos + key.size());
				molecules.insert(newMolecule);
			}

			start = pos + 1;
		}
	}
}

bool iterate(::std::string & molecule, const ReplacementList & replacementList, unsigned int & counter)
{
	bool result = false;
	for (auto & replacement : replacementList)
	{
		const auto & key = replacement.first;
		const auto pos = molecule.find(key);
		if (::std::string::npos == pos)
		{
			continue;
		}
		molecule.replace(pos, key.size(), replacement.second);
		++counter;
		result = true;
	}
	return result;
}


int _tmain(int argc, _TCHAR * argv[])
{
	Replacements replacements;
	Replacements reverseReplacements;
	ReplacementList replacementList;
	::std::string molecule;

	::std::ifstream infile("../Day19/input.txt");
	::std::string line;
	Tokens tokens;
	while (::std::getline(infile, line))
	{
		tokenize(line, tokens);
		if (3 == tokens.size())
		{
			const auto & key = tokens[0];
			const auto & value = tokens[2];

			auto iter = replacements.find(key);
			if (iter == replacements.end())
			{	
				iter = replacements.emplace(::std::make_pair(key, ::std::vector<::std::string>())).first;
			}
			iter->second.push_back(value);

			iter = reverseReplacements.find(value);
			if (iter == reverseReplacements.end())
			{
				iter = reverseReplacements.emplace(::std::make_pair(value, ::std::vector<::std::string>())).first;
			}
			iter->second.push_back(key);

			replacementList.push_back(::std::make_pair(value, key));
		}
		else if (1 == tokens.size())
		{
			assert(molecule.empty());
			molecule = line;
		}
	}

	auto result = countReplacements(molecule, replacements);
	::std::cout << result << ::std::endl;

	// step 2
	// https://www.reddit.com/r/adventofcode/comments/3xhkeb/day_19_part_2_proof_that_everyones_posted/
	// https://gist.github.com/desrtfx/eab96fd43675c9853bd7

	/*
	unsigned int maxReplacementSize = 0;
	for (auto & iter : reverseReplacements)
	{
		maxReplacementSize = ::std::max(maxReplacementSize, iter.first.size());
	}

	::std::set<::std::string> molecules;
	molecules.insert(molecule);

	for (unsigned int steps = 1; ; ++steps)
	{
		decltype(molecules) sources(::std::move(molecules));
		for (const auto & iter : sources)
		{
			populate(iter, reverseReplacements, maxReplacementSize, molecules);
		}

		if (0 == molecules.size())
		{
			::std::cout << "not found!" << ::std::endl;
			break;
		}

		if (molecules.cend() != molecules.find("e"))
		{
			::std::cout << steps << ::std::endl;
			break;
		}

		unsigned int maxSize = 0;
		for (const auto & iter : molecules)
		{
			maxSize = ::std::max(maxSize, iter.size());
		}
		::std::cout << "maxSize: " << maxSize << " size:" << molecules.size() <<::std::endl;
	}
	*/

	::std::random_device rd;
	::std::mt19937 g(rd());
	::std::shuffle(replacementList.begin(), replacementList.end(), g);

	::std::string source(molecule);
	unsigned int counter = 0;
	for (;;)
	{
		if (!iterate(source, replacementList, counter))
		{
			::std::shuffle(replacementList.begin(), replacementList.end(), g);
			source = molecule;
			counter = 0;
			continue;
		}
		if ("e" == source)
		{
			break;
		}
	}
	::std::cout << "counter: " << counter << " " << source << ::std::endl;

	return 0;
}
