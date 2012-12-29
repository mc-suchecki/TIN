#include <iostream>
#include "../include/console.hpp"
#include "../include/parser.hpp"
#include "../include/events/consoleEvent.hpp"

using namespace std;

Console::Console(EventQueue * queue) : queue(queue) {
  parser = new Parser();
}

void Console::run() {
  while(true) {
    string line;
    cout << "? ";
    getline(cin, line);
    history.push_back(line);
    vector<ConsoleEvent*> events = parser->parse(line);
    if(events.size()>0)
      for(unsigned int i = 0; i < events.size(); ++i)
        queue->push(events[i]);

    sleep(1);
  }
}
