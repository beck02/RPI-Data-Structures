

#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <algorithm>
#include "player.h"
#include "champion.h"

//=====================================================================

void parseEvent(std::ifstream& in, std::string& s, std::vector<Player>& pList, std::vector<Champion>& cList){
	assert(s == "@");
	std::string tStamp;
	in >> tStamp;
	std::string killer;
	in >> killer;
	in >> s;
	std::vector<std::string> assistants;
	if(s == "["){
		while( (in >> s) && (s != "]") ){
			assistants.push_back(s);
		}
		assert(s == "]");
		in >> s;
	}
	assert(s == "killed");
	std::string victim;
	in >> victim;

	for(int i = 0; i < pList.size(); i++){
		for(int k = 0; k < assistants.size(); k++){
			if(pList[i].getPlayername() == assistants[k]){
				pList[i].addAssist();
			}
		}
		if(pList[i].getPlayername() == killer){
			pList[i].addKill();
		}
		if(pList[i].getPlayername() == victim){
			pList[i].addDeath();
			if(killer == "minion"){
				std::string deadchamp = pList[i].getCurrentChamp();
				for(int j = 0; j < cList.size(); j++){
					if(cList[j].getChampionname() == deadchamp){
						cList[j].addDeathByMinion();
					}
				}
			}
		}
	}
}

//=====================================================================

void printPlayers(std::ofstream& out, const std::vector<Player>& pList){
	std::string spacing = "   ";
	std::string header1 = "PLAYER NAME";
	std::string header2 = "KILLS";
	std::string header3 = "DEATHS";
	std::string header4 = "  KDR";
	std::string header5 = "PLAYED WITH CHAMPION(S)";

	int maxlength = header1.length();
	for(int i = 0; i < pList.size(); i++){
		std::string name = pList[i].getPlayername();
		if(name.length() > maxlength){
			maxlength = name.length();
		}
	}
	maxlength += 5;

	out << header1 << std::string(maxlength - header1.length(), ' ')
		<< spacing << header2 << spacing << header3 << spacing
		<< header4 << spacing << header5 << std::endl;

	for(int i = 0; i < pList.size(); i++){
		std::string name = pList[i].getPlayername();
		std::string kills = std::to_string(pList[i].getKills());
		std::string deaths = std::to_string(pList[i].getDeaths());
		out << name << std::string(maxlength - name.length(), ' ')
			<< spacing << std::string(5 - kills.length(), ' ') << kills
			<< spacing << std::string(6 - deaths.length(), ' ') << deaths
			<< spacing << " " << std::setprecision(2) << std::fixed << pList[i].getKdr()
			<< spacing;
		//pList[i].sortChampsForPrinting();
		std::vector<std::string> champs = pList[i].getChampions();
		sort(champs.begin(), champs.end());
		for(int j = 0; j < champs.size(); j++){
			if(j > 0){ out << ", "; }
			out << champs[j];
		}
		out << std::endl;
	}
}

//=====================================================================

void printChampions(std::ofstream& out, const std::vector<Champion>& cList){
	std::string spacing = "   ";
	std::string header1 = "CHAMPION NAME";
	std::string header2 = " WINS";
	std::string header3 = "LOSSES";
	std::string header4 = " WIN%";
	std::string header5 = "MINION DEATHS";

	int maxlength = header1.length();
	for(int i = 0; i < cList.size(); i++){
		std::string name = cList[i].getChampionname();
		if(name.length() > maxlength){
			maxlength = name.length();
		}
	}
	maxlength += 5;

	out << header1 << std::string(maxlength - header1.length(), ' ')
		<< spacing << header2 << spacing << header3 << spacing
		<< header4 << spacing << header5 << std::endl;

	for(int i = 0; i < cList.size(); i++){
		std::string name = cList[i].getChampionname();
		std::string wins = std::to_string(cList[i].getWins());
		std::string losses = std::to_string(cList[i].getLosses());
		std::string mdeaths = std::to_string(cList[i].getDeathsByMinion());
		out << name << std::string(maxlength - name.length(), ' ')
			<< spacing << std::string(5 - wins.length(), ' ') << wins
			<< spacing << std::string(6 - losses.length(), ' ') << losses
			<< spacing << " " << std::setprecision(2) << std::fixed << cList[i].getWinPct()
			<< spacing << std::string(13 - mdeaths.length(), ' ') << mdeaths;
		out << std::endl;
	}
}

//=====================================================================

void printCustom(std::ofstream& out, const std::vector<Player>& pList){
	std::string spacing = "   ";
	std::string header1 = "PLAYER NAME";
	std::string header2 = "ASSISTS";
	std::string header3 = "KILLS";
	std::string header4 = "  AKR";
	std::string header5 = "PLAYED WITH CHAMPION(S)";

	int maxlength = header1.length();
	for(int i = 0; i < pList.size(); i++){
		std::string name = pList[i].getPlayername();
		if(name.length() > maxlength){
			maxlength = name.length();
		}
	}
	maxlength += 5;

	out << header1 << std::string(maxlength - header1.length(), ' ')
		<< spacing << header2 << spacing << header3 << spacing
		<< header4 << spacing << header5 << std::endl;
	for(int i = 0; i < pList.size(); i++){
		std::string name = pList[i].getPlayername();
		std::string assists = std::to_string(pList[i].getAssists());
		std::string kills = std::to_string(pList[i].getKills());
		out << name << std::string(maxlength - name.length(), ' ')
			<< spacing << std::string(7 - assists.length(), ' ') << assists
			<< spacing << std::string(5 - kills.length(), ' ') << kills
			<< spacing << " " << std::setprecision(2) << std::fixed << pList[i].getAssistsToKills()
			<< spacing;
		//pList[i].sortChampsForPrinting();
		std::vector<std::string> champs = pList[i].getChampions();
		sort(champs.begin(), champs.end());
		for(int j = 0; j < champs.size(); j++){
			if(j > 0){ out << ", "; }
			out << champs[j];
		}
		out << std::endl;
	}
}

//=====================================================================

int main(int argc, char* argv[]) {
	if(argc != 4){
		std::cerr << "Enter 3 arguments after command name.\n";
		exit(1);
	}
	std::ifstream infile;
	infile.open(argv[1]);
	if (!infile.good()){
		std::cerr << "Can't open" << argv[1] << "to read.\n";
		exit(1);
	}
	std::ofstream outfile;
	outfile.open(argv[2]);
	if (!infile.good()){
		std::cerr << "Can't open" << argv[2] << "to right.\n";
		exit(1);
	}

	std::string printType = argv[3];
	std::vector<Player> playerList;
	std::vector<Champion> champList;
	
	std::string s;
	std::string team = "";

	while(infile >> s){ // goes through each line of input
		if(s == "END"){ continue; }
		else if(s == "MATCH"){
			infile >> s;
			infile >> s; //Match ID #
		} else if(s == "LOSING"){
			infile >> s;
			team = "losing";
		} else if(s == "WINNING"){
			infile >> s;
			team = "winning";
		} else if(s == "EVENTS"){
			continue;
		} else if(s == "@"){ //actual event
			parseEvent(infile, s, playerList, champList);
		} else{ //player entry
			std::string user = s;
			infile >> s; //"playing"
			infile >> s; //"champion"
			infile >> s;
			std::string champ = s;
			bool oldplayer = false;
			for(int i = 0; i < playerList.size(); ++i){
				if( user == playerList[i].getPlayername() ){
					playerList[i].addChampion(champ);
					oldplayer = true;
				}
			}
			if(!oldplayer){
				Player newplayer(user);
				newplayer.addChampion(champ);
				playerList.push_back(newplayer);
			}

			bool oldchamp = false;
			for(int i = 0; i < champList.size(); ++i){
				if( champ == champList[i].getChampionname() ){
					if(team == "winning"){ champList[i].addWin(); }
					else if(team == "losing"){ champList[i].addLoss(); }
					oldchamp = true;
				}
			}
			if(!oldchamp){
				Champion newchamp(champ);
				if(team == "winning"){ newchamp.addWin(); }
				else if(team == "losing"){ newchamp.addLoss(); }
				champList.push_back(newchamp);
			}
		}
	} //end of input

	
	
	if(printType == "players"){
		sort(playerList.begin(), playerList.end(), sortPlayers);
		printPlayers(outfile, playerList);
	} else if(printType == "champions"){
		sort(champList.begin(), champList.end(), sortChampions);
		printChampions(outfile, champList);
	} else if(printType == "custom"){
		sort(playerList.begin(), playerList.end(), sortByAssists);
		printCustom(outfile, playerList);
	}

	return 0;
}




















