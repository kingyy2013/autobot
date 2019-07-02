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
}

TargetRoomEditWindow::~TargetRoomEditWindow()
{
  delete target_room_dialog_ui_;
  delete target_room_dialog_;
  delete ui;
}

QStringList TargetRoomEditWindow::GetSelectedItemNames (bool top_level) {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_rooms->selectedItems();
  QStringList selected_names;
  foreach(QTreeWidgetItem * item, selected_items) {
    // Selected names when.
    // 1. Top-level item && requested top_level.
    // 1. Bottom-level item && not requested top_level.
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
    // Selected rooms.
    if(item->parent() == nullptr) {
      account_list_msg.append(item->text(0));
      account_list_msg.append(" ");
    }
  }
  // If any room got selected.
  if (account_list_msg != nullptr) {
    QMessageBox messagebox(this);
    messagebox.setWindowTitle("");
    messagebox.setText("您确定删除： " + account_list_msg + "?");
    messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
    messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
    // Not so sure why Accept role coresponds to 0, but reject corepsonds to 1.
    if (messagebox.exec() == false) {
      foreach(QTreeWidgetItem * item, selected_items)  {
        AutobotManager::GetRooms().Remove(item->text(0));
        delete item;
      }
    }
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
      QTreeWidgetItem *target_room_item
          = new QTreeWidgetItem(ui->treeWidget_rooms);
      AutobotManager::GetRooms().Add(std::make_shared<TargetRoom>(room_str));
      target_room_item->setText(0, room_str);
      ui->treeWidget_rooms->addTopLevelItem(target_room_item);
      room_to_tree_item_map_[room_str] = target_room_item;
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
}


void TargetRoomEditWindow::on_treeWidget_rooms_itemSelectionChanged() {
  QStringList selected_room_names = GetSelectedItemNames(true);
  AutobotManager::GetRooms().SetSelectedNames(selected_room_names);
}

void autobot::TargetRoomEditWindow::on_pushButton_remove_speech_clicked() {
  QStringList selected_speechs = GetSelectedItemNames(false);
  // If any room got selected.
  if (!selected_speechs.isEmpty()) {
      // Break the assignment, but
      foreach(const QString& selected_speech, selected_speechs)  {
        const QString& upper_name
            = speech_to_tree_item_map_[selected_speech]->parent()->text(0);
        AutobotManager::GetSpeechs().BreakUpper(selected_speech, upper_name);
        delete speech_to_tree_item_map_[selected_speech];
        speech_to_tree_item_map_.remove(selected_speech);
    }
  }
}
} // namespace

