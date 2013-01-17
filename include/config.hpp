#pragma once

#include <string>

/**
 * Responsible for storing config in a simple way (in variables).
 */
class Config{
  public:
    //singletonic supersonic
    //Maciej Suchecki lubi to.
    static Config * getInstance();

    // debug level
    void setDebug(int debug){this->debug = debug;}
    int getDebug(){return debug;}

    // log file
    void setLogFile(std::string logFile){this->logFile = logFile;}
    std::string getLogFile(){return logFile;}

    // script file
    void setScriptFile(std::string scriptFile){this->scriptFile = scriptFile;}
    std::string getScriptFile(){return scriptFile;}

  private:
    Config();
    Config& operator= (Config&);
    Config(Config&);
    ~Config(){};

    static Config * instance;

    int debug;
    std::string logFile;
    std::string scriptFile;

};
