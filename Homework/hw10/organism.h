#ifndef _ORGANISM_H_
#define _ORGANISM_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

//======================================================================================
//======================================================================================

class Organism {
public:
	Organism(map<string, vector<string> > f) : facts(f) {}
	virtual ~Organism() {}
	string getName() const { return facts.find("name:")->second[0]; }
	virtual bool isCarnivore(){ return true; }
	virtual bool isHerbivore(){ return false; }
	virtual bool isOmnivore(){ return false; }
	virtual bool livesInAir(){ return false; }
	virtual bool livesInWater(){ return false; }
	virtual bool livesOnLand(){ return true; }

protected:
	map<string, vector<string> > facts;
};

//================================================================

class Animal : public Organism {
public:
	Animal(map<string, vector<string> > f);// : Organism(f) {}
};

//================================================================

class Plant : public Organism {
public:
	Plant(map<string, vector<string> > f);// : Organism(f) {}
};


//======================================================================================
//====================------INTERMEDIATE------==========================================
//======================================================================================


class LaysEggs : public virtual Animal {
public:
	LaysEggs(map<string, vector<string> > f);// : Animal(f) {}
};

//================================================================

class Endothermic : public virtual Animal {
public:
	Endothermic(map<string, vector<string> > f);// : Animal(f) {}
};

//================================================================

class Ectothermic : public LaysEggs {
public:
	Ectothermic(map<string, vector<string> > f);// : Animal(f), LaysEggs(f) {}
};

//================================================================

class Bird : public LaysEggs, public Endothermic {
public:
	Bird(map<string, vector<string> > f);// : Animal(f), LaysEggs(f), Endothermic(f) {}
};

//================================================================

class Fish : public Ectothermic {
public:
	Fish(map<string, vector<string> > f);// : Animal(f), Ectothermic(f) {}
	bool livesInWater(){ return true; }
	bool livesOnLand(){ return false; }
};

//================================================================

class Reptile : public Ectothermic {
public:
	Reptile(map<string, vector<string> > f);// : Animal(f), Ectothermic(f) {}
};

//================================================================

class Mammal : public Endothermic {
public:
	Mammal(map<string, vector<string> > f);// : Animal(f), Endothermic(f) {}
};


//======================================================================================
//====================------CREATURES------=============================================
//======================================================================================


class Trout : public Fish {
public:
	Trout(map<string, vector<string> > f);// : Animal(f), Fish(f) {}
};

//================================================================

class Tortoise : public Reptile {
public:
	Tortoise(map<string, vector<string> > f);// : Animal(f), Reptile(f) {}
	bool isCarnivore(){ return false; }
	bool isHerbivore(){ return true; }
};

//================================================================

class Cardinal : public Bird {
public:
	Cardinal(map<string, vector<string> > f);// : Animal(f), Bird(f) {}
	bool isCarnivore(){ return false; }
	bool isHerbivore(){ return true; }
	bool livesInAir(){ return true; }
	bool livesOnLand(){ return false; }
};


//================================================================

class Penguin : public Bird {
public:
	Penguin(map<string, vector<string> > f);// : Animal(f), Bird(f) {}
	bool livesInWater(){ return true; }
};


//================================================================

class Bat : public Mammal {
public:
	Bat(map<string, vector<string> > f);// : Animal(f), Mammal(f) {}
	bool livesInAir(){ return true; }
};


//================================================================

class Bear : public Mammal {
public:
	Bear(map<string, vector<string> > f);// : Animal(f), Mammal(f) {}
	bool isCarnivore(){ return false; }
	bool isOmnivore(){ return true; }
};


//================================================================

class Tiger : public Mammal {
public:
	Tiger(map<string, vector<string> > f);// : Animal(f), Mammal(f) {}
};


//================================================================

class Rhinoceros : public Mammal {
public:
	Rhinoceros(map<string, vector<string> > f);// : Animal(f), Mammal(f) {}
	bool isCarnivore(){ return false; }
	bool isHerbivore(){ return true; }
};

//================================================================

class Platypus : public Mammal, public LaysEggs {
public:
	Platypus(map<string, vector<string> > f);// : Animal(f), Mammal(f), LaysEggs(f) {}
	bool livesInWater(){ return true; }
};

//================================================================

class Redwood : public Plant {
public:
	Redwood(map<string, vector<string> > f);// : Plant(f) {}
	bool isCarnivore(){ return false; }
};

//================================================================

class VenusFlyTrap : public Plant {
public:
	VenusFlyTrap(map<string, vector<string> > f);// : Plant(f) {}
};

//======================================================================================
//======================================================================================

#endif