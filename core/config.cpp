#include "../include/config.hpp"

/// instance variable
Config * Config::instance = nullptr;

/// singleton
Config * Config::getInstance(){
  if(instance == nullptr){
    instance = new Config();
  }
return instance;
}

/// constructor with default values
Config::Config(){
  // put default values here
  debug = 0;
  logFile = "no";
  scriptFile = "";
}

