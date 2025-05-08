#pragma once

#include <gtkmm/application.h>
#include <gtkmm/window.h>
class Application : public Gtk::Application {
public:
  Application() = default;
  ~Application() = default;

  void init(std::string app_id);
  int run(int argc, char *argv[]);
};