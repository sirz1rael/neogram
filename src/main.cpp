
#include "enums.h"
#include "login_window.h"
#include "main_window.h"

#include <gtkmm.h>

#include <cstdlib>

int main(int argc, char *argv[]) {

  auto app = Gtk::Application::create("org.neogram");

  auto windowState =
      ClientState::UNAUTHORIZED; // Temporary placeholder for authorization :D

  switch (windowState) {
  case ClientState::AUTHORIZED:
    return app->make_window_and_run<MainWindow>(argc, argv);
  case ClientState::UNAUTHORIZED:
    return app->make_window_and_run<LoginWindow>(argc, argv);
  }
}