// File: student.h
// Purpose: Header for declaration of student record class and associated functions.
#ifndef __student_h_
#define __student_h_

#include <iostream>
#include <string>
#include <vector>

class Student {
public:
	// ACCESSORS
	const std::string& first_name() const { return first_name_; }
	const std::string& last_name() const { return last_name_; }
	const std::string& id_number() const { return id_number_; }
	double hw_avg() const { return hw_avg_; }
	double test_avg() const { return test_avg_; }
	double final_avg() const { return final_avg_; }
	
	bool read(std::istream& in_str, unsigned int num_homeworks, unsigned int num_tests);
	void compute_averages(double hw_weight);
	std::ostream& output_name(std::ostream& out_str) const;
	std::ostream& output_averages(std::ostream& out_str) const;

private: // REPRESENTATION
	std::string first_name_;
	std::string last_name_;
	std::string id_number_;
	std::vector<int> hw_scores_;
	double hw_avg_;
	std::vector<int> test_scores_;
	double test_avg_;
	double final_avg_;
};

bool less_names(const Student& stu1, const Student& stu2);

#endif
