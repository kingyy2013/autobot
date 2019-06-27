#include <QMessageBox>
#include <QDialog>

#include "autobot_edit_window.h"
#include "ui_autobot_edit_window_form.h"
#include "ui_target_room_dialog_form.h"

namespace autobot {
AutobotEditWindow::AutobotEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::AutobotEditWindow),
  target_room_dialog_ui_(new Ui::TargetRoomDialog),
  target_room_dialog_(new QDialog()) {
  ui->setupUi(this);
  target_room_dialog_ui_->setupUi(target_room_dialog_);
  connect(target_room_dialog_ui_->pushButton_add, SIGNAL(clicked()), this,
          SLOT(AssignTargetRoomToTarget()));
  connect(target_room_dialog_ui_->pushButton_cancel, SIGNAL(clicked()),
          target_room_dialog_, SLOT(close()));
}

AutobotEditWindow::~AutobotEditWindow() {
  delete target_room_dialog_ui_;
  delete target_room_dialog_;
  delete ui;
}

void AutobotEditWindow::CombineAutobotAccount(
    const std::shared_ptr<AutobotAccount>& account) {
  autobot_account_ptr_ = account;
  UpdateUI();
}

void AutobotEditWindow::UpdateUI() {
  ui->lineEdit_password->setText(autobot_account_ptr_->GetPassword());
  ui->lineEdit_username->setText(autobot_account_ptr_->GetUsername());
  ui->treeWidget_targets->clear();
  for (const auto& str_to_target_room
       : autobot_account_ptr_->GetTargetRoomMap()) {
    QTreeWidgetItem *target_room_item
        = new QTreeWidgetItem(ui->treeWidget_targets);
    target_room_item->setText(0, str_to_target_room->GetRoomNumber());
    ui->treeWidget_targets->addTopLevelItem(target_room_item);
  }
}

void AutobotEditWindow::AssignTargetRoomToTarget() {
  const QString& target_room_str =
      target_room_dialog_ui_->lineEdit_room->text();
  QRegExp rx("[, ]");// match a comma or a space
  QStringList target_room_list
      = target_room_str.split(rx, QString::SkipEmptyParts);
  QString error_message;
  for (const auto& room_str : target_room_list) {
    if(!autobot_account_ptr_->GetTargetRoomMap().contains(room_str)) {
      autobot_account_ptr_->AssignTargetRoom(
            std::make_shared<TargetRoom>(room_str));
      QTreeWidgetItem *target_room_item
          = new QTreeWidgetItem(ui->treeWidget_targets);
      target_room_item->setText(0, room_str);
      ui->treeWidget_targets->addTopLevelItem(target_room_item);
      target_room_dialog_->close();
    } else {
      error_message.append(room_str + " ");
    }
  }
  if (!error_message.isEmpty()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加，账号： " + error_message + " 已存在！");
    messagebox.exec();
  }
}

void AutobotEditWindow::on_pushButton_remove_target_clicked() {
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
        autobot_account_ptr_->RemoveTargetRoom(item->text(0));
        delete item;
      }
    }
  }
}

void AutobotEditWindow::on_pushButton_update_account_clicked() {
  autobot_account_ptr_->SetUsername(ui->lineEdit_username->text());
  autobot_account_ptr_->SetPassword(ui->lineEdit_password->text());
}

void AutobotEditWindow::on_pushButton_add_target_clicked() {
  target_room_dialog_->exec();
}

}



