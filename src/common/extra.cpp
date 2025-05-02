#include "common/extra.h"

#include <cstdlib>
#include <glib.h>
#include <iostream>
#include <regex>

Glib::RefPtr<Gtk::Builder> Extra::load_builder(const std::string &ui_path) {
  try {
    return Gtk::Builder::create_from_file(ui_path);
  } catch (const Glib::Error &e) {
    g_critical("Error loading UI file: %s", e.what());
    return nullptr;
  }
}

Glib::RefPtr<Gtk::CssProvider> Extra::load_styles(std::string path_to_file) {
  this->css_provider = Gtk::CssProvider::create();
  try {
    css_provider->load_from_path(path_to_file);
  } catch (const Glib::Error &ex) {
    std::cerr << "CSS error: " << ex.what() << std::endl;
  }

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(), css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  return this->css_provider;
}

std::vector<std::string> Extra::split(const std::string &str) {
  std::istringstream iss(str);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

bool Extra::is_phone_valid(const std::string &phone_number) {
  std::regex pattern(R"(^\+?[0-9]+$)");
  return !std::regex_match(phone_number, pattern);
}