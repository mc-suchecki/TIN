#include <iostream>
#include "../include/console.hpp"

using namespace std;

Console::Console(){
	
}

void Console::run(){
	while(true){
		string line;
		cin>>line;
		cout<<line<<endl;
	}
}

#ifdef konsola
int main(){
	Console * cons = new Console();
	cons->run();
	return 0;
}
#endif