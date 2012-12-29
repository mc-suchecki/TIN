#pragma once

/**
 * Responsible for storing config in a simple way (in variables)
 */
class Config{
  public:
    //singletonic supersonic
    static Config * getInstance();

    // port
    void setPort(int port){this->port = port;}
    int getPort(){return port;}

    // debug level
    void setDebug(int debug){this->debug = debug;}
    int getDebug(){return debug;}

  private:
    Config();
    Config& operator= (Config&);
    Config(Config&);
    ~Config(){};

    static Config * instance;

    int port;
    //debug level
    int debug;

};
