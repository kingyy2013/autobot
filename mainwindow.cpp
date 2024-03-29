#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_autobot_login_dialog_form.h"

#include "autobot_helper.h"
#include "autobot_manager.h"
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
          SLOT(UpdateSelectedAccountsToView(const QStringList&)));

  connect(&(AutobotManager::GetInstance()),
          SIGNAL(RoomsRemoved(const QStringList&)),
          this, SLOT(RemoveRoomsFromUi(const QStringList&)));

  // Double click on room edit window will select all rooms in account edit
  // window.
  connect(target_room_edit_window_, SIGNAL(
            FigureAssignedRoomSelection(const QString&)),
          this, SLOT(SetRoomSelection(const QString&)));

  // Double click on room will select room in room edit window.
  connect(this, SIGNAL(FigureAttachedRoomSelection(const QString&)),
          target_room_edit_window_,
          SLOT(SetRoomSelection(const QString&)));

  // Double click on speech in room edit window will select the speech in speech
  // edit window.
  connect(target_room_edit_window_, SIGNAL(
            FigureAttachedSpeechSelection(const QString&)),
          target_speech_edit_window_,
          SLOT(SetSpeechSelection(const QString&)));

  // Double click on speech in room edit window will select the speech in speech
  // edit window.
  connect(target_speech_edit_window_, SIGNAL(
            FigureAssignedSpeechSelection(const QString&)),
          target_room_edit_window_,
          SLOT(SetSpeechSelection(const QString&)));
}

MainWindow::~MainWindow() {
  delete autobot_login_dialog_ui_;
  delete autobot_login_dialog_;
  delete ui;
}

void MainWindow::Clear() {
  account_to_tree_item_map_.clear();
  room_account_to_tree_item_map_.clear();
  ui->treeWidget_accounts->clear();
}

void MainWindow::on_pushButton_account_add_clicked() {
  autobot_login_dialog_->exec();
}

void MainWindow::UpdateAllAccountToView() {
  Clear();
  for (auto account_ptr : AutobotManager::GetAccounts().GetUnitDict()) {
    SetAccountToView(account_ptr->GetUnitName());
  }
}

void MainWindow::SetRoomSelection(const QString& room_name) {
  // For unknown reason, I can't refresh the treeWidget manually, therefore,
  // on_treeWidget_accounts_itemSelectionChanged will be triggered mutiple
  // times.
  ui->treeWidget_accounts->selectionModel()->clearSelection();
  for (const auto& tree_widget : room_account_to_tree_item_map_[room_name]) {
    tree_widget->parent()->setExpanded(true);
    tree_widget->setSelected(true);
  }
}

void MainWindow::UpdateSelectedAccountsToView(
    const QStringList& selected_accounts) {
  for (const auto& selected_account_name : selected_accounts) {
    SetAccountToView(selected_account_name);
  }
}

void MainWindow::SetAccountToView(const QString& account_name) {
  QTreeWidgetItem *autobot_item;
  const auto& account_to_tree_item_map_itr_
      = account_to_tree_item_map_.find(account_name);
  if (account_to_tree_item_map_itr_ != account_to_tree_item_map_.end()) {
    autobot_item = account_to_tree_item_map_itr_.value();
  } else {
    autobot_item = new QTreeWidgetItem(ui->treeWidget_accounts);
    account_to_tree_item_map_[account_name] = autobot_item;
    ui->treeWidget_accounts->addTopLevelItem(autobot_item);
  }

  const std::shared_ptr<AutobotAccount>& account_ptr
      = AutobotManager::GetAccounts().GetUnitPtr(account_name);
  autobot_item->setText(0, account_ptr->GetUsername());
  autobot_item->setText(1, account_ptr->GetNickname());
  QPair<QString, QColor> status_and_color
      = ConvertStatusToQStringAndColor(account_ptr->GetStatus());
  autobot_item->setText(3,
                        status_and_color.first);
  autobot_item->setBackground(0, QBrush(status_and_color.second));
  autobot_item->setBackground(1, QBrush(status_and_color.second));
  autobot_item->setBackground(2, QBrush(status_and_color.second));
  autobot_item->setBackground(3, QBrush(status_and_color.second));
  const auto taregt_room_set = account_ptr->GetAttachedRoomSet();
  for (const auto& target_room : taregt_room_set) {
    const QString& room_name = target_room->GetUnitName();
    const auto& room_to_account_tree_item_map_itr_
        = room_account_to_tree_item_map_.find(room_name);
    QTreeWidgetItem *target_room_item;
    if (room_to_account_tree_item_map_itr_
        == room_account_to_tree_item_map_.end()) {
      target_room_item = new QTreeWidgetItem(autobot_item);
      room_account_to_tree_item_map_[room_name]
          = QHash<QString, QTreeWidgetItem*>(
      {{account_name, target_room_item}});
    } else {
       const auto& room_account_to_tree_item_map_itr_
           = room_to_account_tree_item_map_itr_->find(account_name);
      if (room_account_to_tree_item_map_itr_
          == room_to_account_tree_item_map_itr_->end()) {
        target_room_item = new QTreeWidgetItem(autobot_item);
        room_account_to_tree_item_map_[room_name][account_name]
            = target_room_item;
      } else {
        target_room_item = room_account_to_tree_item_map_itr_.value();
      }
    }
    target_room_item->setText(0, target_room->GetUnitName());
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
    if (AutobotManager::GetAccounts().GetUnitPtr(account_username) != nullptr) {
      messagebox.setText("无法添加，账号： " + account_username + " 已存在！");
      messagebox.exec();
    } else {
      std::shared_ptr<AutobotAccount> temp_account_ptr
          = std::make_shared<AutobotAccount>(account_username,
                                             account_password);
      AutobotManager::GetAccounts().Add(temp_account_ptr);
      SetAccountToView(account_username);
      autobot_login_dialog_->close();
    }
  }
}

void MainWindow::AddManagerToView() {
  for (const auto& autobot_account :
       AutobotManager::GetAccounts().GetUnitDict()) {
    SetAccountToView(autobot_account->GetUnitName());
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
      QSet<QString> selected_rooms;
      foreach(QTreeWidgetItem * item, selected_items)  {
        if (item->parent() == nullptr) {
          // If this is the top level item (account).
          // Remove the account.
          QString account_name = item->text(0);
          account_to_tree_item_map_.remove(account_name);
          const auto& account_ptr =
              AutobotManager::GetAccounts().GetUnitPtr(account_name);
          for (const auto& room_name
               : account_ptr->GetAttachedRoomSet().keys()) {
            delete room_account_to_tree_item_map_[room_name][account_name];
            room_account_to_tree_item_map_[room_name].remove(account_name);
            selected_rooms.insert(room_name);
          }
          AutobotManager::GetAccounts().Remove(account_name);
          delete item;
        } else {
          // Break the rooms.
          const QString room_name = item->text(0);
          const QString account_name = item->parent()->text(0);
          AutobotManager::GetRooms().BreakUpper(room_name,
                                                account_name);
          delete room_account_to_tree_item_map_[room_name][account_name];
          room_account_to_tree_item_map_[room_name].remove(account_name);
          selected_rooms.insert(room_name);
        }
      }
      // Update room info if neccssary.
      if (!selected_rooms.empty()) {
        emit AutobotManager::GetInstance().
            RoomsChanged(selected_rooms.toList());
      }
    }
  }
}

// Still keep the room, just remove it from UI.
void MainWindow::RemoveRoomsFromUi(const QStringList& selected_rooms) {
  for (const QString& room_name : selected_rooms) {
    if (room_account_to_tree_item_map_.contains(room_name)) {
      // Removes tree widget item.
      for (const auto tree_item_itr
           : room_account_to_tree_item_map_[room_name]) {
        delete tree_item_itr;
      }
      room_account_to_tree_item_map_.remove(room_name);
    }
  }
}

void MainWindow::SetSelectedAcountToManager() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_accounts->selectedItems();
  QStringList selected_item_names;
  for (auto item : selected_items) {
    if(item->parent()==nullptr) {
      selected_item_names.append(item->text(0));
    }
  }
  AutobotManager::GetAccounts().SetSelectedNames(selected_item_names);
}

void MainWindow::on_treeWidget_accounts_itemDoubleClicked(
    QTreeWidgetItem *item, int) {
  // If this is top level item.
  if (item->parent() == nullptr) {
    QString account_username = item->text(0);
    std::shared_ptr <AutobotAccount> bot_account_ptr
        = AutobotManager::GetAccounts().
        GetUnitPtr(account_username);
    if (bot_account_ptr == nullptr) {
      qFatal(("account name " + account_username + " is not in the manager")
             .toStdString().data());
    } else {
      // Brings account edit window.
      autobot_edit_window_->CombineAutobotAccount(account_username);
      autobot_edit_window_->move(this->pos().x() + this->width(),
                                 this->pos().y());
      autobot_edit_window_->show();
      // Brings target room edit window.
      target_room_edit_window_->move(this->pos().x() + this->width()
                                     + autobot_edit_window_->width(),
                                     this->pos().y());
      target_room_edit_window_->resize(target_room_edit_window_->width(),
                                       autobot_edit_window_->height());
      // Brings speech edit window.
      target_room_edit_window_->show();

      target_speech_edit_window_->move(this->pos().x() + this->width(),
                                       this->pos().y()
                                       + autobot_edit_window_->height());
      target_speech_edit_window_->resize(autobot_edit_window_->width()
                                         + target_room_edit_window_->width(),
                                         this->height()
                                         - autobot_edit_window_->height());
      target_speech_edit_window_->show();
      SetSelectedAcountToManager();

    }
  } else {
    // When selected on the room.
    emit FigureAttachedRoomSelection(item->text(0));
  }
}



void autobot::MainWindow::on_treeWidget_accounts_itemSelectionChanged() {
  SetSelectedAcountToManager();
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
        autobot_edit_window_->CombineAutobotAccount(account_username);
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
  AutobotManager::GetInstance().Clear();
  if (!ParseXMLToAutobotManager(root,
                               &AutobotManager::GetInstance())) {
    QMessageBox messagebox(this);
    messagebox.setText("无法解读文件格式");
    messagebox.exec();
  }
  AutobotManager::GetInstance().Print();
  this->UpdateAllAccountToView();
  target_room_edit_window_->UpdateAllRoomsToView();
  target_speech_edit_window_->UpdateAllSpeechToView();
}

} //namespace

