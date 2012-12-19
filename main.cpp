#include "include/controller.hpp"
#include <boost/thread.hpp>

int main() {
  Controller *controller = new Controller();
  boost::thread controllerThread = boost::thread(&Controller::run, controller);
  controllerThread.join();
  return 0;
}
