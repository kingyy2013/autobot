#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace autobot {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  autobot_edit_window_(new AutobotEditWindow(this)){
  ui->setupUi(this);
  ui->treeWidget_accounts->setColumnWidth(0, 300);
  connect(&bot_log_dialog_, SIGNAL(AddNewAccount(const QString&,
                                                const QString&)),
          this, SLOT(AddAccount(const QString&, const QString&)));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_pushButton_add_clicked() {
  bot_log_dialog_.show();
}

void MainWindow::AddAccount(const QString& account_username,
                            const QString& account_password) {
  QMessageBox messagebox(this);
  messagebox.setWindowTitle("");
  if (account_username.isEmpty() == true) {
    messagebox.setText("无法添加，账号名不能为空！");
    messagebox.exec();
  } else {
    if (account_manager_.Find(account_username) != nullptr) {
      messagebox.setText("无法添加，账号： " + account_username + " 已存在！");
      messagebox.exec();
    } else {
      std::shared_ptr<AutobotAccount> temp_account_ptr
          = std::make_shared<AutobotAccount>(account_username,
                                             account_password);
      QTreeWidgetItem *autobot_item
          = new QTreeWidgetItem(ui->treeWidget_accounts);
      ui->treeWidget_accounts->addTopLevelItem(autobot_item);
      autobot_item->setText(0, account_username);
      autobot_item->setText(1, "空闲");
      account_manager_.AddAccount(temp_account_ptr);
    }
  }
}

void MainWindow::on_pushButton_delete_clicked() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_accounts->selectedItems();
  if (selected_items.empty() == false) {
    QString account_list_msg;
    foreach(QTreeWidgetItem * item, selected_items)  {
      account_list_msg.append(item->text(0));
      account_list_msg.append(" ");
    }
    QMessageBox messagebox(this);
    messagebox.setWindowTitle("");
    messagebox.setText("您确定删除： " + account_list_msg + "?");
    messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
    messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
    // Not so sure why Accept role coresponds to 0, but reject corepsonds to 1.
    if (messagebox.exec() == false) {
      foreach(QTreeWidgetItem * item, selected_items)  {
        account_manager_.RemoveAutobot(item->text(0));
        delete item;
      }
    }
  }
}

void MainWindow::on_treeWidget_accounts_itemDoubleClicked(
    QTreeWidgetItem *item, int) {
  QString account_username = item->text(0);
  std::shared_ptr <AutobotAccount> bot_account_ptr
      = account_manager_.Find(account_username);
  if (bot_account_ptr == nullptr) {
    qFatal(("account name " + account_username + " is not in the manager")
           .toStdString().data());
  } else {
    autobot_edit_window_->CombineAutobotAccount(bot_account_ptr);
    autobot_edit_window_->move(this->pos().x() + this->width(), this->pos().y());
    autobot_edit_window_->show();
  }
}

void MainWindow::on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item,
                                                    int column) {
  if (autobot_edit_window_->isVisible()) {
    on_treeWidget_accounts_itemDoubleClicked(item, column);
  }
}

} //namespace





