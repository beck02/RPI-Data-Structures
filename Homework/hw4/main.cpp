#include <iostream>
#include <fstream>
#include <list>
#include <cassert>
#include <cstdlib>

#include "school.h"
#include "student.h"

// ==========================================================================================
// IMPORTANT: Carefully read this file, but do not make any changes to
//            the code, except to fill in code where indicated, and 
//            (if necessary) add appropriate error messages.
// ==========================================================================================

// read the data for a school & add to the schools list
void AddSchool(std::istream &istr, std::list<School> &schools) {
  std::string school_name, student_name;
  int num_slots, num_prefs;
  istr >> school_name >> num_slots >> num_prefs;
  // create the school
  School s(school_name, num_slots);
  for (int i = 0; i < num_prefs; i++) {
    istr >> student_name;
    // add the students in the order listed
    s.AddStudent(student_name);
  }
  // add to the schools list
  for(std::list<School>::iterator itr = schools.begin(); itr != schools.end(); itr++){
    if ( (itr->GetName()) == school_name  ){
      std::cerr << "WARNING: could not add school named " << school_name
                << ", this school already exists" << std::endl;
      return;
    }
  }
  schools.push_back(s);
}

// read the data for a student & add to the students list
void AddStudent(std::istream &istr, std::list<Student> &students) {
  std::string student_name, school_name;
  int num_prefs;
  istr >> student_name >> num_prefs;
  // Create the student
  Student s(student_name);
  for (int i = 0; i < num_prefs; i++) {
    istr >> school_name;
    // add the schools in the order listed
    s.AddSchool(school_name);
  }
  // add to the students list
  for(std::list<Student>::iterator itr = students.begin(); itr != students.end(); itr++){
    if ( (itr->GetName()) == student_name  ){
      std::cerr << "WARNING: could not add student named " << student_name
                << ", this student already exists" << std::endl;
      return;
    }
  }
  students.push_back(s);
}

// ==========================================================================================

// read data and add a student into a school's preference list
void InsertStudentIntoSchoolPreferenceList(std::istream &istr, std::list<School> &schools) {
  std::string new_student_name, school_name, insert_before_student_name;
  istr >> school_name >> new_student_name >> insert_before_student_name;
  for(std::list<School>::iterator itr = schools.begin(); itr != schools.end(); itr++){
    if ( (itr->GetName()) == school_name  ){
      itr->InsertStudent(new_student_name, insert_before_student_name);
      return;
    }
  }
  std::cerr << "WARNING: could not insert student into school preference list,"
        << " no school named " << school_name << std::endl;
}

// read data and remove a particular school from a student's preference list
void RemoveSchoolFromStudentPreferenceList(std::istream &istr, std::list<Student> &students) {
  std::string school_name, student_name;
  istr >> student_name >> school_name;
  for(std::list<Student>::iterator itr = students.begin(); itr != students.end(); itr++){
    if ( (itr->GetName()) == student_name  ){
      itr->RemoveSchool(school_name);
      return;
    }
  }
  std::cerr << "WARNING: could not remove school from student preference list,"
        << " no student named " << student_name << std::endl;
}

// ==========================================================================================
// The Gale-Shapley algorithm to solve the Stable Marriage Problem

void PerformMatching(std::list<School> &schools, std::list<Student> &students, std::ofstream &ostr) {
  // Perform any initialization necessary for the school and student objects
  for (std::list<School>::iterator itr = schools.begin(); itr != schools.end(); itr++) {
    itr->PrepareToMakeOffers(); 
  }
  for (std::list<Student>::iterator itr = students.begin(); itr != students.end(); itr++) {
    itr->PrepareToReceiveOffers();
  }

  // Iterate until the matching is stable (no changes)
  int which_iteration = 1;
  while(1) {
    ostr << "---- ROUND " << which_iteration << " ----" << std::endl;
    which_iteration++;
    // keep track of the offers made during this round
    int offers_made = 0;
    
    for (std::list<School>::iterator school_itr = schools.begin(); school_itr != schools.end(); school_itr++) {
      if (school_itr->NumAcceptedStudents() == school_itr->MaxAcceptedStudents()) continue;

      // Each school that has open slots, makes an offer to the next student on their preference list
      std::string student = school_itr->MakeNextOffer();
      // an empty string indicates that this school has reached the end of its preference list
      if (student == "") continue;
      ostr << school_itr->GetName() << " makes an offer to " << student << std::endl;
      offers_made++;
      bool found_student = false;
      for (std::list<Student>::iterator student_itr = students.begin(); student_itr != students.end(); student_itr++) {
        // loop through the students to find the student object with that name
        if (student_itr->GetName() == student) {
          found_student = true;
      	  // check to see if that student has tentatively accepted an offer from another school
      	  std::string current_choice = "";
      	  if (student_itr->HasOffer()) { 
      	    current_choice = student_itr->GetBestOffer(); 
      	  }
      	  // make the offer to the student, returns true if the school is on the student's 
      	  // preference list and is better than the students current offer (if any)
      	  bool tentative_acceptance = student_itr->IsOfferTentativelyAccepted(school_itr->GetName());
      	  if (!tentative_acceptance) {
      	    ostr << "  " << student << " declines offer from " << school_itr->GetName() << std::endl;
      	  } else {
      	    if (current_choice != "") {
      	      // if the student had a prior acceptance, decline the earlier offer
      	      for (std::list<School>::iterator school2_itr = schools.begin(); school2_itr != schools.end(); school2_itr++) {
      		      if (school2_itr->GetName() == current_choice) {
            		  ostr << "  " << student << " withdraws tentative acceptance of offer from " << current_choice << std::endl;		
            		  school2_itr->StudentDeclinesTentativeAcceptance(student);
            		}//end if 
      	      }//end for school2_itr
      	    }//end if has current choice
      	    // student tentatively accepts the offer from the (new) school
      	    school_itr->StudentTentativelyAcceptsOffer(student);
      	    ostr << "  " << student << " tentatively accepts offer from " << school_itr->GetName() << std::endl;
      	  }//end else
        }//end if student
      }//end for find student
      if(!found_student){
        std::cerr << "WARNING: could not make offer to student " << student << ", that student does not exist" << std::endl;
      }
    }//end for each school
    if (offers_made == 0) {
      ostr << "no offers_made this round" << std::endl << std::endl;
      break;
    }
  }
}

// ==========================================================================================
// PRINTING

// Print the ranking of students by a particular school
void PrintSchoolPreferenceList(std::istream &istr, const std::list<School> &schools, std::ostream &ostr) {
  std::string school_name;
  istr >> school_name;
  for(std::list<School>::const_iterator itr = schools.begin(); itr != schools.end(); itr++){
    if ( (itr->GetName()) == school_name  ){
      itr->PrintSchoolPreferenceList(ostr);
      ostr << std::endl;
      return;
    }
  }
  std::cerr << "WARNING: could not print student preference list for " << school_name
        << ", that school does not exist" << std::endl;
}

// Print a ranking of schools by a particular student
void PrintStudentPreferenceList(std::istream &istr, const std::list<Student> &students, std::ostream &ostr) {
  std::string student_name;
  istr >> student_name;
  for(std::list<Student>::const_iterator itr = students.begin(); itr != students.end(); itr++){
    if ( (itr->GetName()) == student_name  ){
      itr->PrintStudentPreferenceList(ostr);
      ostr << std::endl;
      return;
    }
  }
  std::cerr << "WARNING: could not print school preference list for " << student_name
        << ", that student does not exist" << std::endl;
}

// Print the list of all schools (sorted alphabetically) and the students enrolled (also alphabetically)
void PrintSchoolEnrollments(std::list<School> &schools, std::ofstream &ostr) {
  schools.sort(alpha_by_school_name);
  for (std::list<School>::iterator itr = schools.begin(); itr != schools.end(); itr++) {
    itr->PrintSchoolEnrollment(ostr);
  }
  ostr << std::endl;
}

// Print a list of the students (sorted alphabetically) and the school they will attend (if any)
void PrintStudentDecisions(std::list<Student> &students, std::ofstream &ostr) {
  students.sort(alpha_by_student_name);
  for (std::list<Student>::const_iterator itr = students.begin(); itr != students.end(); itr++) {
    itr->PrintStudentDecision(ostr);
  }
  ostr << std::endl;
}

// ==========================================================================================

int main(int argc, char* argv[]) {

  // process command line arguments
  if (argc != 3) {
    std::cerr << "ERROR: incorrect number of command line arguments" << std::endl;
    std::cerr << "USAGE:   " << argv[0] << " <input_file> <output_file>" << std::endl;
    exit(1);
  }
  // open the input file for parsing
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "ERROR: could not open input file " << argv[1] << std::endl;
    exit(1);
  }
  // open the output file
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: could not open output file " << argv[2] << std::endl;
    exit(1);
  }

  // lists to store school and student objects
  std::list<School> schools;
  std::list<Student> students;

  // parse the file
  std::string keyword;
  while(istr >> keyword) {
    if (keyword == std::string("add_school")) {
      AddSchool(istr,schools);
    } else if (keyword == std::string("insert_student_into_school_preference_list")) {
      InsertStudentIntoSchoolPreferenceList(istr,schools);
    } else if (keyword == std::string("print_school_preferences")) {
      PrintSchoolPreferenceList(istr,schools,ostr);
    }

    else if (keyword == std::string("add_student")) { 
      AddStudent(istr,students);
    } else if (keyword == std::string("remove_school_from_student_preference_list")) { 
      RemoveSchoolFromStudentPreferenceList(istr,students);
    } else if (keyword == std::string("print_student_preferences")) {
      PrintStudentPreferenceList(istr,students,ostr);
    } 

    else if (keyword == std::string("perform_matching")) {
      PerformMatching(schools,students,ostr);
    } else if (keyword == std::string("print_school_enrollments")) {
      PrintSchoolEnrollments(schools,ostr);
    } else if (keyword == std::string("print_student_decisions")) {
      PrintStudentDecisions(students,ostr);
    } else {
      std::cerr << "unknown keyword in input file: " << keyword << std::endl;
      exit(1);
    }
  }
}

// ==========================================================================================
