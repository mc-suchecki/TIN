#pragma once

#include <iostream>
#include <vector>

class ConsoleEvent;

class Parser{
  public:
    Parser();
    ~Parser();
    std::vector<ConsoleEvent*> parse(std::string);

};
