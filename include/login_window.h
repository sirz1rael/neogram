#pragma once

#include <gtkmm/headerbar.h>
#include <gtkmm/window.h>

class LoginWindow : public Gtk::Window {
public:    
    LoginWindow();
    ~LoginWindow() override;
    protected:
    Gtk::HeaderBar w_header;

private:
    struct {
        int width = 600;
        int height = 400;
    } wndSize;
};