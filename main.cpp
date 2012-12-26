#include <boost/thread.hpp>
#include "include/connection.hpp"
#include "include/controller.hpp"

int main() {
  Controller *controller = new Controller();
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
