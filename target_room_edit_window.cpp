#include <QMessageBox>
#include <QDebug>

#include "target_room_edit_window.h"
#include "target_room.h"
#include "autobot_account.h"

#include "autobot_manager.h"

#include "ui_target_room_edit_window_form.h"
#include "ui_target_room_dialog_form.h"

namespace autobot {

TargetRoomEditWindow::TargetRoomEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TargetRoomEditWindow),
  target_room_dialog_ui_(new Ui::TargetRoomDialog),
  target_room_dialog_(new QDialog()) {
  ui->setupUi(this);
  target_room_dialog_ui_->setupUi(target_room_dialog_);
  connect(target_room_dialog_ui_->pushButton_add, SIGNAL(clicked()), this,
          SLOT(AddRoomFromDialog()));
  connect(target_room_dialog_ui_->pushButton_add, SIGNAL(clicked()),
          target_room_dialog_, SLOT(close()));
  connect(target_room_dialog_ui_->pushButton_cancel, SIGNAL(clicked()),
          target_room_dialog_, SLOT(close()));

  connect(&(AutobotManager::GetInstance()),
          SIGNAL(RoomsChanged(const QStringList&)), this,
          SLOT(UpdateSelectedRoomsToView(const QStringList&)));

  connect(&(AutobotManager::GetInstance()),
          SIGNAL(SpeechsRemoved(const QStringList&)), this,
          SLOT(RemoveSpeechsFromUi(const QStringList&)));
}

TargetRoomEditWindow::~TargetRoomEditWindow()
{
  delete target_room_dialog_ui_;
  delete target_room_dialog_;
  delete ui;
}


void TargetRoomEditWindow::RemoveSpeechsFromUi(
    const QStringList& selected_speechs) {
  for (const QString& speech_name : selected_speechs) {
    if (speech_to_room_tree_item_map_.contains(speech_name)) {
      // Removes tree widget item.
      for (const auto tree_item_itr
           : speech_to_room_tree_item_map_[speech_name]) {
        delete tree_item_itr;
      }
      speech_to_room_tree_item_map_.remove(speech_name);
    }
  }
}

QStringList TargetRoomEditWindow::GetSelectedItemNames (bool top_level) {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_rooms->selectedItems();
  QStringList selected_names;
  foreach(QTreeWidgetItem * item, selected_items) {
    // Selected names when.
    // 1. Top-level item && requested top_level.
    // 2. Bottom-level item && not requested top_level.
    if((item->parent() == nullptr) ^ !top_level) {
      selected_names.append(item->text(0));
    }
  }
  return selected_names;
}

void TargetRoomEditWindow::on_pushButton_add_room_clicked() {
  target_room_dialog_->exec();
}

void TargetRoomEditWindow::on_pushButton_remove_room_clicked() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_rooms->selectedItems();
  QString account_list_msg;
  foreach(QTreeWidgetItem * item, selected_items) {
    account_list_msg.append(item->text(0));
    account_list_msg.append(" ");
  }
  // If any room got selected.
  if (account_list_msg != nullptr) {
    QMessageBox messagebox(this);
    messagebox.setWindowTitle("");
    messagebox.setText("您确定删除： " + account_list_msg + "?");
    messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
    messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
    // Not so sure why Accept role coresponds to 0, but reject corepsonds to 1.
    QStringList selected_room_names;
    if (messagebox.exec() == false) {
      foreach(QTreeWidgetItem * item, selected_items)  {
        if (item->parent() == nullptr) {
          // If this is the top level item (account).
          selected_room_names.append(item->text(0));
          room_to_tree_item_map_.remove(item->text(0));
          AutobotManager::GetRooms().Remove(item->text(0));
          delete item;
        } else {
          // Remove the rooms.
          const QString speech_name = item->text(0);
          const QString room_name = item->parent()->text(0);
          AutobotManager::GetSpeechs().BreakUpper(speech_name,
                                                room_name);
          // Removes tree widget item.
          delete speech_to_room_tree_item_map_[speech_name][room_name];
          speech_to_room_tree_item_map_[speech_name].remove(room_name);
        }
      }
    }
    emit AutobotManager::GetInstance().RoomsRemoved(selected_room_names);
  }
}

void TargetRoomEditWindow::UpdateSelectedRoomsToView(
    const QStringList& selected_rooms) {
  for (const auto& selected_room_name : selected_rooms) {
    SetRoomToView(selected_room_name);
  }
}

void TargetRoomEditWindow::SetRoomToView(const QString& room_name) {
  QTreeWidgetItem *room_item;
  const auto& room_to_tree_item_map_itr_
      = room_to_tree_item_map_.find(room_name);
  if (room_to_tree_item_map_itr_ != room_to_tree_item_map_.end()) {
    room_item = room_to_tree_item_map_itr_.value();
  } else {
    room_item = new QTreeWidgetItem(ui->treeWidget_rooms);
    room_to_tree_item_map_[room_name] = room_item;
    ui->treeWidget_rooms->addTopLevelItem(room_item);
  }

  const std::shared_ptr<TargetRoom>& room_ptr
      = AutobotManager::GetRooms().GetUnitPtr(room_name);
  room_item->setText(0, room_ptr->GetRoomNumber());
  room_item->setText(1,
                     QString::number(room_ptr->GetAssignedAccountSet().size()));

  const auto speech_room_set = room_ptr->GetSpeechNameSet();
  for (const auto& target_speech : speech_room_set) {
    const QString& speech_name = target_speech->GetUnitName();
    const auto& speech_to_room_tree_item_map_itr
        = speech_to_room_tree_item_map_.find(speech_name);
    QTreeWidgetItem *target_speech_item;
    if (speech_to_room_tree_item_map_itr
        == speech_to_room_tree_item_map_.end()) {
      target_speech_item = new QTreeWidgetItem(room_item);
      speech_to_room_tree_item_map_[speech_name]
          = QHash<QString, QTreeWidgetItem*>(
      {{room_name, target_speech_item}});
    } else {
       const auto& speech_room_to_tree_item_map_itr_
           = speech_to_room_tree_item_map_itr->find(room_name);
      if (speech_room_to_tree_item_map_itr_
          == speech_to_room_tree_item_map_itr->end()) {
        target_speech_item = new QTreeWidgetItem(room_item);
        speech_to_room_tree_item_map_[speech_name][room_name]
            = target_speech_item;
      } else {
        target_speech_item = speech_room_to_tree_item_map_itr_.value();
      }
    }
    target_speech_item->setText(0, target_speech->GetUnitName());
  }
}

void TargetRoomEditWindow::AddRoomFromDialog() {
  const QString& target_room_str =
      target_room_dialog_ui_->lineEdit_room->text();
  QRegExp rx("[, ]");// match a comma or a space
  QStringList target_room_list
      = target_room_str.split(rx, QString::SkipEmptyParts);
  QString error_message;
  for (const auto& room_str : target_room_list) {
    if(AutobotManager::GetRooms().GetUnitPtr(room_str) == nullptr) {
      AutobotManager::GetRooms().Add(std::make_shared<TargetRoom>(room_str));
      SetRoomToView(room_str);
    } else {
      error_message.append(" 房间：" + room_str + "\n");
    }
  }
  if(!error_message.isEmpty()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加: \n" + error_message + " 已存在！");
    messagebox.exec();
  }
}

void autobot::TargetRoomEditWindow::on_pushButton_set_room_clicked() {
  QString error_message;
  if (!AutobotManager::GetInstance().AssignSelectedRoomsToSelectedAccounts()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加，\n" + error_message + " 已存在！");
    messagebox.exec();
  }
  QStringList selected_account_names
      = AutobotManager::GetAccounts().GetSelectedNames();
  QStringList selected_room_names
      = AutobotManager::GetRooms().GetSelectedNames();
  emit AutobotManager::GetInstance().AccountsChanged(selected_account_names);
  emit AutobotManager::GetInstance().RoomsChanged(selected_room_names);
}


void TargetRoomEditWindow::on_treeWidget_rooms_itemSelectionChanged() {
  QStringList selected_room_names = GetSelectedItemNames(true);
  AutobotManager::GetRooms().SetSelectedNames(selected_room_names);
}

} // namespace


