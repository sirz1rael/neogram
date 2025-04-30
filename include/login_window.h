#pragma once

#include <glibmm/refptr.h>
#include <gtkmm.h>
#include <gtkmm/buildable.h>
#include <gtkmm/button.h>

class LoginWindow : public Gtk::Window {
public:
  LoginWindow();
  ~LoginWindow() override;

private:
  Gtk::HeaderBar w_headerbar;
  Gtk::MenuButton *country_button = nullptr;
  Gtk::Entry *phone_number = nullptr;
  Gtk::Button *next_button = nullptr;

  Gtk::Button *theme_btn = nullptr;

  Glib::RefPtr<Gtk::Builder> builder{};
  Glib::RefPtr<Gtk::Settings> settings{};
  Glib::RefPtr<Gtk::CssProvider> css_provider{};

protected:
  void load_widgets();
  void get_ui_window();

  void toggle_theme();
  bool m_dark_theme = false;
};