
#include "main_window.h"
#include "login_window.h"
#include "window_states.h"

#include <gtkmm.h>

#include <cstdlib>

int main(int argc, char* argv[]) {

    auto app = Gtk::Application::create("org.neogram");
    
    auto windowState = WindowState::UNAUTHORIZED; // Temporary placeholder for authorization :D

    switch (windowState) {
        case WindowState::AUTHORIZED:
            return app->make_window_and_run<MainWindow>(argc, argv);
        case WindowState::UNAUTHORIZED:
            return app->make_window_and_run<LoginWindow>(argc, argv);
    }
}   