/** 
File: player.cpp
Purpose: Implementation of Player class
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "player.h"
using namespace std;

/*std::string mfName;
std::string mlName;
vector<int> mframeScores;
vector<int> mthrows;
int mnumThrows;*/

//Constructors
Player::Player(string fn, string ln){
	mfName = fn;
	mlName = ln;
	mthrows.push_back(0);
	mbonusPoints = 0;
	mnumThrows = 20;
	mnameLength = mfName.size() + mlName.size() + 1;
}

//Accessors
int Player::getFrameScore(int frameNum) const {
	int ret = 0;
	if( (frameNum >= 1) && (frameNum <= 10) ){
		ret = mframeScores[frameNum];
	} else{
		cerr << "Invalid frame number from which to get score.\n"
					<< "Should be: 1 <= frameNum <= 10." << std::endl;
		ret = -1;
	}
	return ret;
}

int Player::getThrowScore(int throwNum) const {
	int ret = 0;
	if( (throwNum >= 1) && (throwNum <= mnumThrows) ){
		ret = mframeScores[throwNum];
	} else{
		cerr << "Invalid throw number about which to get score.\n"
				<< "For this player, should be: 1 <= frameNum <= " << mnumThrows << std::endl;
		ret = -1;
	}
	return ret;
}

//Mutators
void Player::addThrow(int th, int thrNum, int frame){
	if( mthrows.size() < (unsigned int)mnumThrows + 1 ){
		if( (th >= 0) && (th < 10) ){
			if( (frame == 10) && (thrNum == 2) && ((mthrows.back() + th) == 10) ){
				mnumThrows++;
			}
			mthrows.push_back(th);
		} else if(th == 10){
			mthrows.push_back(th);
			if( (frame < 10) && (thrNum == 1) ){
				mnumThrows--;
			} else if( (frame == 10) && (thrNum == 1) ){
				mnumThrows++;
			}
		} else{
			mthrows.push_back(-1);
			cerr << "Invalid value for throw score. Should be between 1 and 10." << endl;
		}
	} else{
		cerr << "All throws for this player have already been entered." << endl;
	}
}
	
//Other helpers
void Player::calculateScores(){
	mframeScores.push_back(0);
	int thNum = 1;
	for(int fNum = 1; fNum <= 10; fNum++){
		int fScore = 0;
		int th1 = mthrows[thNum];
		if(th1 < 10){
			thNum++;
			int th2 = mthrows[thNum];
			if( (th1 + th2) == 10 ){ //spare
				fScore = mframeScores[fNum-1] + th1 + th2 + mthrows[thNum+1];
				mbonusPoints += mthrows[thNum+1];
			} else{
				fScore = mframeScores[fNum-1] + th1 + th2;
			}
		} else if(th1 == 10){ //strike
			fScore = mframeScores[fNum-1] + th1 + mthrows[thNum+1] + mthrows[thNum+2];
			if(fNum < 10){
				mbonusPoints += (mthrows[thNum+1] + mthrows[thNum+2]);
			}
		}
		thNum++;
		mframeScores.push_back(fScore);
	}
}

ostream& Player::outputTableRow(ostream& out_str, int maxLength) const{
	//begin line one of row
	out_str << "| " << mfName << " " << mlName << string(maxLength - mnameLength, ' ') << " ";
	int throwNum = 1;
	for(int fNum = 1; fNum <= 9; fNum++){ //first nine frames
		out_str << "| ";
		for(int i = 1; i <= 2; i++){ //once per throw in each frame
			if( mthrows[throwNum] == 0 ){
				out_str << "- ";
			} else if( (i == 2) && ((mthrows[throwNum] + mthrows[throwNum-1]) == 10) ){
				out_str << "/ ";
			} else if( mthrows[throwNum] == 10 ){
				out_str << "  X ";
				throwNum++;
				break;
			} else{
				out_str << mthrows[throwNum] << " ";
			}
			throwNum++;
		}
	}
	//last frame
	out_str << "| ";
	int n = 2;
	for(int i = 1; i <= n; i++){ //once per throw in frame
		if( mthrows[throwNum] == 0 ){
			out_str << "- ";
		} else if( (i == 2) && ((mthrows[throwNum] + mthrows[throwNum-1]) == 10) ){
			out_str << "/ ";
			n = 3;
		} else if( mthrows[throwNum] == 10 ){
			out_str << "X ";
			n = 3;
		} else{
			out_str << mthrows[throwNum] << " ";
		}
		throwNum++;
	}
	if(n == 2){
		out_str << "  ";
	}
	out_str << "|" << endl;
	//end of line one of row
	//begin line two of row
	out_str << "|" << string(maxLength + 2, ' ');
	for(int fNum = 1; fNum <= 10; fNum++){
		out_str << "| ";
		if(fNum == 10){
			out_str << "  ";
		}
		int currScore = mframeScores[fNum];
		if( currScore < 10 ){
			out_str << "  " << currScore << " ";
		} else if( (currScore > 9) && (currScore < 100) ){
			out_str << " " << currScore << " ";
		} else if( currScore > 99 ){
			out_str << currScore << " ";
		}
	}
	out_str << "|" << endl;
	//end line two of row	
	return out_str;
}

vector<Player> readPlayersFromFile(istream& in_str){
	vector<Player> pList;
	string tempFN;
	while( in_str >> tempFN ){
		string tempLN;
		in_str >> tempLN;
		Player tPl(tempFN, tempLN);
		int thrNum = 1;
		int frame = 1;
		for(int i = 1; i <= tPl.getNumThrows(); i++){
			int th;
			if( !(in_str >> th) ){
				cerr << "The throw failed to read." << endl;
			}
			tPl.addThrow(th, thrNum, frame);
			if( (th == 10) && (thrNum == 1) ){
				if(frame < 10){
					frame++;
				} else if(frame == 10){
					thrNum = 2;
				}
			} else if( (th < 10) && (thrNum == 1) ){
				thrNum = 2;
			} else if( thrNum == 2 ){
				if(frame < 10){
					thrNum = 1;
					frame++;
				} else if( frame == 10 ){
					thrNum = 3;
				}
			}
		}
		tPl.calculateScores();
		pList.push_back(tPl);
	}
	return pList;
}	

bool sortAlphaOrder(const Player& pl1, const Player& pl2){
	return ( pl1.getLName() < pl2.getLName() ) || 
			( (pl1.getLName() == pl2.getLName()) && (pl1.getFName() < pl2.getFName()) );
}

bool sortFinalScoreHiLow(const Player& pl1, const Player& pl2){
	return ( pl1.getFrameScore(10) > pl2.getFrameScore(10) );
}

bool sortBonusPointsHiLow(const Player& pl1, const Player& pl2){
	return ( pl1.getBonusPoints() > pl2.getBonusPoints() );
}



