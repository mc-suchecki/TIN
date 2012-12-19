#include <iostream>
#include "../include/console.hpp"
#include "../include/parser.hpp"

using namespace std;

Console::Console(EventQueue * queue){
  parser = new Parser();
}

void Console::run(){
  while(true){
    string line;

    cout<<"? "<<endl;
    cin>>line;
    parser->parse(line);
  }
}

#ifdef konsola
int main(){
  Console * cons = new Console();
  cons->run();
  return 0;
}
#endif
