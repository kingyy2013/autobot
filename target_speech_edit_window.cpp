#include "target_speech_edit_window.h"
#include "ui_target_speech_edit_window_form.h"
#include "ui_target_speech_dialog_form.h"
#include "autobot_manager.h"

#include <QDebug>
#include <QMessageBox>

namespace autobot {

TargetSpeechEditWindow::TargetSpeechEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TargetSpeechEditWindow),
  dialog_ui_form(new Ui::TargetSpeechDialog),
  prev_list_widge_(nullptr) {
  ui->setupUi(this);
  dialog_ui = new QDialog();
  dialog_ui_form->setupUi(dialog_ui);
  connect(dialog_ui_form->pushButton_speech_add, SIGNAL(clicked()), this,
          SLOT(SpeechDialogAdd()));
  for (const auto& target_speech
       : AutobotManager::GetInstance().GetSpeechDict()) {
    ui->listWidget_speech_names->addItem(target_speech->SpeechName());
  }
}

TargetSpeechEditWindow::~TargetSpeechEditWindow() {
  delete ui;
}

void TargetSpeechEditWindow::on_pushButton_new_clicked() {
  dialog_ui->exec();
}

void TargetSpeechEditWindow::SpeechDialogAdd() {
  dialog_ui->close();
  QString speech_nickname_str = dialog_ui_form->lineEdit_speech->text();

  QRegExp rx("[, ]");// match a comma or a space
  QStringList speech_nickname_list
      = speech_nickname_str.split(rx, QString::SkipEmptyParts);
  QString error_message;
  for (const auto& speech_nickname : speech_nickname_list) {
    auto& speech_dict = AutobotManager::GetInstance().GetSpeechDictMutable();
    if(!speech_dict.contains(speech_nickname)) {
      speech_dict.insert(speech_nickname,
                         std::make_shared<TargetSpeech>(speech_nickname));
      ui->listWidget_speech_names->addItem(speech_nickname);
    } else {
      error_message.append(speech_nickname + " ");
    }
  }
  if (!error_message.isEmpty()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加，语言集名称： " + error_message + " 已存在！");
    messagebox.exec();
  }
}

void TargetSpeechEditWindow::on_listWidget_speech_names_itemClicked(
    QListWidgetItem *item) {
  if (item == prev_list_widge_) {
    return;
  }
  auto& speech_dict =
      AutobotManager::GetInstance().GetSpeechDictMutable();
  if (prev_list_widge_ != nullptr) {
    const QString& prev_speech_nickname = prev_list_widge_->text();
    const QStringList& prev_word_list
        = speech_dict[prev_speech_nickname]->GetWordsList();
    const QString& prev_speech_edit = ui->textEdit_speech_words->toPlainText();
    // Ask to save or not.
    if (prev_word_list.join("\n") != prev_speech_edit) {
      QMessageBox messagebox(this);
      messagebox.setWindowTitle("");
      messagebox.setText("确定保存 " + prev_list_widge_->text() + " 更改的内容吗？");
      messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
      messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
      if (messagebox.exec() == false) {
        speech_dict[prev_speech_nickname]->SetWordsList(
              ui->textEdit_speech_words->toPlainText().split("\n"));
      }
    }
  }

  const QString& speech_nickname = item->text();
  const QStringList& word_list = speech_dict[speech_nickname]->GetWordsList();
  ui->textEdit_speech_words->setPlainText(word_list.join("\n"));
  prev_list_widge_ = item;
  return;
}

}// namespace


