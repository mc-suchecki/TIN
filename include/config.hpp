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

    // port
    void setPort(int port){this->port = port;}
    int getPort(){return port;}

    // debug level
    void setDebug(int debug){this->debug = debug;}
    int getDebug(){return debug;}

    // log file
    void setLogFile(std::string logFile){this->logFile = logFile;}
    std::string getLogFile(){return logFile;}

  private:
    Config();
    Config& operator= (Config&);
    Config(Config&);
    ~Config(){};

    static Config * instance;

    int port;
    int debug;
    std::string logFile;

};
