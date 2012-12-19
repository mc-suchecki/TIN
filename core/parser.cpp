#include "../include/parser.hpp"
#include "../include/events/consoleEvent.hpp"
#include <boost/spirit/include/qi.hpp>
#include <vector>
#include <string>

using namespace std;
namespace qi = boost::spirit::qi;


Parser::Parser(){

}

ConsoleEvent * Parser::parse(string input){
  vector<double> v;
  vector<string> sv;
  bool result;
  result = qi::parse(
     input.begin(), input.end(), 
     (
      //(+alpha_p)[push_back_a(sv)] >> ',' >> (+alpha_p)[push_back_a(sv)]
      qi::double_
     )
     );
  
  if(result){
    for(int i=0; i<sv.size(); i++){
      cout<<i<<": "<<sv[i];
    }
    cout<<endl;
  }
  else{
    cout<<"Fail!"<<endl;
  }
  return new ConsoleEvent("tekst "+input);
}
