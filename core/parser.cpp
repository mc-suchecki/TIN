#include "../include/parser.hpp"
#include "../include/events/consoleEvent.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <vector>
#include <string>

using namespace std;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
using phoenix::push_back;
using qi::_val;
using qi::_1;
using boost::spirit::lit;
using boost::spirit::lexeme;
using boost::phoenix::ref;

Parser::Parser(){

}


//template <typename Iterator>
//bool adder(Iterator first, Iterator last, 
//
/*
   struct events_ : qi::symbols<char, Event*>
   {
   events_()
   {
   add
   ("connect" , new CreateConnectionEvent(""))
   ("send"   , new ConsoleEvent())
   ;
   }
   } events;
   */

ConsoleEvent * Parser::parse(string input){
  bool result;
  string action;
  string address;
  string command;
  int port = 0;
  result = qi::parse(
      input.begin(), input.end(), 
      (
       (
        lit("connect")[ref(action) = "connect"]
        >> " "
        >> qi::as_string[+(qi::char_ - " ")][ref(address) = _1]
        >> *(" " >>
          (qi::int_)[ref(port) = _1])
       )
       | (
         lit("send")[ref(action) = "send"]
         >> " "
         >> qi::as_string[+(qi::char_ - " ")][ref(address) = _1]
         >> " "
         >> qi::as_string[+(qi::char_ - " ")][ref(command) = _1]
         )
       | (
         lit("disconnect")
         )
      )
  );

  if(result){
    if(action == "connect"){
      if(port!=0)
        return new CreateConnectionEvent(address, port);
      else
        return new CreateConnectionEvent(address);
    }
    else if(action == "send"){
      return new SendCommandEvent(address, command);
    }
  }
  else {
    cout << "Unrecognised command!" << endl;
  }
  return NULL;
}
