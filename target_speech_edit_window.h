#ifndef TARGET_SPEECH_EDIT_WINDOW_H
#define TARGET_SPEECH_EDIT_WINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class TargetSpeechEditWindow;
class TargetSpeechDialog;
}

namespace autobot {

class TargetSpeechEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit TargetSpeechEditWindow(QWidget *parent = nullptr);
  ~TargetSpeechEditWindow();

private slots:
  void on_pushButton_speech_words_new_clicked();
  void SpeechDialogAdd();

  void on_treeWidget_speech_names_itemClicked(QTreeWidgetItem *item);

  void on_pushButton_speech_words_save_clicked();

  void on_pushButton_speech_words_delete_clicked();

  void on_pushButton_speech_words_set_clicked();

private:
  Ui::TargetSpeechEditWindow *ui;
  Ui::TargetSpeechDialog* dialog_ui_form;
  QDialog *dialog_ui;
  // Container to store previous selected speech name.
  QTreeWidgetItem* prev_list_widge_;
};
}// namespace

#endif // TARGET_SPEECH_EDIT_WINDOW_H
