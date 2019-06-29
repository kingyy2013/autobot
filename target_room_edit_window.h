#ifndef TARGET_ROOM_EDIT_WINDOW_H
#define TARGET_ROOM_EDIT_WINDOW_H

#include <QMainWindow>

namespace Ui {
class TargetRoomEditWindow;
class TargetRoomDialog;
}
namespace autobot {

class TargetRoomEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit TargetRoomEditWindow(QWidget *parent = nullptr);
  ~TargetRoomEditWindow();

private slots:
  void AssignTargetRoomToTarget();

  void on_pushButton_add_target_clicked();

  void on_pushButton_remove_target_clicked();

private:
  Ui::TargetRoomEditWindow *ui;
  Ui::TargetRoomDialog* target_room_dialog_ui_;
  QDialog* target_room_dialog_;
};

} // namespace

#endif // TARGET_ROOM_EDIT_WINDOW_H
