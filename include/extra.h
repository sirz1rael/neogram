#pragma once

#include <string>
#include <gtkmm.h>


class Extra {
public:
    Glib::RefPtr<Gtk::Builder> load_builder(std::string path_to_file);
    Glib::RefPtr<Gtk::CssProvider> load_styles(std::string path_to_file);
private:
    Glib::RefPtr<Gtk::Builder> builder {};
    Glib::RefPtr<Gtk::CssProvider> css_provider {};
};
