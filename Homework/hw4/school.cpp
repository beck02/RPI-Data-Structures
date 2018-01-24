#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "school.h"
using namespace std;

/*
string m_name;
list<string> m_prefs;
list<string> m_enrolled;
list<string>::iterator m_offer_itr;
int m_slots;
int m_num_accepted;
*/

void School::PrintSchoolEnrollment(ofstream& ostr) {
	ostr << "student(s) who will be attending " << m_name << ":" << endl;
	m_enrolled.sort();
	for (list<string>::const_iterator itr = m_enrolled.begin(); itr != m_enrolled.end(); ++itr){
		ostr << "  " << *itr << endl;
	}
	if(m_num_accepted < m_slots){
		ostr << "  [" << m_slots - m_num_accepted << " remaining slot(s) in enrollment]" << endl;
	}
}

void School::PrintSchoolPreferenceList(ostream& ostr) const {
	ostr << m_name << " preference list:" << endl;
	int count = 1;
	for (list<string>::const_iterator itr = m_prefs.begin(); itr != m_prefs.end(); ++itr, ++count){
		if (count < 10){
			ostr << "  " << count << ". ";
		} else if( count > 9 && count < 100){
			ostr << " " << count << ". ";
		} else if(count > 99){
			ostr << count << ". ";
		}
		ostr << *itr << endl;
	}
}

void School::AddStudent(string student_name){
	for(list<string>::iterator itr = m_prefs.begin(); itr != m_prefs.end(); ++itr){
		if( *itr == student_name ){
			cerr << "WARNING: could not add " << student_name << " into school preference list,"
					<< " this student is already in the list" << endl;
			return;
		}
	}
	m_prefs.push_back(student_name);
}

void School::InsertStudent(string new_student_name, string insert_before_student_name){
	bool new_st_in_list = false;
	bool st_bef_in_list = false;
	list<string>::iterator before_spot;
	list<string>::iterator n_st_old_place;
	for(list<string>::iterator itr = m_prefs.begin(); itr != m_prefs.end(); ++itr){
		if( *itr == new_student_name ){
			new_st_in_list = true;
			n_st_old_place = itr;
		}
		if( *itr == insert_before_student_name ){
			st_bef_in_list = true;
			before_spot = itr;
		}
	}
	if( st_bef_in_list ){
		if(new_st_in_list){
			m_prefs.erase(n_st_old_place);
		}
		m_prefs.insert(before_spot, new_student_name);
	} else{
		cerr << "WARNING: could not insert new student into school preference list before "
			<< insert_before_student_name << ", this student is not in the list" << endl;
		return;
	}
	
}

void School::PrepareToMakeOffers(){
	m_num_accepted = 0;
	m_enrolled.clear();
	m_offer_itr = m_prefs.begin();
}

string School::MakeNextOffer(){
	string offer;
	if( m_offer_itr != m_prefs.end() ){
		offer = *m_offer_itr;
		++m_offer_itr;
	} else{
		offer = "";
	}
	return offer;
}

void School::StudentDeclinesTentativeAcceptance(string student_name){
	for(list<string>::iterator itr = m_enrolled.begin(); itr != m_enrolled.end(); ++itr){
		if( *itr == student_name ){
			m_enrolled.erase(itr);
			m_num_accepted--;
			break;
		}
	}
}

void School::StudentTentativelyAcceptsOffer(string student_name){
	m_enrolled.push_back(student_name);
	m_num_accepted++;
}

bool alpha_by_school_name(const School& sch1, const School& sch2){
	return sch1.GetName() < sch2.GetName();
}

