#include "main_window.h"

MainWindow::MainWindow() {
  set_title("Neogram <ALPHA> AUTHORIZED");
  set_default_size(wndSize.width, wndSize.height);

  set_titlebar(this->w_header);
}

MainWindow::~MainWindow() = default;