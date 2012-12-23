#pragma once

#include <iostream>

class ConsoleEvent;

class Parser{
  public:
    Parser();
    ~Parser();
    ConsoleEvent * parse(std::string);

};
