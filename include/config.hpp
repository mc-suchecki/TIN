class Config{
public:
  static Config * getInstance();
  void setPort(int port){this->port = port;}
  int getPort(){return port;};
private:
    Config(){port = 777;};
    Config& operator= (Config&);
    Config(Config&);
    ~Config(){};

    static Config * instance;

    int port;

};
