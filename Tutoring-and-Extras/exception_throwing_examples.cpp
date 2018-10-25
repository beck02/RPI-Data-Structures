

void main(){
	stuff
	stuff
	stuff

	// try{
		func1();
	// } catch(int n){}

	more stuff
	more stuff
}

void func1(){
	blah
	blah
	blah

	func2();

	more blah
	more blah
	more blah
}



void func2(){
	wheee
	wheee
	wheee

	// try{
		func3();
	// }catch(...){}

	more wheee
	more wheee
	more wheee
}

void func3(){
	wooooo

	throw -1;

	more wooooo
}


int my_func(int a, int b) throw(double, bool){
	if(a > b){
		throw 20.3;
	} else if(a == b){
		throw "they're equal";
	} else{
		throw false;
	}
}
















