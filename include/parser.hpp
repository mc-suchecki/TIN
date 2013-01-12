#pragma once

#include <iostream>
#include <vector>
#include <map>

class ConsoleEvent;

class Parser{
  public:
    Parser();
    ~Parser();
    std::vector<ConsoleEvent*> parse(std::string);
  private:
    std::map<std::string,std::string> aliases;
};
