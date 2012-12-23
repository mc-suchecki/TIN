#include <boost/thread.hpp>
#include "include/connection.hpp"
#include "include/controller.hpp"

int main() {
  Controller *controller = new Controller();
  boost::thread controllerThread = boost::thread(&Controller::run, controller);
  controllerThread.join();
  return 0;
}
