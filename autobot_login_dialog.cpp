#include "autobot_login_dialog.h"
#include "ui_autobot_login_dialog_form.h"

namespace autobot {

AutobotLoginDialog::AutobotLoginDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AutobotLoginDialog) {
  ui->setupUi(this);
}

AutobotLoginDialog::~AutobotLoginDialog() {
  delete ui;
}

QString AutobotLoginDialog::GetUsername() const {
  return ui->lineEdit_username->text();
}

QString AutobotLoginDialog::GetPassword() const {
  return ui->lineEdit_username->text();
}

void AutobotLoginDialog::on_pushButton_add_clicked() {
  close();
}

void AutobotLoginDialog::on_pushButton_cancel_clicked() {
  close();
}

} //namespace



