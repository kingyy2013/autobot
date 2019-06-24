#ifndef TARGET_SPEECH_EDIT_WINDOW_H
#define TARGET_SPEECH_EDIT_WINDOW_H

#include <QMainWindow>

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
  void on_pushButton_speech_add_clicked();

private:
  Ui::TargetSpeechEditWindow *ui;
  Ui::TargetSpeechDialog* dialog_ui_form;
  QDialog *dialog_ui;
};
}// namespace

#endif // TARGET_SPEECH_EDIT_WINDOW_H
