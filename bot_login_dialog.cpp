#include "autobot_login_dialog.h"
#include "ui_bot_login_dialog_form.h"

namespace autobot {

BotLoginDialog::BotLoginDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::BotLoginDialog) {
  ui->setupUi(this);  
}

BotLoginDialog::~BotLoginDialog() {
  delete ui;
}

QString BotLoginDialog::GetUsername() {
  return ui->lineEdit_username->text();
}

QString BotLoginDialog::GetPassword() {
  return ui->lineEdit_username->text();
}

void BotLoginDialog::on_pushButton_add_clicked() {
  close();
}

void BotLoginDialog::on_pushButton_cancel_clicked() {

}

} //namespace



