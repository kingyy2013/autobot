#include <QMessageBox>
#include <QDialog>

#include "autobot_edit_window.h"
#include "autobot_manager.h"

#include "ui_autobot_edit_window_form.h"

namespace autobot {
AutobotEditWindow::AutobotEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::AutobotEditWindow) {
  ui->setupUi(this);
  connect(ui->radioButton_freq_fix, SIGNAL(clicked()), this,
          SLOT(SetTaskConfig()));
  connect(ui->radioButton_freq_range, SIGNAL(clicked()), this,
          SLOT(SetTaskConfig()));
  connect(ui->radioButton_order_fix, SIGNAL(clicked()), this,
          SLOT(SetTaskConfig()));
  connect(ui->radioButton_order_random, SIGNAL(clicked()), this,
          SLOT(SetTaskConfig()));
  connect(ui->spinBox_freq_fix, SIGNAL(editingFinished()), this,
          SLOT(SetTaskConfig()));
  connect(ui->spinBox_freq_max, SIGNAL(editingFinished()), this,
          SLOT(SetTaskConfig()));
  connect(ui->spinBox_freq_min, SIGNAL(editingFinished()), this,
          SLOT(SetTaskConfig()));
}

AutobotEditWindow::~AutobotEditWindow() {
  delete ui;
}

void AutobotEditWindow::CombineAutobotAccount(const QString& account_name) {
  autobot_account_ptr_
      = AutobotManager::GetAccounts().GetUnitPtr(account_name);
  UpdateUI();
}

void AutobotEditWindow::UpdateUI() {
  ui->lineEdit_password->setText(autobot_account_ptr_->GetPassword());
  ui->lineEdit_nickname->setText(autobot_account_ptr_->GetNickname());
  ui->lineEdit_username->setText(autobot_account_ptr_->GetUsername());

  const auto& task_config = autobot_account_ptr_->GetTaskConfig();
  ui->spinBox_freq_fix->setValue(task_config.interval_seconds);
  ui->spinBox_freq_max->setValue(task_config.max_seconds);
  ui->spinBox_freq_min->setValue(task_config.min_seconds);

  if (task_config.fixed_interval == true) {
    ui->radioButton_freq_fix->setChecked(true);
    ui->spinBox_freq_fix->setDisabled(false);
    ui->spinBox_freq_max->setDisabled(true);
    ui->spinBox_freq_min->setDisabled(true);
  } else {
    ui->radioButton_freq_range->setChecked(true);
    ui->spinBox_freq_fix->setDisabled(true);
    ui->spinBox_freq_max->setDisabled(false);
    ui->spinBox_freq_min->setDisabled(false);
  }
  if (task_config.fixed_order == true) {
    ui->radioButton_order_fix->setChecked(true);
    ui->radioButton_order_random->setChecked(false);
  } else {
    ui->radioButton_order_random->setChecked(true);
    ui->radioButton_order_fix->setChecked(false);
  }
}


void AutobotEditWindow::on_pushButton_update_account_clicked() {
  autobot_account_ptr_->SetUsername(ui->lineEdit_username->text());
  autobot_account_ptr_->SetNickname(ui->lineEdit_nickname->text());
  autobot_account_ptr_->SetPassword(ui->lineEdit_password->text());
}

void AutobotEditWindow::SetTaskConfig() {
  TaskConfig task_config = autobot_account_ptr_->GetTaskConfig();
  if (ui->radioButton_freq_fix->isChecked()) {
    task_config.fixed_interval = true;
    task_config.interval_seconds = ui->spinBox_freq_fix->value();
    ui->spinBox_freq_fix->setDisabled(false);
    ui->spinBox_freq_max->setDisabled(true);
    ui->spinBox_freq_min->setDisabled(true);
  } else {
    task_config.fixed_interval = false;
    if (ui->spinBox_freq_min->value() > task_config.max_seconds) {
      ui->spinBox_freq_max->setValue(ui->spinBox_freq_min->value());
    }
    if (ui->spinBox_freq_max->value() < task_config.min_seconds) {
      ui->spinBox_freq_min->setValue(ui->spinBox_freq_max->value());
    }
    task_config.min_seconds = ui->spinBox_freq_min->value();
    task_config.max_seconds = ui->spinBox_freq_max->value();
    ui->spinBox_freq_fix->setDisabled(true);
    ui->spinBox_freq_max->setDisabled(false);
    ui->spinBox_freq_min->setDisabled(false);
  }
  if (ui->radioButton_order_fix->isChecked()) {
    task_config.fixed_order = true;
    ui->radioButton_order_random->setChecked(false);
  } else {
    task_config.fixed_order = false;
    ui->radioButton_order_fix->setChecked(false);
  }
  autobot_account_ptr_->SetTaskConfig(task_config);
}

}




