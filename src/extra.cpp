#include "extra.h"

#include <cstdlib>
#include <iostream>

Glib::RefPtr<Gtk::Builder> Extra::load_builder(std::string path_to_file) {
  this->builder = Gtk::Builder::create();
  try {
    builder->add_from_file(path_to_file);
  } catch (const Glib::FileError &ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
  }

  return this->builder;
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