#include "organism.h"
using namespace std;

//======================================================================================
//======================================================================================

Animal::Animal(map<string, vector<string> > f) : Organism(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("eats:");
	if( itr == f.end() ){ throw 0; }
	itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( (thing == "leaves") || (thing == "roots") || (thing == "branches") ){ throw 0; }
		}
	}
}

//================================================================

Plant::Plant(map<string, vector<string> > f) : Organism(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( (thing == "leaves") || (thing == "roots") || (thing == "branches") ){ return; }
		}
		throw 0;
	}
}

//======================================================================================
//====================------INTERMEDIATE------==========================================
//======================================================================================

LaysEggs::LaysEggs(map<string, vector<string> > f) : Animal(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lays_eggs:");
	if( (itr == f.end()) || (itr->second[0] == "false") ) { throw 0; }
}

//================================================================

Endothermic::Endothermic(map<string, vector<string> > f) : Animal(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("warm_blooded:");
	if( (itr == f.end()) || (itr->second[0] == "false") ) { throw 0; }
}

//================================================================

Ectothermic::Ectothermic(map<string, vector<string> > f) : Animal(f), LaysEggs(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("warm_blooded:");
	if( (itr == f.end()) || (itr->second[0] == "true") ) { throw 0; }
}

//================================================================

Bird::Bird(map<string, vector<string> > f) : Animal(f), LaysEggs(f), Endothermic(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "feathers" ){ return; }
		}
		throw 0;
	}
}

//================================================================

Fish::Fish(map<string, vector<string> > f) : Animal(f), Ectothermic(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "fins" ){ return; }
		}
		throw 0;
	}
}

//================================================================

Reptile::Reptile(map<string, vector<string> > f) : Animal(f), Ectothermic(f) {

}

//================================================================

Mammal::Mammal(map<string, vector<string> > f) : Animal(f), Endothermic(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "fur" || thing == "teeth" ){ return; }
		}
		throw 0;
	}
}

//======================================================================================
//====================------CREATURES------=============================================
//======================================================================================

Trout::Trout(map<string, vector<string> > f) : Animal(f), Fish(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lives_in:");
	if(itr != f.end()){
		if(itr->second.size() != 1 || itr->second[0] != "water"){ throw 0; }
	}
}

//================================================================

Tortoise::Tortoise(map<string, vector<string> > f) : Animal(f), Reptile(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lifespan:");
	if(itr != f.end()){
		if(itr->second[0] != ">100yrs"){ throw 0; }
	}
}

//================================================================

Cardinal::Cardinal(map<string, vector<string> > f) : Animal(f), Bird(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lives_in:");
	if(itr != f.end()){
		if(itr->second.size() != 1 || itr->second[0] != "air"){ throw 0; }
	}
}

//================================================================

Penguin::Penguin(map<string, vector<string> > f) : Animal(f), Bird(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lives_in:");
	if(itr != f.end()){
		
	}
}

//================================================================

Bat::Bat(map<string, vector<string> > f) : Animal(f), Mammal(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "wings" ){ return; }
		}
		throw 0;
	}
}

//================================================================

Bear::Bear(map<string, vector<string> > f) : Animal(f), Mammal(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("eats:");
	if(itr != f.end()){
		bool meat = false; bool veggies = false;
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "deer" || thing == "fish" ){ meat = true; }
			if( thing == "bugs" || thing == "plants" ){ veggies = true; }
			if(meat && veggies){ return; }
		}
		throw 0;
	}
}

//================================================================

Tiger::Tiger(map<string, vector<string> > f) : Animal(f), Mammal(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "stripes" ){ return; }
		}
		throw 0;
	}
}

//================================================================

Rhinoceros::Rhinoceros(map<string, vector<string> > f) : Animal(f), Mammal(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "horn" ){ return; }
		}
		throw 0;
	}
}

//================================================================

Platypus::Platypus(map<string, vector<string> > f) : Animal(f), Mammal(f), LaysEggs(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("has:");
	if(itr != f.end()){
		for(int i = 0; i < itr->second.size(); ++i){
			string thing = itr->second[i];
			if( thing == "bill" ){ return; }
		}
		throw 0;
	}
}

//================================================================

Redwood::Redwood(map<string, vector<string> > f) : Plant(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lifespan:");
	if(itr != f.end()){
		if(itr->second[0] != ">100yrs"){ throw 0; }
	}
}

//================================================================

VenusFlyTrap::VenusFlyTrap(map<string, vector<string> > f) : Plant(f) {
	map<string, vector<string> >::const_iterator itr = facts.find("lifespan:");
	if(itr != f.end()){
		if(itr->second[0] != "<100yrs"){ throw 0; }
	}
}

//======================================================================================
//======================================================================================