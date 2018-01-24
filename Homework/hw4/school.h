#ifndef School_h_
#define School_h_

#include <list>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class School{
public:
	School(string school_name, int num_slots)
		: m_name(school_name), m_slots(num_slots), m_num_accepted(0) {}

	const string& GetName() const { return m_name; }
	const int MaxAcceptedStudents() const { return m_slots; }
	const int NumAcceptedStudents() const { return m_num_accepted; }
	void PrintSchoolEnrollment(ofstream& ostr);
	void PrintSchoolPreferenceList(ostream& ostr) const;

	void AddStudent(string student_name);
	void InsertStudent(string new_student_name, string insert_before_student_name);
	void StudentDeclinesTentativeAcceptance(string student_name);
	void StudentTentativelyAcceptsOffer(string student_name);
	string MakeNextOffer();
	void PrepareToMakeOffers();

private:
	string m_name;
	list<string> m_prefs;
	list<string> m_enrolled;
	list<string>::iterator m_offer_itr;
	int m_slots;
	int m_num_accepted;
};

bool alpha_by_school_name(const School& sch1, const School& sch2);

#endif