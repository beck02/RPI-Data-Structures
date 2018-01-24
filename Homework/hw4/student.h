#ifndef Student_h_
#define Student_h_

#include <list>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Student{
public:
	Student(string student_name) : m_name(student_name){}

	const string& GetName() const { return m_name; }
	const string& GetBestOffer() const { return m_best_offer; }
	void PrintStudentDecision(ofstream& ostr) const;
	void PrintStudentPreferenceList(ostream& ostr) const;

	void AddSchool(string school_name);
	void RemoveSchool(string school_name);
	bool IsOfferTentativelyAccepted(string school_name);
	void PrepareToReceiveOffers();
	bool HasOffer() { return (m_best_offer != ""); }

private:
	string m_name;
	string m_best_offer;
	list<string>::iterator m_offer_index;
	list<string> m_prefs;
};

bool alpha_by_student_name(const Student& st1, const Student& st2);

#endif