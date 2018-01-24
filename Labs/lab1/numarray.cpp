#include <iostream>

int main(){
	std::cout << "Please enter an int for # of inputs, then that many float numbers: " << std::endl;
	int numIn;
	std::cin >> numIn;
	float nums[numIn];
	for(int i = 0; i < numIn; i++){
		float n;
		std::cin >> n;
		nums[i] = n;
	}
	std::cout << "Numbers: ";
	for(int i = 0; i < numIn; i++){
		std::cout << nums[i] << ", ";
	}
	std::cout << std::endl;
	float avg;
	float sum = 0;
	for(int i = 0; i < numIn; i++){
		sum += nums[i];
	}
	avg = sum/numIn;
	int numNumsGreater = 0;
	std::cout << "Average: " << avg << std::endl << "Nums > Avg: ";
	for(int i = 0; i < numIn; i++){
		if( nums[i] > avg ){
			std::cout << nums[i] << ", ";
			numNumsGreater++;
		}
	}
	if(numNumsGreater == 0){
		std::cout << "None";
	}
	std::cout << std::endl;
	return 0;
}