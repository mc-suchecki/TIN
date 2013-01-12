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
  vector<string> command_aliases;
  vector<string> addresses;
  vector<string> files;
  vector<int> ports;
  string command;
  result = qi::parse(
      input.begin(), input.end(), 
      (
       (
        lit("connect")[phoenix::ref(action) = "connect"]
        >> " "
        >> qi::as_string[+(qi::char_ - " ")][push_back(phoenix::ref(addresses), _1)]
        >> " "
        >> qi::as_string[+(qi::char_ - " ")][push_back(phoenix::ref(command_aliases),_1)]
        >> " "
        >> (qi::int_)[push_back(phoenix::ref(ports), _1)]
        >> *(
          ", "
        >> qi::as_string[+(qi::char_ - " ")][push_back(phoenix::ref(addresses), _1)]
        >> " "
        >> qi::as_string[+(qi::char_ - " ")][push_back(phoenix::ref(command_aliases),_1)]
        >> " "
        >> (qi::int_)[push_back(phoenix::ref(ports), _1)]
       )
      )
      
       | (
         lit("send")[phoenix::ref(action) = "send"]
         >> " "
         >> qi::as_string[+(qi::char_ - " " - ",")][push_back(phoenix::ref(command_aliases),_1)]
         >> *(
           ", "
           >> qi::as_string[+(qi::char_ - " " - ",")][push_back(phoenix::ref(command_aliases),_1)]
           )
         >> " "
         >> qi::as_string[+(qi::char_)][phoenix::ref(command) = _1]
         )
       | (
         lit("exit")[phoenix::ref(action) = "exit"]
         )
       | (
         lit("help")[phoenix::ref(action) = "help"]
         )
       | (
         lit("sendf")[phoenix::ref(action) = "sendf"]
         >> " "
         >> qi::as_string[+(qi::char_ - " " - ",")][push_back(phoenix::ref(addresses),_1)]
         >> *(
           ", "
           >> qi::as_string[+(qi::char_ - " " - ",")][push_back(phoenix::ref(addresses),_1)]
           )
           >> *(
             " "
             >> qi::as_string[+(qi::char_ - " ")][push_back(phoenix::ref(files), _1)]
             )
         )
       )
       );

  if(result){
    if(action == "connect"){
      for(unsigned int i = 0; i < addresses.size();++i){
        retVector.push_back(new CreateConnectionEvent(addresses[i], ports[i]));
        aliases[command_aliases[i]]=addresses[i];
      }
    }
    else if(action == "send"){
      for(unsigned int i = 0; i < command_aliases.size();++i)
        retVector.push_back(new SendCommandEvent(aliases[command_aliases[i]], command));
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
    else if(action == "exit"){
      exit(0);
    }
    else if(action == "help"){
      cout << "Available commands:" << std::endl;
      cout << "connect - extablish connection with server" << std::endl;
      cout << "  examples:" << std::endl;
      cout << "    connect 123.123.123.123 alias 1500" << std::endl;
      cout << "    connect 123.123.123.123 alias 1500, 134.134.134.134 alias2 1500" << std::endl;
      cout << "send - send command to connected server" << std::endl;
      cout << "  examples:" << std::endl;
      cout << "    send alias command_to_execute" << std::endl;
      cout << "    send alias, alias command_to_execute" << std::endl;
      cout << "sendf - send commands from file to connected server" << std::endl;
      cout << "  examples:" << std::endl;
      cout << "    sendf 123.123.123.123 ./file/path" << std::endl;
      cout << "    sendf 123.123.123.123 ./file/path ./file/path2" << std::endl;
      cout << "    sendf 123.123.123.123, 234.234.234.234 ./file/path" << std::endl;
      cout << "    sendf 123.123.123.123, 234.234.234.234 ./file/path ./file/path2" << std::endl << std::endl;
      cout << "  file contents:" << std::endl;
      cout << "    command to execute 1" << std::endl;
      cout << "    command to execute 2" << std::endl << std::endl;
      cout << "help - display this help prompt" << std::endl;
      cout << "exit - shutdown program" << std::endl;
    }

  }
  else {
    cout << "Unrecognised command! Type \"help\" to display available commands." << endl;
  }
  return retVector;
}
