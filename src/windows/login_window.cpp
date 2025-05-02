#include "windows/login_window.h"
#include "common/enums.h"
#include "common/extra.h"
#include "country_loader.h"
#include "windows/main_window.h"
#include <exception>
#include <giomm/action.h>
#include <giomm/actiongroup.h>
#include <giomm/actionmap.h>
#include <giomm/menuitem.h>
#include <giomm/simpleactiongroup.h>
#include <glib.h>
#include <glibmm/ustring.h>
#include <glibmm/variant.h>
#include <glibmm/varianttype.h>
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/listbox.h>
#include <gtkmm/listboxrow.h>
#include <gtkmm/listitemfactory.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/object.h>
#include <gtkmm/popover.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/stack.h>
#include <gtkmm/window.h>
#include <iostream>
#include <memory>
#include <sigc++/functors/mem_fun.h>

LoginWindow::LoginWindow() {
  std::unique_ptr<Extra> extra = std::make_unique<Extra>();

  this->builder = extra->load_builder("ui/LoginWindow.ui");

  g_info("Loaded UI file contains objects:");
  auto objects = builder->get_objects();

  // Checks if .ui file is incorrect or builder has error while loading objects
  // from it.
  if (objects.empty()) {
    g_critical("Builder doesn't have objects to load.");
  } else {
    for (auto &obj : objects) {
      g_info("- %s", G_OBJECT_TYPE_NAME(obj->gobj()));
    }
  }

  // Initialization of css & widgets
  this->css_provider = extra->load_styles("ui/styles/login_window.css");
  this->get_ui_window();
  this->setup_widgets();

  // Selecting country for phone humber
  auto action =
      Gio::SimpleAction::create("select-country", Glib::VARIANT_TYPE_STRING);
  action->signal_activate().connect([this](const Glib::VariantBase &variant) {
    this->is_country_selected = true;
    this->country_name =
        Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(variant)
            .get();
    country_button->set_label(country_name);
  });
  add_action(action);

  next_button->signal_clicked().connect([this, &extra]() {
    if (extra->is_phone_valid(phone_number->get_text()) ||
        is_country_selected == false) {
      auto dialog = Gtk::AlertDialog::create("Phone number is incorrect!");
      dialog->show(*this);
      return;
    } else {
      phone_number_to_login =
          extra->split(this->country_name)[1] + "-" + phone_number->get_text();
      std::cout << phone_number_to_login << std::endl;
    }
    this->change_page(PageTypes::TWO_FACTOR_LOGIN_PAGE);
  });
  qr_button->signal_clicked().connect(
      [this]() { this->change_page(PageTypes::QR_LOGIN_PAGE); });
  back_btn->signal_clicked().connect(
      [this]() { this->change_page(PageTypes::MAIN_LOGIN_PAGE); });

  back_btn->set_visible(current_page != PageTypes::MAIN_LOGIN_PAGE);

  verify_button->signal_clicked().connect([this]() {
    // Placeholder
    bool auth_success = check_credentials();

    if (auth_success) {
      on_login_success();
    } else {
      //..
    }
  });

  set_title("Neogram WIP");
  set_titlebar(w_headerbar);
  set_default_size(400, 600);
  set_resizable(false);
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
    auto dialog = Gtk::AlertDialog::create("Failed to load UI window!");
    dialog->show(*this);
    close();
    return;
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

void LoginWindow::setup_widgets() {
  g_assert(builder && "Builder not initialized");

  // Getting widgets
  country_button = this->get_widget<Gtk::MenuButton>("country_button");
  phone_number = this->get_widget<Gtk::Entry>("phone_number");
  next_button = this->get_widget<Gtk::Button>("next_button");
  qr_button = this->get_widget<Gtk::Button>("qr_button");
  main_stack = this->get_widget<Gtk::Stack>("main_stack");
  verify_button = this->get_widget<Gtk::Button>("verify_button");

  // Back button initialization
  back_btn = Gtk::make_managed<Gtk::Button>();
  back_btn->set_icon_name("go-previous-symbolic");
  w_headerbar.pack_start(*back_btn);

  if (!main_stack || !phone_number || !next_button) {
    g_critical("Essential widgets missing");
    std::terminate();
  }

  main_stack->set_visible_child("main_login_page");
  current_page = PageTypes::MAIN_LOGIN_PAGE;

  // Maybe it will be transferred to .ui file
  phone_number->set_input_purpose(Gtk::InputPurpose::PHONE);
  phone_number->set_max_length(15);

  // WIP: Next button request to the server for authorization
  // ...

  // WIP: Phone codes depending on country
  // Ready: Loading data from the json in a separate thread.
  // TODO(): determine country by ip using tdlib & when definition
  // successfully replace the icon with the country phone code similarly
  // when the user selects his country. And all of it in separate thread.

  auto menu = Gio::Menu::create();

  m_loader.get_dispatcher().connect([this, menu] {
    if (m_loader.has_error()) {
      auto dialog = Gtk::AlertDialog::create("Error loading countries");
      dialog->show(*this);
      return;
    }
    auto data = m_loader.get_data();

    std::sort(data.begin(), data.end(), [](const auto &a, const auto &b) {
      return a < b; // Sorting by name alphabetically
    });

    for (const auto &[name, code] : data) {
      auto item_text = name + " " + code;

      auto item = Gio::MenuItem::create(item_text, "win.select-country");
      item->set_action_and_target(
          "win.select-country",
          Glib::Variant<Glib::ustring>::create(item_text));
      menu->append_item(item);
    }
  });

  country_button->set_menu_model(menu);

  m_loader.load_async("countries.json");

  // WIP: Theme button, just for sexy look
  theme_btn = Gtk::make_managed<Gtk::Button>();
  theme_btn->set_icon_name("weather-clear-night-symbolic");
  theme_btn->set_tooltip_text("Toggle Dark Theme");
  theme_btn->signal_clicked().connect(
      sigc::mem_fun(*this, &LoginWindow::toggle_theme));
  w_headerbar.pack_start(*theme_btn);
}

void LoginWindow::change_page(const PageTypes &next_page) {
  const char *page_name = nullptr;
  switch (next_page) {
  case PageTypes::QR_LOGIN_PAGE:
    page_name = "qr_login_page";
    break;
  case PageTypes::TWO_FACTOR_LOGIN_PAGE:
    page_name = "2fa_login_page";
    break;
  case PageTypes::MAIN_LOGIN_PAGE:
    page_name = "main_login_page";
    break;
  default:
    g_error("Invalid page: %d", static_cast<int>(next_page));
    return;
  }

  if (auto page = main_stack->get_child_by_name(page_name)) {
    main_stack->set_visible_child(*page);
    current_page = next_page;
    back_btn->set_visible(current_page != PageTypes::MAIN_LOGIN_PAGE);
  } else {
    g_critical("Page %s not found in stack!", page_name);
  }
}

void LoginWindow::on_login_success() {
  auto app = this->get_application();

  main_window = Gtk::make_managed<MainWindow>();
  app->add_window(*main_window);

  main_window->present();

  this->close();
  //..
}

bool LoginWindow::check_credentials() { return true; }