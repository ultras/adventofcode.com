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


struct Boss
{
	int hitPoints;
	int damage;

	Boss(int hitPoints_, int damage_)
		: hitPoints(hitPoints_)
		, damage(damage_)
	{}
};


struct Spell
{
	int cost;
	int turns;
	const char * name;

	int damage;
	int armor;
	int heals;
	int mana;

	Spell(const char * name_ = nullptr, int cost_ = 0, int turns_ = 1)
		: name(name_)
		, cost(cost_)
		, turns(turns_)
		, damage(0)
		, armor(0)
		, heals(0)
		, mana(0)
	{
	}

	bool turn()
	{
		--turns;
		return 0 < turns;
	}

	bool instantly() const
	{
		return 1 == turns;
	}
};
typedef ::std::map<const char *, Spell> SpellMap;
typedef ::std::vector<Spell> Spells;

Spell & find(Spells & spells, const char * name)
{
	const auto iter = ::std::find_if(spells.begin(), spells.end(),
		[name](const Spell & iter)
		{
			return iter.name == name;
		});
	assert(iter != spells.end());
	return *iter;
}


struct Player
{
	int hitPoints;
	int mana;
	int spentMana;
	SpellMap spells;
	::std::vector<::std::string> usedSpells;

	int damage;
	int armor;

	Player(int hitPoints_, int mana_)
		: hitPoints(hitPoints_)
		, mana(mana_)
		, spentMana(0)
		, damage(0)
		, armor(0)
	{}

	bool cast(const Spell & spell)
	{
		assert(spell.name);
		assert(0 < spell.cost);

		if (spells.cend() != spells.find(spell.name))
		{
			return false;
		}

		if (mana < spell.cost)
		{
			return false;
		}

		mana -= spell.cost;
		spentMana += spell.cost;
		spells[spell.name] = Spell(spell);

		usedSpells.push_back(spell.name);
		return true;
	}

	void turn()
	{
		damage = armor = 0;
		for (auto iter = spells.begin(); iter != spells.end();)
		{
			const auto & spell = iter->second;
			damage += spell.damage;
			armor += spell.armor;
			hitPoints += spell.heals;
			mana += spell.mana;

			if (iter->second.turn())
			{
				++iter;
			}
			else
			{
				iter = spells.erase(iter);
			}
		}
	}

	void printUsedSpells()
	{
		/*
		::std::cout << "--------------------------" << ::std::endl;
		for (auto & iter : usedSpells)
		{
			::std::cout << iter << ::std::endl;
		}
		::std::cout << "--------------------------" << ::std::endl << ::std::endl;
		*/
	}
};


bool playerTurn(Player & player, const Spell & spell, Boss & boss, ::std::function<void(int spentMana)> callback)
{
	// Part Two
	player.hitPoints -= 1;
	if (0 >= player.hitPoints)
	{
		// lost
		return false;
	}

	const auto instantly = spell.instantly();
	if (instantly)
	{
		if (!player.cast(spell))
		{
			return false;
		}
	}

	// apply spells
	player.turn();

	// kick boss
	boss.hitPoints -= player.damage;
	if (0 >= boss.hitPoints)
	{
		// Won!
		player.printUsedSpells();
		callback(player.spentMana);
		return false;
	}

	if (!instantly)
	{
		if (!player.cast(spell))
		{
			return false;
		}
	}

	return true;
}

bool bossTurn(Player & player, Boss & boss, ::std::function<void(int spentMana)> callback)
{
	// apply spells
	player.turn();

	// kick boss
	boss.hitPoints -= player.damage;
	if (0 >= boss.hitPoints)
	{
		// Won!
		player.printUsedSpells();
		callback(player.spentMana);
		return false;
	}

	player.hitPoints -= ::std::max(1, boss.damage - player.armor);
	if (0 >= player.hitPoints)
	{
		// lost
		return false;
	}

	return true;
}

void searchWinStrategy(const Player & player, const Boss & boss,
					   const Spells & spells,
					   ::std::function<void(int spentMana)> callback)
{
	// speel a cast
	for (const auto & spell : spells)
	{
		Player playerIter(player);
		Boss bossIter(boss);
		if (!playerTurn(playerIter, spell, bossIter, callback))
		{
			continue;
		}

		if (!bossTurn(playerIter, bossIter, callback))
		{
			continue;
		}

		searchWinStrategy(playerIter, bossIter, spells, callback);
	}
}


int _tmain(int argc, _TCHAR * argv[])
{
	const Boss c_boss(71, 10);
	const Player c_player(50, 500);

	//const Boss c_boss(13, 8);
	//const Player c_player(10, 250);

	Spells c_spells({
		Spell("Recharge", 229, 5),
		Spell("Shield", 113, 6),
		Spell("Poison", 173, 6),
		Spell("Magic Missile", 53),
		Spell("Drain", 73),
		});
	find(c_spells, "Magic Missile").damage = 4;
	find(c_spells, "Drain").damage = 2;
	find(c_spells, "Drain").heals = 2;
	find(c_spells, "Shield").armor = 7;
	find(c_spells, "Poison").damage = 3;
	find(c_spells, "Recharge").mana = 101;


	int minMana = INT_MAX;
	Boss boss(c_boss);
	Player player(c_player);
	searchWinStrategy(player, boss, c_spells,
		[&minMana](int spentMana)
		{
			::std::cout << "spentMana: " << spentMana << ::std::endl;
			minMana = ::std::min(minMana, spentMana);
		});
	::std::cout << "minMana: " << minMana << ::std::endl;
}
