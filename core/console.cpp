#include <iostream>
#include "../include/console.hpp"
#include "../include/parser.hpp"
#include "../include/config.hpp"
#include "../include/events/consoleEvent.hpp"

using namespace std;

Console::Console(EventQueue * queue) : queue(queue) {
  parser = new Parser();
}

void Console::run() {
  Config * config = Config::getInstance();
  if(config->getScriptFile() != "")
    executeLine("run "+config->getScriptFile());

  while(true) {
    string line;
    cout << "? ";
    getline(cin, line);
    executeLine(line);
    sleep(1);
  }
}

void Console::executeLine(string line){
    history.push_back(line);
    vector<ConsoleEvent*> events = parser->parse(line);
    if(events.size()>0)
      for(unsigned int i = 0; i < events.size(); ++i)
        queue->push(events[i]);
}
