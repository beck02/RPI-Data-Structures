// File: student.cpp
// Purpose: Implementation of the class Student

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "student.h"
#include "std_dev.h"

// Read information about a student, returning true if the information was read correctly.
bool Student::read(std::istream& in_str, unsigned int num_homeworks, unsigned int num_tests) {
	
	// If we don't find an id, we've reached the end of the file & silently return false.
	if (!(in_str >> id_number_)) return false;
	// Once we have an id number, any other failure in reading is treated as an error.
	
	// read the name
	if (! (in_str >> first_name_ >> last_name_)) {
		std::cerr << "Failed reading name for student " << id_number_ << std::endl;
		return false;
	}
	
	unsigned int i;
	int score;
	
	// Read the homework scores
	hw_scores_.clear();
	for (i=0; i<num_homeworks && (in_str >> score); ++i)
		hw_scores_.push_back(score);
	if (hw_scores_.size() != num_homeworks) {
		std::cerr << "Pre-mature end of file or invalid input reading "
			<< "hw scores for " << id_number_ << std::endl;
		return false;
	}
	
	// Read the test scores
	test_scores_.clear();
	for (i=0; i<num_tests && (in_str >> score); ++i)
		test_scores_.push_back(score);
	if (test_scores_.size() != num_tests) {
		std::cerr << "Pre-mature end of file or invalid input reading "
			<< "test scores for" << id_number_ << std::endl;
		return false;
	}
	return true; // everything was fine
}


// Compute and store the hw, test and final average for the student.
void Student::compute_averages(double hw_weight) {
	double dummy_stddev;
	avg_and_std_dev(hw_scores_, hw_avg_, dummy_stddev);
	avg_and_std_dev(test_scores_, test_avg_, dummy_stddev);
	final_avg_ = hw_weight * hw_avg_ + (1 - hw_weight) * test_avg_;
}

std::ostream& Student::output_name(std::ostream& out_str) const {
	out_str << last_name_ << ", " << first_name_;
	return out_str;
}

std::ostream& Student::output_averages(std::ostream& out_str) const {
	out_str << std::fixed << std::setprecision(1);
	out_str << hw_avg_ << " " << test_avg_ << " " << final_avg_ << std::endl;
	return out_str;
}

// Boolean function to define alphabetical ordering of names. The vector sort
// function requires that the objects be passed by CONSTANT REFERENCE.
bool less_names(const Student& stu1, const Student& stu2) {
	return stu1.last_name() < stu2.last_name() ||
		(stu1.last_name() == stu2.last_name() && stu1.first_name() < stu2.first_name());
}
