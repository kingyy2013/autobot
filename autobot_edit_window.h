#ifndef BOT_EDIT_WINDOW_H
#define BOT_EDIT_WINDOW_H

#include <QMainWindow>

namespace Ui {
  class AutobotEditWindow;
}

namespace autobot {

class AutobotEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit AutobotEditWindow(QWidget *parent = nullptr);
  ~AutobotEditWindow();

private:
  Ui::AutobotEditWindow *ui;
};
}
#endif // BOT_EDIT_WINDOW_H
