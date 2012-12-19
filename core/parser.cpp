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

Parser::Parser(){

}

void read(string const& a){
  cout<<a;
}


ConsoleEvent * Parser::parse(string input){
  vector<double> v;
  vector<string> sv;
  bool result;
  string address;
  result = qi::parse(
      input.begin(), input.end(), 
      (
      // (lit("connect") >> 
        qi::as_string[+(qi::char_ - " ")]
        >> " " >>
        qi::as_string[+(qi::char_)]
       // )
      // | lit("send")
      // | lit("disconnect")
      ),
      sv
      );

  if(result){
    if(sv[0] == "connect"){
      return new CreateConnectionEvent(sv[1]);
    }
    else if(sv[0] == "send"){
      return new ConsoleEvent("tekst "+input);
    }
  }
  else{
    cout<<"Unrecognised command!"<<endl;
  }
  return NULL;
}
