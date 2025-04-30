#pragma once

#include <gtkmm.h>
#include <string>

class Extra {
public:
  Glib::RefPtr<Gtk::Builder> load_builder(const std::string &ui_path);
  Glib::RefPtr<Gtk::CssProvider> load_styles(std::string path_to_file);

private:
  Glib::RefPtr<Gtk::Builder> builder{};
  Glib::RefPtr<Gtk::CssProvider> css_provider{};
};
