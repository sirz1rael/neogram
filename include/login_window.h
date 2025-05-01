#pragma once

#include "countries.h"
#include "enums.h"
#include <glibmm/refptr.h>
#include <gtkmm.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/buildable.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class LoginWindow : public Gtk::ApplicationWindow {
public:
  LoginWindow();
  ~LoginWindow() override;

private:
  Gtk::HeaderBar w_headerbar;
  Gtk::Button *back_btn = nullptr;

  Gtk::MenuButton *country_button = nullptr;
  Gtk::Entry *phone_number = nullptr;
  Gtk::Button *next_button = nullptr;
  Gtk::Button *qr_button = nullptr;
  Gtk::Window *ui_window = nullptr;
  Gtk::Stack *main_stack = nullptr;
  Gtk::Button *verify_button = nullptr;
  Gtk::Button *theme_btn = nullptr;

  Glib::RefPtr<Gtk::Builder> builder{};
  Glib::RefPtr<Gtk::Settings> settings{};
  Glib::RefPtr<Gtk::CssProvider> css_provider{};

  Gtk::Window *main_window = nullptr;

protected:
  void load_widgets();
  void get_ui_window();

  void toggle_theme();
  bool m_dark_theme = false;

  Pages current_page = Pages::MAIN_LOGIN_PAGE;
  void change_page(Pages next_page);

  void on_login_success();
  bool check_credentials();

  template <typename T> T *get_widget(const std::string &name);
  CountryLoader m_loader;
};