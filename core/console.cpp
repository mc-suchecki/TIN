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
    ConsoleEvent *event = parser->parse(line);
    if(event)
      queue->push(event);
    sleep(1);
  }
}
