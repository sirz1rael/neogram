#include "login_window.h"
#include "enums.h"
#include "extra.h"
#include <exception>
#include <glib.h>
#include <gtkmm/button.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/object.h>
#include <gtkmm/stack.h>
#include <gtkmm/window.h>
#include <sigc++/functors/mem_fun.h>

LoginWindow::LoginWindow() {
  Extra *extra = new Extra();

  this->builder = extra->load_builder("ui/LoginWindow.ui");

  g_info("Loaded UI file contains objects:");
  auto objects = builder->get_objects();

  if (objects.empty()) {
    g_critical("Builder doesn't have objects to load.");
  } else {
    for (auto &obj : objects) {
      g_info("- %s", G_OBJECT_TYPE_NAME(obj->gobj()));
    }
  }

  this->css_provider = extra->load_styles("ui/styles/login_window.css");
  this->get_ui_window();
  this->load_widgets();

  next_button->signal_clicked().connect(
      [this]() { this->change_page(Pages::TFA_LOGIN_PAGE); });
  qr_button->signal_clicked().connect(
      [this]() { this->change_page(Pages::QR_LOGIN_PAGE); });
  back_btn->signal_clicked().connect(
      [this]() { this->change_page(Pages::MAIN_LOGIN_PAGE); });

  back_btn->set_visible(current_page != Pages::MAIN_LOGIN_PAGE);

  set_title("Neogram WIP");
  set_titlebar(w_headerbar);
  set_default_size(400, 600);
  set_resizable(false);

  delete extra;
}

LoginWindow::~LoginWindow() = default;

void LoginWindow::toggle_theme() {
  m_dark_theme = !m_dark_theme;

  settings = Gtk::Settings::get_default();
  settings->property_gtk_application_prefer_dark_theme() = m_dark_theme;

  theme_btn->set_icon_name(m_dark_theme ? "weather-clear-symbolic"
                                        : "weather-clear-night-symbolic");
}

void LoginWindow::get_ui_window() {
  // Gettings root_container
  ui_window = builder->get_widget<Gtk::Window>("login_window");
  if (!ui_window) {
    g_critical("Failed to load UI window!");
    std::terminate();
  }

  // Transfer content from ui file
  if (auto child = ui_window->get_child()) {
    ui_window->unset_child();
    set_child(*child); // Setting root container to current window
  } else {
    g_critical("ui_window has no child!");
  }
}

template <typename T> T *LoginWindow::get_widget(const std::string &name) {
  T *widget = nullptr;
  widget = builder->get_widget<T>(name);
  if (!widget) {
    g_warning("Widget %s not found!", name.c_str());
  }
  return widget;
}

void LoginWindow::load_widgets() {
  g_assert(builder && "Builder not initialized");

  // Getting widgets
  country_button = this->get_widget<Gtk::MenuButton>("country_button");
  phone_number = this->get_widget<Gtk::Entry>("phone_number");
  next_button = this->get_widget<Gtk::Button>("next_button");
  qr_button = this->get_widget<Gtk::Button>("qr_button");
  main_stack = this->get_widget<Gtk::Stack>("main_stack");

  back_btn = Gtk::make_managed<Gtk::Button>();
  back_btn->set_icon_name("go-previous-symbolic");
  w_headerbar.pack_start(*back_btn);

  if (!main_stack || !phone_number || !next_button) {
    g_critical("Essential widgets missing");
    std::terminate();
  }

  main_stack->set_visible_child("main_login_page");
  current_page = Pages::MAIN_LOGIN_PAGE;

  // Maybe it will be transferred to .ui file
  phone_number->set_input_purpose(Gtk::InputPurpose::PHONE);
  phone_number->set_max_length(15);

  // WIP: Next button request to the server for authorization
  // ...

  // WIP: Phone codes depending on country
  // ...

  // WIP: Theme button, just for sexy look
  theme_btn = Gtk::make_managed<Gtk::Button>();
  theme_btn->set_icon_name("weather-clear-night-symbolic");
  theme_btn->set_tooltip_text("Toggle Dark Theme");
  theme_btn->signal_clicked().connect(
      sigc::mem_fun(*this, &LoginWindow::toggle_theme));
  w_headerbar.pack_start(*theme_btn);
}

void LoginWindow::change_page(Pages next_page) {
  const char *page_name = nullptr;
  switch (next_page) {
  case Pages::QR_LOGIN_PAGE:
    page_name = "qr_login_page";
    break;
  case Pages::TFA_LOGIN_PAGE:
    page_name = "2fa_login_page";
    break;
  case Pages::MAIN_LOGIN_PAGE:
    page_name = "main_login_page";
    break;
  default:
    g_error("Invalid page: %d", static_cast<int>(next_page));
    return;
  }

  if (auto page = main_stack->get_child_by_name(page_name)) {
    main_stack->set_visible_child(*page);
    current_page = next_page;
    back_btn->set_visible(current_page != Pages::MAIN_LOGIN_PAGE);
  } else {
    g_critical("Page %s not found in stack!", page_name);
  }
}