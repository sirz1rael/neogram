#include "login_window.h"

LoginWindow::LoginWindow() {
    set_title("Neogram <ALPHA> UNAUTHORIZED");
    set_default_size(wndSize.width, wndSize.height);
    
    set_titlebar(this->w_header);
}

LoginWindow::~LoginWindow() = default; 