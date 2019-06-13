#include "bot_edit_window.h"
#include "ui_bot_edit_window_form.h"

BotEditWindow::BotEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BotEditWindow)
{
  ui->setupUi(this);
}

BotEditWindow::~BotEditWindow()
{
  delete ui;
}
