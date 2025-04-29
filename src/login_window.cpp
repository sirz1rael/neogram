#include "login_window.h"
#include "extra.h"
#include <exception>
#include <gtkmm/menubutton.h>
#include <gtkmm/object.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>

LoginWindow::LoginWindow() {
    Extra *extra = new Extra();

    this->builder = extra->load_builder("ui/LoginWindow.ui");
    this->css_provider = extra->load_styles("ui/styles/login_window.css");
    
    
    // Gettings root_container
    auto ui_window = builder->get_widget<Gtk::Window>("login_window");
    if (!ui_window) {
        std::cerr << "Could not get 'login_window' from UI file" << std::endl;
        std::terminate();
    }

    // Transfer content from ui file
    if (auto child = ui_window->get_child()) {
        ui_window->unset_child();
        set_child(*child); // Setting root container to current window
    }

    // Getting widgets
    country_button = builder->get_widget<Gtk::MenuButton>("country_button");
    phone_number = builder->get_widget<Gtk::Entry>("phone_number");
    next_button = builder->get_widget<Gtk::Button>("next_button");

    next_button->signal_clicked().connect([this]() {
        if(phone_number->get_text() != "") {
            std::cout << phone_number->get_text() << std::endl;
        } else {
            std::cout << "Some of fields are empty!" << std::endl;
        } 
    });

    theme_btn = Gtk::make_managed<Gtk::Button>();
    theme_btn->set_icon_name("weather-clear-night-symbolic");
    theme_btn->set_tooltip_text("Toggle Dark Theme");

    theme_btn->signal_clicked().connect(
        sigc::mem_fun(*this, &LoginWindow::toggle_theme)
    );

    w_headerbar.pack_start(*theme_btn);
    
    set_title("Neogram");
    set_titlebar(w_headerbar);
    set_default_size(400, 500);
    set_resizable(false);
}

LoginWindow::~LoginWindow() = default;

void LoginWindow::toggle_theme() {
    m_dark_theme = !m_dark_theme;

    settings = Gtk::Settings::get_default();
    settings->property_gtk_application_prefer_dark_theme() = m_dark_theme;

    theme_btn->set_icon_name(
        m_dark_theme ? "weather-clear-symbolic" : "weather-clear-night-symbolic"
    );
}