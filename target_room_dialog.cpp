#include "target_room_dialog.h"
#include "ui_target_room_dialog_form.h"
namespace autobot {

TargetRoomDialog::TargetRoomDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TargetRoomDialog) {
  ui->setupUi(this);
}

TargetRoomDialog::~TargetRoomDialog() {
  delete ui;
}

void TargetRoomDialog::on_pushButton_add_clicked() {
  emit AddNewRoom(ui->lineEdit_room->text());
  close();
}

void TargetRoomDialog::on_pushButton_cancel_clicked() {
  close();
}

} // namespace


