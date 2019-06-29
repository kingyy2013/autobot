#include <QMessageBox>

#include "target_room_edit_window.h"
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
          SLOT(AssignTargetRoomToTarget()));
  connect(target_room_dialog_ui_->pushButton_cancel, SIGNAL(clicked()),
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

void TargetRoomEditWindow::on_pushButton_add_target_clicked() {
  target_room_dialog_->exec();
}

void TargetRoomEditWindow::on_pushButton_remove_target_clicked() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_targets->selectedItems();
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
//        autobot_account_ptr_->RemoveTargetRoom(item->text(0));
        delete item;
      }
    }
  }
}


void TargetRoomEditWindow::AssignTargetRoomToTarget() {
  const QString& target_room_str =
      target_room_dialog_ui_->lineEdit_room->text();
  QRegExp rx("[, ]");// match a comma or a space
  QStringList target_room_list
      = target_room_str.split(rx, QString::SkipEmptyParts);
  QString error_message;
  for (const auto& room_str : target_room_list) {
//    // Assgin the added room to the current account's edit window.
//    if(!autobot_account_ptr_->GetTargetRoomMap().contains(room_str)) {
//      QTreeWidgetItem *target_room_item
//          = new QTreeWidgetItem(ui->treeWidget_targets);
//      target_room_item->setText(0, room_str);
//      ui->treeWidget_targets->addTopLevelItem(target_room_item);
//    }

    // Assgin the added room to all the selected accounts.
    const auto& selected_accounts =
        AutobotManager::GetInstance().GetSelectedAcountNames();
    for (const auto& selected_account : selected_accounts) {
      auto autobot_account_ptr
          = AutobotManager::GetInstance().Find(selected_account);
      if(!autobot_account_ptr->GetTargetRoomMap().contains(room_str)) {
        autobot_account_ptr->AssignTargetRoom(
              std::make_shared<TargetRoom>(room_str));
      } else {
        error_message.append(" 房间：" + room_str + " 到 " + " 机器人："
                             + selected_account + "\n");
      }
    }
  }
  if (!error_message.isEmpty()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加，\n" + error_message + " 已存在！");
    messagebox.exec();
  } else {
    target_room_dialog_->close();
  }
}

} // namespace
