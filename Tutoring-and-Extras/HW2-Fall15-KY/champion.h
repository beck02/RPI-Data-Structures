#ifndef __champion_h_
#define __champion_h_

class Champion {
public:
	// Constructors
	Champion(std::string name);
	
	// Accessors
	const std::string& getChampionname() const { return championname; }
	int getWins() const { return wins; }
	int getLosses() const { return losses; }
	float getWinPct() const { return winpct; }
	int getDeathsByMinion() const { return miniondeaths; }

	// Mutators
	void addWin();
	void addLoss();
	void updateWinPct();
	void addDeathByMinion() { miniondeaths++; }

private:
	std::string championname;
	int wins;
	int losses;
	float winpct;
	int miniondeaths;
};

bool sortChampions(const Champion& c1, const Champion& c2);


#endif