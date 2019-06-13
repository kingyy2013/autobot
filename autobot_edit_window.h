#ifndef BOT_EDIT_WINDOW_H
#define BOT_EDIT_WINDOW_H

#include <QMainWindow>

namespace Ui {
  class BotEditWindow;
}

class BotEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit BotEditWindow(QWidget *parent = nullptr);
  ~BotEditWindow();

private:
  Ui::BotEditWindow *ui;
};

#endif // BOT_EDIT_WINDOW_H
