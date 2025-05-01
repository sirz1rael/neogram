#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/headerbar.h>

class MainWindow : public Gtk::ApplicationWindow {
public:
  MainWindow();
  ~MainWindow() override;

protected:
  Gtk::HeaderBar w_header;

private:
  struct {
    int width = 800;
    int height = 600;
  } wndSize;
};