#include "target_room_dialog.h"
#include "ui_target_room_dialog_form.h"
namespace autobot {

TargetRoomDialog::TargetRoomDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TargetRoomDialog)
{
  ui->setupUi(this);
}

TargetRoomDialog::~TargetRoomDialog()
{
  delete ui;
}
} // namespace
