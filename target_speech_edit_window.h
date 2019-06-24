#ifndef TARGET_SPEECH_EDIT_WINDOW_H
#define TARGET_SPEECH_EDIT_WINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

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
  void on_pushButton_new_clicked();
  void SpeechDialogAdd();

  void on_listWidget_speech_names_itemClicked(QListWidgetItem *item);

private:
  Ui::TargetSpeechEditWindow *ui;
  Ui::TargetSpeechDialog* dialog_ui_form;
  QDialog *dialog_ui;
  // Container to store previous selected speech name.
  QListWidgetItem* prev_list_widge_;
};
}// namespace

#endif // TARGET_SPEECH_EDIT_WINDOW_H
