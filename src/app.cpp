#include "app.h"
#include "windows/login_window.h"

void Application::init(std::string app_id) { this->create(app_id); }

void Application::run(int argc, char *argv[]) {
  this->make_window_and_run<LoginWindow>(argc, argv);
}
