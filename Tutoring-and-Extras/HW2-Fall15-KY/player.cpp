
#include <iostream>
#include "player.h"

// Constructors
Player::Player(std::string name) {
	playername = name;
	kills = 0;
	deaths = 0;
	kdr = 0;
	assists = 0;
}

// Mutators
void Player::addKill(){
	kills++;
	updateKdr();
}
void Player::addDeath(){
	deaths++;
	updateKdr();
}
float Player::getAssistsToKills() const {
	if(kills == 0){
		return assists;
	} else{
		return ((float)assists) / ((float)kills);
	}
}

void Player::updateKdr(){
	if(deaths == 0){
		kdr = kills;
	} else{
		kdr = ((float)kills) / ((float)deaths);
	}
}
void Player::addChampion(std::string c){
	for(int i = 0; i < champions.size(); i++){
		if( c == champions[i] ){
			std::string temp = champions[champions.size()-1];
			champions[champions.size()-1] = champions[i];
			champions[i] = temp;
			return;
		}
	}
	champions.push_back(c);
}

bool sortPlayers(const Player& p1, const Player& p2){
	if (p1.getKdr() != p2.getKdr()){
		return p1.getKdr() > p2.getKdr();
	} else{ // kdr1 == kdr2
		if (p1.getKills() != p2.getKills()){
			return p1.getKills() > p2.getKills();
		} else{ // kills1 == kills2
			if (p1.getDeaths() != p2.getDeaths()){
				return p1.getDeaths() < p2.getDeaths();
			} else{ // deaths1 == deaths2
				return p1.getPlayername() < p2.getPlayername();
			}
		}
	}
}

bool sortByAssists(const Player& p1, const Player& p2){
	if (p1.getAssistsToKills() != p2.getAssistsToKills()){
		return p1.getAssistsToKills() > p2.getAssistsToKills();
	} else{ 
		if (p1.getAssists() != p2.getAssists()){
			return p1.getAssists() > p2.getAssists();
		} else{ 
			if (p1.getKills() != p2.getKills()){
				return p1.getKills() > p2.getKills();
			} else{
				return p1.getPlayername() < p2.getPlayername();
			}
		}
	}
}





