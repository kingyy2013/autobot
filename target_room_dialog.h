#ifndef TARGET_ROOM_DIALOG_H
#define TARGET_ROOM_DIALOG_H

#include <QDialog>

namespace Ui {
class TargetRoomDialog;
}

namespace autobot {

class TargetRoomDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TargetRoomDialog(QWidget *parent = nullptr);
  ~TargetRoomDialog();

private:
  Ui::TargetRoomDialog *ui;
};
} // namespace

#endif // TARGET_ROOM_DIALOG_H
