#include <iostream>
#include "../include/console.hpp"
#include "../include/parser.hpp"
#include "../include/events/consoleEvent.hpp"

using namespace std;

Console::Console(EventQueue * queue):queue(queue){
  parser = new Parser();
}

void Console::run(){
  while(true){
    string line;

    cout<<"? ";
    getline(cin,line);
    ConsoleEvent * event = parser->parse(line);
    cout<< event.getMessage()<<endl;
    queue->push(event);
  }
}

#ifdef konsola
int main(){
  Console * cons = new Console();
  cons->run();
  return 0;
}
#endif
