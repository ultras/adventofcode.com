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


struct Actor
{
	int hitPoints;
	int damage;
	int armor;

	Actor(int hitPoints_, int damage_, int armor_)
		: hitPoints(hitPoints_)
		, damage(damage_)
		, armor(armor_)
	{}
};

int fight(Actor & l, Actor & r)
{
	for (;;)
	{
		r.hitPoints -= ::std::max(1, l.damage - r.armor);
		if (0 >= r.hitPoints)
		{
			return 1;
		}

		l.hitPoints -= ::std::max(1, r.damage - l.armor);
		if (0 >= l.hitPoints)
		{
			return -1;
		}
	}
}



int _tmain(int argc, _TCHAR * argv[])
{
	assert(1 == fight(Actor(8, 5, 5), Actor(12, 7, 2)));

	const ::std::map<int, int> c_weapons({ // Cost -> Damage
		{ 8, 4 },
		{ 10, 5 },
		{ 25, 6 },
		{ 40, 7 },
		{ 74, 8 },
		});

	const ::std::map<int, int> c_armor({ // Cost -> Armor
		{ 0, 0 },
		{ 13, 1 },
		{ 31, 2 },
		{ 53, 3 },
		{ 75, 4 },
		{ 102, 5 },
		});

	const ::std::map<int, ::std::pair<int, int>> c_rings({ // Cost -> Damage, Armor
		{ 0, { 0, 0 } },
		{ 0, { 0, 0 } },
		{ 25, { 1, 0 } },
		{ 50, { 2, 0 } },
		{ 100, { 3, 0 } },
		{ 20, { 0, 1 } },
		{ 40, { 0, 2 } },
		{ 80, { 0, 3 } } });

	const Actor c_boss(103, 9, 2);
	const Actor c_me(100, 0, 0);

	int minCost = INT_MAX;
	int maxCost = 0;
	for (auto & weapon : c_weapons)
	{
		for (auto & armor : c_armor)
		{
			for (auto & ring1 : c_rings)
			{
				for (auto & ring2 : c_rings)
				{
					if (ring1 == ring2)
					{
						continue;
					}

					Actor boss(c_boss);
					Actor me(c_me);
					me.damage += weapon.second;					
					me.damage += ring1.second.first;
					me.damage += ring2.second.first;
					me.armor += armor.second;
					me.armor += ring1.second.second;
					me.armor += ring2.second.second;

					const int cost = weapon.first + armor.first + ring1.first + ring2.first;
					if (1 == fight(me, boss))
					{						
						minCost = ::std::min(minCost, cost);
						/*if (121 == cost)
						{
							::std::cout << "weapon " << weapon.first
										<< " armor " << armor.first
										<< " ring1 " << ring1.first << ", " << ring1.second.first << ", " << ring1.second.second
										<< " ring2 " << ring2.first << ", " << ring2.second.first << ", " << ring2.second.second
										<< ::std::endl;
						}*/
					}
					else
					{
						maxCost = ::std::max(maxCost, cost);
					}
				}
			}
		}
	}
	::std::cout << "minCost " << minCost << ::std::endl;
	::std::cout << "maxCost " << maxCost << ::std::endl;

	return 0;
}
