#pragma once

#include <gtkmm.h>
#include <string>

class Extra {
public:
  Glib::RefPtr<Gtk::Builder> load_builder(const std::string &ui_path);
  Glib::RefPtr<Gtk::CssProvider> load_styles(std::string path_to_file);
  std::vector<std::string> split(const std::string &str);
  bool is_phone_valid(const std::string &phone_number);

private:
  Glib::RefPtr<Gtk::Builder> builder{};
  Glib::RefPtr<Gtk::CssProvider> css_provider{};
};
