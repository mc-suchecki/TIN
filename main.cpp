#include <boost/thread.hpp>
#include "include/connection.hpp"
#include "include/controller.hpp"

int main(int argc, char * argv[]) {
  Controller *controller = new Controller(argc, argv);
  boost::thread controllerThread = boost::thread(&Controller::run, controller);
  controllerThread.join();

  /*EventQueue eventQueue;
  Connection connection(&eventQueue, "127.0.0.1", 100);
  connection.init();

  std::string testCommand;
  std::cout << "Wpisz wiadomosc:" <<std::endl;
  std::cin >> testCommand;
  connection.execute(testCommand);*/

  return 0;
}
