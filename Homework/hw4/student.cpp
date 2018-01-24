#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "student.h"
using namespace std;

/*
string m_name;
string m_best_offer;
list<string>::iterator m_offer_index;
list<string> m_prefs;
*/

void Student::PrintStudentDecision(ofstream& ostr) const{
	if(m_best_offer == ""){
		ostr << m_name << " has not received an acceptable offer" << endl;
	} else{
		ostr << m_name << " will be attending " << m_best_offer << endl;
	}
}

void Student::PrintStudentPreferenceList(ostream& ostr) const{
	ostr << m_name << " preference list:" << endl;
	int count = 1;
	for (list<string>::const_iterator itr = m_prefs.begin(); itr != m_prefs.end(); ++itr, ++count){
		ostr << "  " << count << ". " << *itr << endl;
	}
}

void Student::AddSchool(string school_name){
	for(list<string>::iterator itr = m_prefs.begin(); itr != m_prefs.end(); ++itr){
		if( *itr == school_name ){
			cerr << "WARNING: could not add " << school_name << " into student preference list,"
					<< " this school is already in the list" << endl;
			return;
		}
	}
	m_prefs.push_back(school_name);
}

void Student::RemoveSchool(string school_name){
	for(list<string>::iterator itr = m_prefs.begin(); itr != m_prefs.end(); ++itr){
		if( *itr == school_name ){
			m_prefs.erase(itr);
			break;
		}
	}
}

void Student::PrepareToReceiveOffers(){
	m_best_offer = "";
	m_offer_index = m_prefs.end();
}

bool Student::IsOfferTentativelyAccepted(string school_name){
	//go through preference list checking each school up until the current best offer 
	//to ensure offer is only accepted if new school is preferred to previously accepted one
	for(list<string>::iterator itr = m_prefs.begin(); itr != m_offer_index; ++itr){
		if( *itr == school_name ){
			m_best_offer = *itr;
			m_offer_index = itr;
			return true;
		}
	}
	return false;
}


bool alpha_by_student_name(const Student& st1, const Student& st2){
	return st1.GetName() < st2.GetName();
}



