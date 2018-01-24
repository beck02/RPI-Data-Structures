/** 
File: player.h
Purpose: Header file with declaration of the Player class, including 
member functions and private member variables.
**/

#ifndef __player_h_
#define __player_h_

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

class Player {
public:
	//Constructors
	Player(string fn, string ln);
	
	//Accessors
	const string& getFName() const { return mfName; }
	const string& getLName() const { return mlName; }
	int getNumThrows() const { return mnumThrows; }
	int getNameLength() const { return mnameLength; }
	int getBonusPoints() const { return mbonusPoints; }
	int getFrameScore(int frameNum) const;
	int getThrowScore(int throwNum) const;
	
	//Mutators
	void addThrow(int th, int thrNum, int frame);
	
	//Other helpers
	void calculateScores();
	ostream& outputTableRow(ostream& out_str, int maxLength) const;
	
private: //Member variables (all begin with character "m" for uniformity/recognizability)
	string mfName;
	string mlName;
	vector<int> mframeScores;
	vector<int> mthrows;
	int mnumThrows;
	int mnameLength;
	int mbonusPoints;
	
};

vector<Player> readPlayersFromFile(istream& in_str);
bool sortAlphaOrder(const Player& pl1, const Player& pl2);
bool sortFinalScoreHiLow(const Player& pl1, const Player& pl2);
bool sortBonusPointsHiLow(const Player& pl1, const Player& pl2);

#endif