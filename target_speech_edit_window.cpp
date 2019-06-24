#include "target_speech_edit_window.h"
#include "ui_target_speech_edit_window_form.h"
#include "ui_target_speech_dialog_form.h"
#include "autobot_manager.h"
#include <QDebug>

namespace autobot {

TargetSpeechEditWindow::TargetSpeechEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TargetSpeechEditWindow),
  dialog_ui_form(new Ui::TargetSpeechDialog) {
  ui->setupUi(this);
  dialog_ui = new QDialog();
  dialog_ui_form->setupUi(dialog_ui);
}

TargetSpeechEditWindow::~TargetSpeechEditWindow() {
  delete ui;
}


void TargetSpeechEditWindow::on_pushButton_new_clicked() {
  dialog_ui->exec();
}

void TargetSpeechEditWindow::on_pushButton_speech_add_clicked() {
  qDebug() << "123123123";
}
}// namespace
