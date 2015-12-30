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


typedef ::std::map<::std::string, ::std::map<::std::string, int>> Scores;
typedef ::std::set<::std::string> NameSet;
typedef ::std::vector<::std::string> Names;


void iterate(::std::vector<::std::string> & items,
			 ::std::function<void(const ::std::vector<::std::string> &)> callback)
{
	::std::sort(items.begin(), items.end());
	do
	{
		callback(items);
	}
	while (::std::next_permutation(items.begin(), items.end()));
}


void maxScore(const Scores & scores, const NameSet & nameSet)
{
	Names names(nameSet.cbegin(), nameSet.cend());
	int maxScore = 0;
	iterate(names,
		[&scores, &maxScore](const ::std::vector<::std::string> & items)
	{
		int score = 0;
		auto next = items.cbegin();
		++next;
		auto prev = items.cend();
		prev--;

		for (auto iter = items.cbegin(); iter != items.cend(); ++iter)
		{
			auto scoresIter = scores.find(*iter);
			assert(scoresIter != scores.cend());
			const auto score1 = scoresIter->second.find(*prev)->second;
			const auto score2 = scoresIter->second.find(*next)->second;
			score += score1;
			score += score2;

			if (items.cend() == ++next)
			{
				next = items.cbegin();
			}
			if (items.cend() == ++prev)
			{
				prev = items.cbegin();
			}
		}

		maxScore = ::std::max(maxScore, score);
	});
	::std::cout << maxScore << ::std::endl;
}


int _tmain(int argc, _TCHAR * argv[])
{
	Scores scores;
	NameSet nameSet;

	::std::ifstream infile("../Day13/input.txt");
    ::std::string line;
	Tokens tokens;
    while (::std::getline(infile, line))
    {
		tokenize(line, tokens);
		const auto & name = tokens[0];
		auto sittingTo = tokens[10];
		if ('.' == *sittingTo.crbegin())
		{
			sittingTo.erase(--sittingTo.end());
		}
		const auto score = ::std::atoi(tokens[3].c_str()) * ("gain" == tokens[2] ? 1 : -1);

		assert(0 != score);
		assert(scores.cend() == scores.find(name) || scores[name].cend() == scores[name].find(sittingTo));
		scores[name][sittingTo] = score;

		nameSet.insert(name);
    }

	maxScore(scores, nameSet);

	// Step 2
	// Add me:
	nameSet.insert("Me");
	for (auto & iter : scores)
	{
		scores["Me"][iter.first] = 0;
		iter.second["Me"] = 0;
	}
	maxScore(scores, nameSet);

    return 0;
}
