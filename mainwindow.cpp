#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_autobot_login_dialog_form.h"

#include "autobot_helper.h"
#include <QDebug>
#include <QFileDialog>

namespace autobot {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  autobot_login_dialog_ui_(new Ui::AutobotLoginDialog()),
  autobot_login_dialog_(new QDialog()),
  autobot_edit_window_(new AutobotEditWindow(this)),
  target_room_edit_window_(new TargetRoomEditWindow(this)),
  target_speech_edit_window_(new TargetSpeechEditWindow(this)),
  last_directory_(QDir::currentPath()) {
  ui->setupUi(this);
  ui->treeWidget_accounts->setColumnWidth(0, 150);
  ui->treeWidget_accounts->setColumnWidth(1, 150);
  ui->treeWidget_accounts->setColumnWidth(2, 150);
  autobot_login_dialog_ui_->setupUi(autobot_login_dialog_);
  // Set up the autobot_login_dialog connections.
  connect(autobot_login_dialog_ui_->pushButton_add, SIGNAL(clicked()),
          this, SLOT(AddAccountFromUi()));
  connect(autobot_login_dialog_ui_->pushButton_cancel, SIGNAL(clicked()),
          autobot_login_dialog_, SLOT(close()));

  connect(&(AutobotManager::GetInstance()),
          SIGNAL(AccountsChanged(const QStringList&)), this,
          SLOT(UpdateSelectedAccountToView(const QStringList&)));
  connect(&(AutobotManager::GetInstance()), SIGNAL(AccountsChanged()),
          this, SLOT(UpdateAllAccountToView()));
}

MainWindow::~MainWindow() {
  delete autobot_login_dialog_ui_;
  delete autobot_login_dialog_;
  delete ui;
}

void MainWindow::on_pushButton_account_add_clicked() {
  autobot_login_dialog_->exec();
}

void MainWindow::UpdateAllAccountToView() {
  for (auto account_ptr : AutobotManager::GetInstance().GetAccountDict()) {
    SetAccountToView(account_ptr,
                     account_to_tree_item_map_[account_ptr->GetUsername()]);
  }
}

void MainWindow::UpdateSelectedAccountToView(
    const QStringList& selected_accounts) {
  const auto& account_dict = AutobotManager::GetInstance().GetAccountDict();
  for (const auto& selected_account_name : selected_accounts) {
    SetAccountToView(account_dict[selected_account_name],
                     account_to_tree_item_map_[selected_account_name]);
  }
}

void MainWindow::UpdateAccountToCurrentView(
    const std::shared_ptr<AutobotAccount>& account_ptr) {
  SetAccountToView(account_ptr,
                   ui->treeWidget_accounts->currentItem());
}

void MainWindow::SetAccountToView(
    const std::shared_ptr<AutobotAccount>& account_ptr,
    QTreeWidgetItem *autobot_item) {
  autobot_item->setText(0, account_ptr->GetUsername());
  autobot_item->setText(1, account_ptr->GetNickname());
  autobot_item->setText(2, account_ptr->GetSpeechName());
  QPair<QString, QColor> status_and_color
      = ConvertStatusToQStringAndColor(account_ptr->GetStatus());
  autobot_item->setText(3,
                        status_and_color.first);
  autobot_item->setBackground(0, QBrush(status_and_color.second));
  autobot_item->setBackground(1, QBrush(status_and_color.second));
  autobot_item->setBackground(2, QBrush(status_and_color.second));
  autobot_item->setBackground(3, QBrush(status_and_color.second));
  for (const auto& target_room : account_ptr->GetTargetRoomMap()) {
    QTreeWidgetItem *target_room_item
        = new QTreeWidgetItem(autobot_item);
    target_room_item->setText(0, target_room->GetRoomNumber());
    autobot_item->addChild(target_room_item);
  }
}

void MainWindow::AddAccountFromUi() {
  const QString& account_username
      = autobot_login_dialog_ui_->lineEdit_username->text();
  const QString& account_password
      = autobot_login_dialog_ui_->lineEdit_password->text();
  QMessageBox messagebox(this);
  messagebox.setWindowTitle("");
  if (account_username.isEmpty() == true) {
    messagebox.setText("无法添加，账号名不能为空！");
    messagebox.exec();
  } else {
    if (AutobotManager::GetInstance().Find(account_username) != nullptr) {
      messagebox.setText("无法添加，账号： " + account_username + " 已存在！");
      messagebox.exec();
    } else {
      std::shared_ptr<AutobotAccount> temp_account_ptr
          = std::make_shared<AutobotAccount>(account_username,
                                             account_password);
      QTreeWidgetItem *autobot_item
          = new QTreeWidgetItem(ui->treeWidget_accounts);
      SetAccountToView(temp_account_ptr, autobot_item);
      account_to_tree_item_map_[temp_account_ptr->GetUsername()]
          = autobot_item;
      ui->treeWidget_accounts->addTopLevelItem(autobot_item);
      AutobotManager::GetInstance().AddAccount(temp_account_ptr);
      autobot_login_dialog_->close();
    }
  }
}

void MainWindow::AddManagerToView() {
  for (const auto& autobot_account :
       AutobotManager::GetInstance().GetAccountDict()) {
    QTreeWidgetItem *autobot_item
        = new QTreeWidgetItem(ui->treeWidget_accounts);
    SetAccountToView(autobot_account, autobot_item);
    ui->treeWidget_accounts->addTopLevelItem(autobot_item);
    account_to_tree_item_map_[autobot_account->GetUsername()]
        = autobot_item;
  }
}

void MainWindow::on_pushButton_account_delete_clicked() {
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
        // If this is the top level item (account).
        if (item->parent() == nullptr) {
          account_to_tree_item_map_.remove(item->text(0));
          AutobotManager::GetInstance().RemoveAutobot(item->text(0));
          delete item;
        } else {
          const QString& parent_account_name = item->parent()->text(0);
          AutobotManager::GetInstance().GetAccountDict()[parent_account_name]->
              RemoveTargetRoom(item->text(0));
          delete item;
        }
      }
    }
  }
}

void MainWindow::SetSelectedAcountToManager() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_accounts->selectedItems();
  QStringList selected_item_names;
  for (auto item : selected_items) {
    selected_item_names.append(item->text(0));
  }
  AutobotManager::GetInstance().SetSelectedAcountNames(selected_item_names);
}

void MainWindow::on_treeWidget_accounts_itemDoubleClicked(
    QTreeWidgetItem *item, int) {
  // If this is top level item.
  if (item->parent() == nullptr) {
    QString account_username = item->text(0);
    std::shared_ptr <AutobotAccount> bot_account_ptr
        = AutobotManager::GetInstance().Find(account_username);
    if (bot_account_ptr == nullptr) {
      qFatal(("account name " + account_username + " is not in the manager")
             .toStdString().data());
    } else {
      // Brings account edit window.
      autobot_edit_window_->CombineAutobotAccount(bot_account_ptr);
      autobot_edit_window_->move(this->pos().x() + this->width(),
                                 this->pos().y());
      autobot_edit_window_->show();
      // Brings target room edit window.
      target_room_edit_window_->move(this->pos().x() + this->width()
                                     + autobot_edit_window_->width(),
                                     this->pos().y());
      target_room_edit_window_->resize(target_room_edit_window_->width(),
                                       autobot_edit_window_->height());
      //target_room_edit_window_->AddAllRoomsToView();
      // Brings speech edit window.
      target_room_edit_window_->show();

      target_speech_edit_window_->move(this->pos().x() + this->width(),
                                       this->pos().y()
                                       + autobot_edit_window_->height());
      target_speech_edit_window_->resize(autobot_edit_window_->width()
                                         + target_room_edit_window_->width(),
                                         this->height()
                                         - autobot_edit_window_->height());
      target_speech_edit_window_->AddAllSpeechToView();
      target_speech_edit_window_->show();
      SetSelectedAcountToManager();
    }
  }
}

void MainWindow::on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item,
                                                    int column) {
  if (item == nullptr) {
    autobot_edit_window_->hide();
  } else {
    // If this is top level item.
    if (item->parent() == nullptr) {
      if (autobot_edit_window_->isVisible()) {
        QString account_username = item->text(0);
        std::shared_ptr <AutobotAccount> bot_account_ptr
            = AutobotManager::GetInstance().Find(account_username);
        autobot_edit_window_->CombineAutobotAccount(bot_account_ptr);
      }
      SetSelectedAcountToManager();
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
  } else {
    return;
  }

  // Writing to a file
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox messagebox(this);
    messagebox.setText("无法打开文件");
    messagebox.exec();
  } else {
    QDomDocument document;
    QDomElement root = ConvertAutobotManagerToXML(
          AutobotManager::GetInstance(), &document);
    document.appendChild(root);
    QTextStream stream(&file);
    stream << document.toString();
    file.close();
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
  } else {
    return;
  }

  // Create a document to write XML
  QDomDocument document;

  // Open a file for reading
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))  {
    QMessageBox messagebox(this);
    messagebox.setText("无法打开文件");
    messagebox.exec();
  } else {
    QTextStream in(&file);
    // loading
    if(!document.setContent(in.readAll(), true)) {
      QMessageBox messagebox(this);
      messagebox.setText("无法读取文件内容或文件格式损坏");
      messagebox.exec();
    }
    file.close();
  }

  // Getting root element
  QDomElement root = document.firstChildElement();
  if (!ParseXMLToAutobotManager(root,
                               &AutobotManager::GetInstance())) {
    QMessageBox messagebox(this);
    messagebox.setText("无法解读文件格式");
    messagebox.exec();
  }
  AddManagerToView();
}

} //namespace




