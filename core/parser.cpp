#include "../include/parser.hpp"
#include "../include/events/consoleEvent.hpp"

using namespace std;

Parser::Parser(){

}

ConsoleEvent Parser::parse(string input){
  return ConsoleEvent("tekst "+input);
}
