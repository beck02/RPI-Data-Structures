#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include "battleship.h"

std::string Battleship::get_ship_string(){
	if(get_name() == "submarine"){
		return "o";
	}
	else if(get_name() == "destroyer"){
		if(get_orientation() == "horizontal"){
			return "<>";
		}
		else{
			return "V^";
		}	
	}
	else if(get_name() == "cruiser"){
		if(get_orientation() == "horizontal"){
			return "<X>";
		}
		else{
			return "VX^";
		}
	}
	else if (get_name() == "b.get_name()"){
		if(get_orientation() == "horizontal"){
			return "<BB>";
		}
		else{
			return "VBB^";
		}
	}
	else if(get_name() == "carrier"){
		if(get_orientation() == "horizontal"){
			return "<CCC>";
		}
		else{
			return "VCCC^";
		}
	}
	else if(get_name() == "cargo"){
		if(get_orientation() == "horizontal"){
			return "<AAAA>";
		}
		else{
			return "VAAAA^";
		}
	}
	else if(get_name() == "tanker"){
		if(get_orientation() == "horizontal"){
			return "<TTTTT>";
		}
		else{
			return "VTTTTT^";
		}
	}
	else{
		return "fuck";
		std::cerr << "error" << std::endl;
	}
}