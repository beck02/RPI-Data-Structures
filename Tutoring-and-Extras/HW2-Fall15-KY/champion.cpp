#include <iostream>
#include "champion.h"

// Constructors
Champion::Champion(std::string name) {
	championname = name;
	wins = 0;
	losses = 0;
	winpct = 0;
	miniondeaths = 0;
}

// Mutators
void Champion::addWin(){
	wins++;
	updateWinPct();
}
void Champion::addLoss(){
	losses++;
	updateWinPct();
}
void Champion::updateWinPct(){
	winpct = ((float)wins) / ((float) (wins + losses));

}

bool sortChampions(const Champion& c1, const Champion& c2){
	if (c1.getWinPct() != c2.getWinPct()){
		return c1.getWinPct() > c2.getWinPct();
	} else{ // winpct1 == winpct2
		if (c1.getWins() != c2.getWins()){
			return c1.getWins() > c2.getWins();
		} else{ // wins1 == wins2
			if (c1.getLosses() != c2.getLosses()){
				return c1.getLosses() < c2.getLosses();
			} else{ // losses1 == losses2
				return c1.getChampionname() < c2.getChampionname();
			}
		}
	}
}