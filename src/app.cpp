#include "app.h"
#include "windows/login_window.h"
#include "windows/main_window.h"
#include <giomm/application.h>
#include <gtkmm/application.h>
#include <gtkmm/object.h>

void Application::init(std::string app_id) { this->create(app_id); }

int Application::run(int argc, char *argv[]) {

  return this->make_window_and_run<LoginWindow>(argc, argv);
}
