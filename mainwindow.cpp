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
  QMessageBox messagebox(this);
  messagebox.setWindowTitle("");
  if (account_username.isEmpty() == true) {
    messagebox.setText("无法添加，账号名不能为空！");
    messagebox.exec();
  } else {
    if (account_manager.Find(account_username) != nullptr) {
      messagebox.setText("无法添加，账号： " + account_username + " 已存在！");
      messagebox.exec();
    } else {
      std::shared_ptr<AutobotAccount> temp_account_ptr
          = std::make_shared<AutobotAccount>(account_username,
                                             account_password);
      ui->listWidget_accounts->addItem(account_username);
      account_manager.AddAccount(temp_account_ptr);
    }
  }
}

void MainWindow::on_pushButton_delete_clicked() {
  QList<QListWidgetItem*> selected_items
      = ui->listWidget_accounts->selectedItems();
  if (selected_items.empty() == false) {
    QString account_list_msg;
    foreach(QListWidgetItem * item, selected_items)  {
      account_list_msg.append(item->text());
      account_list_msg.append(" ");
    }
    QMessageBox messagebox(this);
    messagebox.setWindowTitle("");
    messagebox.setText("您确定删除： " + account_list_msg + "?");

    messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
    messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
    // Not so sure why Accept role coresponds to 0, but reject corepsonds to 1.
    if (messagebox.exec() == false) {
      foreach(QListWidgetItem * item, selected_items)  {
        account_manager.RemoveAutobot(item->text());
        delete ui->listWidget_accounts->
            takeItem(ui->listWidget_accounts->row(item));
      }
    }
  }
}

} //namespace
