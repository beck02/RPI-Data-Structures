#include "superhero.h"

ostream& operator<<(ostream& os, const Superhero& hero){
	if(hero.isGood()){ os << "Superhero "; }
	else{ os << "Supervillain "; }
	os << hero.getName() << " has power " << hero.getPower() << endl;
	return os;
}

bool operator>(const Superhero& a, const Superhero& b){
	string a_power = a.getPower();
	string b_power = b.getPower();
	if(a_power == b_power){ return false; }
	if(a_power == "Speed"){ return true; }
	if(b_power == "Speed"){ return false; }
	if(a_power == "Flexible"){
		//Flexible
		if(b_power == "Flying"){ return false; }
		else if(b_power == "Invisible"){ return true; }
		else if(b_power == "Laser"){ return false; }
		else if(b_power == "Fire"){ return false; }
		else if(b_power == "Wood"){ return true; }
		else if(b_power == "Water"){ return false; }
		//End Flexible
	} else if(a_power == "Flying"){
		//Flying
		if(b_power == "Invisible"){ return true; }
		else if(b_power == "Laser"){ return false; }
		else if(b_power == "Fire"){ return true; }
		else if(b_power == "Wood"){ return false; }
		else if(b_power == "Water"){ return true; }
		else{ return !(b > a); }
		//End Flying
	} else if(a_power == "Invisible"){
		//Invisible
		if(b_power == "Laser"){ return true; }
		else if(b_power == "Fire"){ return false; }
		else if(b_power == "Wood"){ return true; }
		else if(b_power == "Water"){ return false; }
		else{ return !(b > a); }
		//End Invisible
	} else if(a_power == "Laser"){
		//Laser
		if(b_power == "Fire"){ return false; }
		else if(b_power == "Wood"){ return true; }
		else if(b_power == "Water"){ return true; }
		else{ return !(b > a); }
		//End Laser
	} else if(a_power == "Fire"){
		//Fire
		if(b_power == "Wood"){ return true; }
		else if(b_power == "Water"){ return false; }
		else{ return !(b > a); }
		//End Fire
	} else if(a_power == "Wood"){
		//Wood
		if(b_power == "Water"){ return true; }
		else{ return !(b > a); }
		//End Wood
	} else if(a_power == "Water"){ return !(b > a); }
	

	return false;

}





