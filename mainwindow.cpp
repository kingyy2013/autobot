#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace autobot {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_pushButton_add_clicked() {
  bot_log_dialog.show();
}

void MainWindow::AddNewAccount(AutobotAccount) {

}

} //namespace
