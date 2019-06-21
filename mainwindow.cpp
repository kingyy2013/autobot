#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autobot_helper.h"
#include <QDebug>
#include <QFileDialog>

namespace autobot {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  bot_log_dialog_(new AutobotLoginDialog()),
  autobot_edit_window_(new AutobotEditWindow(this)),
  last_directory_(QDir::currentPath()) {
  ui->setupUi(this);
  ui->treeWidget_accounts->setColumnWidth(0, 300);
  connect(bot_log_dialog_, SIGNAL(AddNewAccount(const QString&,
                                                const QString&)),
          this, SLOT(AddAccount(const QString&, const QString&)));
}

MainWindow::~MainWindow() {
  delete bot_log_dialog_;
  delete ui;
}

void MainWindow::on_pushButton_add_clicked() {
  bot_log_dialog_->show();
}

void MainWindow::UpdateAccountToView(
    const std::shared_ptr<AutobotAccount>& account_ptr) {
  SetAccountToView(account_ptr,
                   ui->treeWidget_accounts->currentItem());
}

void MainWindow::SetAccountToView(
    const std::shared_ptr<AutobotAccount>& account_ptr,
    QTreeWidgetItem *autobot_item) {
  autobot_item->setText(0, account_ptr->GetUsername());
  QPair<QString, QColor> status_and_color
      = ConvertStatusToQStringAndColor(account_ptr->GetStatus());
  autobot_item->setText(1,
                        status_and_color.first);
  autobot_item->setBackground(0, QBrush(status_and_color.second));
  autobot_item->setBackground(1, QBrush(status_and_color.second));
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
      SetAccountToView(temp_account_ptr, autobot_item);
      ui->treeWidget_accounts->addTopLevelItem(autobot_item);
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
    autobot_edit_window_->move(this->pos().x() + this->width(),
                               this->pos().y());
    autobot_edit_window_->show();
  }
}

void MainWindow::on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item,
                                                    int column) {
  if (item == nullptr) {
    autobot_edit_window_->hide();
  } else {
    if (autobot_edit_window_->isVisible()) {
      on_treeWidget_accounts_itemDoubleClicked(item, column);
    }
  }
}

void MainWindow::on_pushButton_saveall_clicked() {
  QString filename =
      QFileDialog::getSaveFileName(
        this,
        "Save Document",
        last_directory_,
        "Document files (*.yys);;All files (*.*)");
  if (!filename.isEmpty()) {
    QFileInfo file_info(filename);
    last_directory_ = file_info.absoluteDir().absolutePath();
  }

  // Writing to a file
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Open the file for writing failed";
  } else {
    QDomDocument document;
    QDomElement root = ConvertAutobotManagerToXML(account_manager_, &document);
    document.appendChild(root);
    QTextStream stream(&file);
    stream << document.toString();
    file.close();
    qDebug() << "Writing is done: " << document.toString();
  }
}

void MainWindow::on_pushButton_loadall_clicked() {
  QString filename =
      QFileDialog::getOpenFileName(
        this,
        "Open Document",
        last_directory_,
        "Document files (*.yys);;All files (*.*)");
  if (!filename.isEmpty()) {
    QFileInfo file_info(filename);
    last_directory_ = file_info.absoluteDir().absolutePath();
  }

  // Create a document to write XML
  QDomDocument document;

  // Open a file for reading
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))  {
    qDebug() << "Failed to open the file for reading.";
  } else {
    // loading
    if(!document.setContent(&file, true)) {
      qDebug() << "Failed to load the file for reading.";
    }
    file.close();
  }

  // Getting root element
  QDomElement root = document.firstChildElement();
  qDebug() << "Read file saved on: " << root.attributeNode("DateTime").value();
  if (ParseXMLToAutobotManager(root,
                               &account_manager_)) {
    qDebug() << "Failed to parse file.";
  }
}

} //namespace




