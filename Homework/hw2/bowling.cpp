/**
File: bowling.cpp
Purpose: Read bowling game scores from a file and print them out 
	as a formatted, sorted table into a file.
**/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "player.h"
using namespace std;

int main( int argc, char* argv[] ){
	if( argc != 3 ){
		cerr << "Please enter two arguments after the command name: \n" 
			<< "(filename to be read from) (filename to be written to)\n" << std::endl;
		return 1;
	}
	ifstream in_str(argv[1]);
	if (!in_str.good()) {
		cerr << "Can't open " << argv[1] << " to read.\n";
		return 1;
	}
	ofstream out_str(argv[2]);
	if (!out_str.good()) {
		cerr << "Can't open " << argv[2] << " to write.\n";
		return 1;
	}
	
	//read players from file and put them in a vector
	vector<Player> playerList = readPlayersFromFile(in_str);
	//sort the vector alphabetically by last name
	sort(playerList.begin(), playerList.end(), sortAlphaOrder);
	
	//find max name length
	int maxLength = 0;
	for(unsigned int i = 0; i < playerList.size(); i++){
		maxLength = max(maxLength, playerList[i].getNameLength());
	}
	
	//output players into table
	int lineLength = 66 + maxLength;
	out_str << string(lineLength, '-') << endl;
	for(unsigned int i = 0; i < playerList.size(); i++){
		playerList[i].outputTableRow(out_str, maxLength);
		out_str << string(lineLength, '-') << endl;
	}
	
	//output player list with final scores
	out_str << endl;
	sort(playerList.begin(), playerList.end(), sortFinalScoreHiLow);
	for(unsigned int i = 0; i < playerList.size(); i++){
		out_str << playerList[i].getFName() << " " << playerList[i].getLName()
				<< string(maxLength - playerList[i].getNameLength() + 2, ' ');
		int score = playerList[i].getFrameScore(10);
		if( score < 10 ){
			out_str << "  " << score << endl;
		} else if( (score > 9) && (score < 100) ){
			out_str << " " << score << endl;
		} else if( score > 99 ){
			out_str << score << endl;
		}
	}
	
	//third output
	out_str << endl << "Creative Statistic: Bonus Points (extra points earned from X or /)" << endl;
	sort(playerList.begin(), playerList.end(), sortBonusPointsHiLow);
	for(unsigned int i = 0; i < playerList.size(); i++){
		out_str << playerList[i].getFName() << " " << playerList[i].getLName()
				<< string(maxLength - playerList[i].getNameLength() + 2, ' ');
		int bonus = playerList[i].getBonusPoints();
		if( bonus < 10 ){
			out_str << "  " << bonus << endl;
		} else if( (bonus > 9) && (bonus < 100) ){
			out_str << " " << bonus << endl;
		} else if( bonus > 99 ){
			out_str << bonus << endl;
		}
	}
	
	return 0;
}



