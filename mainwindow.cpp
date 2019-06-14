#include "mainwindow.h"
#include "ui_mainwindow.h"


namespace autobot {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(&bot_log_dialog, SIGNAL(AddNewAccount(const QString&,
                                                const QString&)),
          this, SLOT(AddAccount(const QString&, const QString&)));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_pushButton_add_clicked() {
  bot_log_dialog.show();
}

void MainWindow::AddAccount(const QString&, const QString&) {
  messagebox.setText("Success");
  messagebox.show();
}

} //namespace
