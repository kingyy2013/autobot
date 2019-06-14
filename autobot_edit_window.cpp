#include "autobot_edit_window.h"
#include "ui_autobot_edit_window_form.h"

namespace autobot {
AutobotEditWindow::AutobotEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::AutobotEditWindow)
{
  ui->setupUi(this);
}

AutobotEditWindow::~AutobotEditWindow() {
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
}

}
