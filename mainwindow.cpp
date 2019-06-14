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

void MainWindow::AddAccount(const QString& account_username,
                            const QString& account_password) {

  if (account_username.isEmpty() == true) {
    messagebox.setText("无法添加，账号名不能为空！");
  } else {
    if (account_manager.Find(account_username) != nullptr) {
      messagebox.setText("无法添加，账号： " + account_username + " 已存在！");
    } else {
      std::shared_ptr<AutobotAccount> temp_account_ptr
          = std::make_shared<AutobotAccount>(account_username,
                                             account_password);
      ui->listWidget_accounts->addItem(account_username);
      account_manager.AddAccount(temp_account_ptr);
      messagebox.setText("成功添加，账号： " + account_username);
    }
  }
  messagebox.show();
}

} //namespace
