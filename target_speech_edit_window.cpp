#include "target_speech_edit_window.h"
#include "ui_target_speech_edit_window_form.h"
#include "ui_target_speech_dialog_form.h"
#include "autobot_manager.h"
#include "target_speech_set.h"

#include <QDebug>
#include <QMessageBox>

namespace autobot {

TargetSpeechEditWindow::TargetSpeechEditWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TargetSpeechEditWindow),
  speech_input_dialog_ui_(new Ui::TargetSpeechDialog),
  prev_list_widge_(nullptr) {
  ui->setupUi(this);
  speech_input_dialog_ = new QDialog();
  speech_input_dialog_ui_->setupUi(speech_input_dialog_);
  connect(speech_input_dialog_ui_->pushButton_speech_add, SIGNAL(clicked()), this,
          SLOT(SpeechDialogAdd()));
  connect(speech_input_dialog_ui_->pushButton_speech_cancel, SIGNAL(clicked()),
          speech_input_dialog_, SLOT(close()));
  UpdateAllSpeechToView();
  ui->textEdit_speech_words->setText(kDefaultInstruction);
}

TargetSpeechEditWindow::~TargetSpeechEditWindow() {
  delete speech_input_dialog_ui_;
  delete speech_input_dialog_;
  delete ui;
}


void TargetSpeechEditWindow::UpdateAllSpeechToView() {
  UpdateSelectedSpeechToView(
        AutobotManager::GetSpeechs().GetAllNames());
}

void TargetSpeechEditWindow::UpdateSelectedSpeechToView(
    const QStringList& selected_speechs) {
  for (const QString& selected_speech : selected_speechs) {
    SetSpeechToView(selected_speech);
  }
}

void TargetSpeechEditWindow::SetSpeechToView(const QString& speech_name) {
  QTreeWidgetItem *speech_tree_item;
  const auto& speech_to_tree_item_map_itr
      = speech_to_tree_item_map_.find(speech_name);
  if (speech_to_tree_item_map_itr != speech_to_tree_item_map_.end()) {
    speech_tree_item = speech_to_tree_item_map_itr.value();
  } else {
    speech_tree_item = new QTreeWidgetItem(ui->treeWidget_speech_names);
    speech_to_tree_item_map_[speech_name] = speech_tree_item;
  }
  speech_tree_item->setText(0, speech_name);
  ui->treeWidget_speech_names->addTopLevelItem(speech_tree_item);
}

void TargetSpeechEditWindow::on_pushButton_speech_words_new_clicked() {
  speech_input_dialog_->exec();
}

void TargetSpeechEditWindow::SpeechDialogAdd() {
  speech_input_dialog_->close();
  QString speech_nickname_str = speech_input_dialog_ui_->lineEdit_speech->text();

  QRegExp rx("[, ]");// match a comma or a space
  QStringList speech_nickname_list
      = speech_nickname_str.split(rx, QString::SkipEmptyParts);
  QString error_message;
  for (const auto& speech_nickname : speech_nickname_list) {
    if(!AutobotManager::GetSpeechs().GetUnitDict().contains(speech_nickname)) {
      AutobotManager::GetSpeechs()
          .Add(std::make_shared<TargetSpeech>(speech_nickname));
      const auto& speech_dict =
          AutobotManager::GetSpeechs().GetUnitDict();
      SetSpeechToView(speech_nickname);
    } else {
      error_message.append(speech_nickname + " ");
    }
  }

  if (!error_message.isEmpty()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加，语言集名称： " + error_message + " 已存在！");
    messagebox.exec();
  }

//  const QString& target_room_str =
//      target_room_dialog_ui_->lineEdit_room->text();
//  QRegExp rx("[, ]");// match a comma or a space
//  QStringList target_room_list
//      = target_room_str.split(rx, QString::SkipEmptyParts);
//  QString error_message;
//  for (const auto& room_str : target_room_list) {
//    if(AutobotManager::GetRooms().GetUnitPtr(room_str) == nullptr) {
//      QTreeWidgetItem *target_room_item
//          = new QTreeWidgetItem(ui->treeWidget_rooms);
//      AutobotManager::GetRooms().Add(std::make_shared<TargetRoom>(room_str));
//      target_room_item->setText(0, room_str);
//      ui->treeWidget_rooms->addTopLevelItem(target_room_item);
//      room_to_tree_item_map_[room_str] = target_room_item;
//    } else {
//      error_message.append(" 房间：" + room_str + "\n");
//    }
//  }
//  if(!error_message.isEmpty()) {
//    QMessageBox messagebox(this);
//    messagebox.setText("无法添加: \n" + error_message + " 已存在！");
//    messagebox.exec();
//  }
}

void TargetSpeechEditWindow::on_treeWidget_speech_names_itemClicked(
    QTreeWidgetItem *item) {
  if (item == prev_list_widge_) {
    return;
  }
  const auto& speech_dict =
      AutobotManager::GetSpeechs().GetUnitDict();
  if (prev_list_widge_ != nullptr) {
    const QString& prev_speech_nickname = prev_list_widge_->text(0);
    const QStringList& prev_word_list
        = std::static_pointer_cast<TargetSpeech>(speech_dict[prev_speech_nickname])->GetWordsList();
    const QString& prev_speech_edit = ui->textEdit_speech_words->toPlainText();
    // Ask to save or not.
    if (prev_word_list.join("\n") != prev_speech_edit) {
      QMessageBox messagebox(this);
      messagebox.setWindowTitle("");
      messagebox.setText("确定保存 " + prev_list_widge_->text(0) + " 更改的内容吗？");
      messagebox.addButton("确定", QMessageBox::ButtonRole::AcceptRole);
      messagebox.addButton("取消", QMessageBox::ButtonRole::RejectRole);
      if (messagebox.exec() == false) {
        AutobotManager::GetSpeechs().
            GetUnitPtr(prev_speech_nickname)->
              SetWordsList(ui->textEdit_speech_words->
                           toPlainText().split("\n"));
      }
    }
  }
  const QString& speech_nickname = item->text(0);
  const QStringList& word_list = AutobotManager::GetSpeechs().
      GetUnitPtr(speech_nickname)->GetWordsList();
  ui->textEdit_speech_words->setPlainText(word_list.join("\n"));
  prev_list_widge_ = item;
  return;
}

void TargetSpeechEditWindow::on_pushButton_speech_words_save_clicked() {
  if (ui->treeWidget_speech_names->currentItem() != nullptr) {
    const QString& curr_speech_nickname
        = ui->treeWidget_speech_names->currentItem()->text(0);
    AutobotManager::GetSpeechs().GetUnitPtr(curr_speech_nickname)->
        SetWordsList(ui->textEdit_speech_words->toPlainText().split("\n"));
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
            AutobotManager::GetSpeechs().Remove(item->text(0));
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

void TargetSpeechEditWindow::on_treeWidget_speech_names_itemSelectionChanged() {
  QList<QTreeWidgetItem*> selected_items
      = ui->treeWidget_speech_names->selectedItems();
  QStringList selected_speech_names;
  for (const auto& seleted_item : selected_items) {
    selected_speech_names.append(seleted_item->text(0));
  }
  AutobotManager::GetSpeechs().SetSelectedNames(selected_speech_names);
}


void TargetSpeechEditWindow::on_pushButton_speech_words_set_clicked() {
  QString error_message;
  if (!AutobotManager::GetInstance().AssignSelectedSpeechsToSelectedRooms()) {
    QMessageBox messagebox(this);
    messagebox.setText("无法添加，\n" + error_message + " 已存在！");
    messagebox.exec();
  }
  QStringList selected_room_names
      = AutobotManager::GetRooms().GetSelectedNames();
  emit AutobotManager::GetInstance().RoomsChanged(selected_room_names);
}

}// namespace

