#ifndef __player_h_
#define __player_h_

#include <vector>
#include "champion.h"

class Player {
public:
	// Constructors
	Player(std::string name);
	
	// Accessors
	const std::string& getPlayername() const { return playername; }
	int getKills() const { return kills; }
	int getDeaths() const { return deaths; }
	int getAssists() const { return assists; }
	float getAssistsToKills() const;
	float getKdr() const { return kdr; }
	const std::vector<std::string>& getChampions() const { return champions; }
	const std::string& getCurrentChamp() const { return champions[champions.size()-1]; }

	// Mutators
	void addKill();
	void addDeath();
	void addAssist() { assists++; }
	void updateKdr();
	void addChampion(std::string c);
	void sortChampsForPrinting(){
		sort(champions.begin(), champions.end());
	}

private:
	std::string playername;
	int kills;
	int deaths;
	int assists;
	float kdr;
	std::vector<std::string> champions;
};

bool sortPlayers(const Player& p1, const Player& p2);
bool sortByAssists(const Player& p1, const Player& p2);


#endif