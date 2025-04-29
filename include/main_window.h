#pragma once

#include <gtkmm/headerbar.h>
#include <gtkmm/window.h>

class MainWindow : public Gtk::Window {
public:
    MainWindow();
    ~MainWindow() override;

    

protected:
    Gtk::HeaderBar w_header;

private:
    struct {
        int width = 600;
        int height = 400;
    } wndSize;
};