
#include "app.h"

#include <gtkmm.h>

#include <cstdlib>
#include <gtkmm/object.h>
#include <memory>

int main(int argc, char *argv[]) {
  std::shared_ptr<Application> application = std::make_shared<Application>();

  application->init("org.neogram");
  application->run(argc, argv);
}