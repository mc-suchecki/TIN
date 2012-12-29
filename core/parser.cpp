#include "../include/parser.hpp"
#include "../include/events/consoleEvent.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <vector>
#include <string>
#include <fstream> 

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


vector<ConsoleEvent*> Parser::parse(string input){
  vector<ConsoleEvent*> retVector;
  bool result;
  string action;
  vector<string> addresses;
  vector<string> files;
  string command;
  int port = 0;
  result = qi::parse(
      input.begin(), input.end(), 
      (
       (
        lit("connect")[ref(action) = "connect"]
        >> " "
        >> qi::as_string[+(qi::char_ - " " - ",")][push_back(ref(addresses), _1)]
        >> *(
          ", "
          >> qi::as_string[+(qi::char_ - " " - ",")][push_back(ref(addresses),_1)]
          )
        >> *(" " >>
          (qi::int_)[ref(port) = _1])
       )
       | (
         lit("send")[ref(action) = "send"]
         >> " "
         >> qi::as_string[+(qi::char_ - " " - ",")][push_back(ref(addresses),_1)]
         >> *(
           ", "
           >> qi::as_string[+(qi::char_ - " " - ",")][push_back(ref(addresses),_1)]
           )
         >> " "
         >> qi::as_string[+(qi::char_ - " ")][ref(command) = _1]
         )
       | (
         lit("disconnect")
         )
       | (
         lit("sendf")[ref(action) = "sendf"]
         >> " "
         >> qi::as_string[+(qi::char_ - " " - ",")][push_back(ref(addresses),_1)]
         >> *(
           ", "
           >> qi::as_string[+(qi::char_ - " " - ",")][push_back(ref(addresses),_1)]
           )
           >> *(
             " "
             >> qi::as_string[+(qi::char_ - " ")][push_back(ref(files), _1)]
             )
         )
       )
       );

  if(result){
    if(action == "connect"){
      for(unsigned int i = 0; i < addresses.size();++i)
        retVector.push_back(new CreateConnectionEvent(addresses[i], port));
    }
    else if(action == "send"){
      for(unsigned int i = 0; i < addresses.size();++i)
        retVector.push_back(new SendCommandEvent(addresses[i], command));
    }
    else if(action == "sendf"){
      ifstream inputFile;

      for(unsigned int i = 0; i < files.size(); ++i){
        inputFile.open(files[i].c_str());
        if(!inputFile){
          cout<<"Unable to open file!"<<endl;
          continue;
        }
        string command;
        while(inputFile >> command){
          for(unsigned int i = 0; i < addresses.size();++i)
            retVector.push_back(new SendCommandEvent(addresses[i], command));
        }

        inputFile.close();
      }
    }
  }
  else {
    cout << "Unrecognised command!" << endl;
  }
  return retVector;
}
