#include "autobot_edit_window.h"
#include "ui_autobot_edit_window_form.h"

namespace autobot {
AutobotEditWindow::AutobotEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::AutobotEditWindow)
{
  ui->setupUi(this);
}

AutobotEditWindow::~AutobotEditWindow()
{
  delete ui;
}
}
