#include "../include/config.hpp"
#include <cstddef> //get NULL working

Config * Config::instance = NULL;

Config * Config::getInstance(){
  if(instance == NULL){
    instance = new Config();
  }
return instance;
}

