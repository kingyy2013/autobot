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
    QTreeWidgetItem *speech_item
        = new QTreeWidgetItem(ui->treeWidget_speech_names);
    speech_item->setText(0, target_speech->SpeechName());
    ui->treeWidget_speech_names->addTopLevelItem(speech_item);
  }
  ui->textEdit_speech_words->setText(kDefaultInstruction);
}

TargetSpeechEditWindow::~TargetSpeechEditWindow() {
  delete ui;
}

void TargetSpeechEditWindow::on_pushButton_speech_words_new_clicked() {
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
    if(!AutobotManager::GetInstance().
       GetSpeechDict().contains(speech_nickname)) {
      AutobotManager::GetInstance()
          .AddSpeech(std::make_shared<TargetSpeech>(speech_nickname));
      QTreeWidgetItem *speech_item
          = new QTreeWidgetItem(ui->treeWidget_speech_names);
      speech_item->setText(0, speech_nickname);
      ui->treeWidget_speech_names->addTopLevelItem(speech_item);
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

void TargetSpeechEditWindow::on_treeWidget_speech_names_itemClicked(
    QTreeWidgetItem *item) {
  if (item == prev_list_widge_) {
    return;
  }
  const auto& speech_dict =
      AutobotManager::GetInstance().GetSpeechDict();
  if (prev_list_widge_ != nullptr) {
    const QString& prev_speech_nickname = prev_list_widge_->text(0);
    const QStringList& prev_word_list
        = speech_dict[prev_speech_nickname]->GetWordsList();
    const QString& prev_speech_edit = ui->textEdit_speech_words->toPlainText();
    // Ask to save or not.
    if (prev_word_list.join("\n") != prev_speech_edit) {
      QMessageBox messagebox(this);
      messagebox.setWindowTitle("");
      messagebox.setText("确定保存 " + prev_list_widge_->text(0) + " 更改的内容吗？");
      messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
      messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
      if (messagebox.exec() == false) {
        AutobotManager::GetInstance().SetSpeechContent(
              prev_speech_nickname,
              ui->textEdit_speech_words->toPlainText().split("\n"));
      }
    }
  }
  const QString& speech_nickname = item->text(0);
  const QStringList& word_list = speech_dict[speech_nickname]->GetWordsList();
  ui->textEdit_speech_words->setPlainText(word_list.join("\n"));
  prev_list_widge_ = item;
  return;
}

void TargetSpeechEditWindow::on_pushButton_speech_words_save_clicked() {
  if (ui->treeWidget_speech_names->currentItem() != nullptr) {
    const QString& curr_speech_nickname
        = ui->treeWidget_speech_names->currentItem()->text(0);
    AutobotManager::GetInstance().SetSpeechContent(
          curr_speech_nickname,
          ui->textEdit_speech_words->toPlainText().split("\n"));
  }
}

void TargetSpeechEditWindow::on_pushButton_speech_words_delete_clicked() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_speech_names->selectedItems();
  if (selected_items.empty() == false) {
    QString account_list_msg;
    foreach(QTreeWidgetItem * item, selected_items)  {
      if (item->text(0) != kDefaultSpeechName) {
        account_list_msg.append(item->text(0));
        account_list_msg.append(" ");
      }
    }
    if (!account_list_msg.isEmpty()) {
      QMessageBox messagebox(this);
      messagebox.setWindowTitle("");
      messagebox.setText("您确定删除： " + account_list_msg + "?");
      messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
      messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
      // Not so sure why Accept role coresponds to 0,
      // but reject corepsonds to 1.
      if (messagebox.exec() == false) {
        foreach(QTreeWidgetItem * item, selected_items)  {
          if (item->text(0) != kDefaultSpeechName) {
            AutobotManager::GetInstance().RemoveSpeech(item->text(0));
            prev_list_widge_ = nullptr;
            ui->textEdit_speech_words->setText(kDefaultInstruction);
            delete item;
          }
        }
      }
    } else {
      QMessageBox messagebox(this);
      messagebox.setText("不可删除： " + kDefaultSpeechName);
      messagebox.exec();
    }
  }
}

void TargetSpeechEditWindow::on_pushButton_speech_words_set_clicked() {
  const auto current_speech =  ui->treeWidget_speech_names->currentItem();
  if (current_speech != nullptr) {
    QStringList selected_account_names
        = AutobotManager::GetInstance().GetSelectedAcountNames();
    for (const auto &selected_name : selected_account_names) {
      AutobotManager::GetInstance().
          AssignSpeechToAccount(current_speech->text(0),
                                selected_name);
    }
  }
}

}// namespace
