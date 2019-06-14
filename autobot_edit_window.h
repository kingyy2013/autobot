#ifndef BOT_EDIT_WINDOW_H
#define BOT_EDIT_WINDOW_H

#include <QMainWindow>
#include "autobot_account.h"
#include <memory>

namespace Ui {
  class AutobotEditWindow;
}

namespace autobot {

class AutobotEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit AutobotEditWindow(QWidget *parent = nullptr);
  void CombineAutobotAccount(std::shared_ptr<AutobotAccount> account);
  ~AutobotEditWindow();

private:
  Ui::AutobotEditWindow *ui;
  std::shared_ptr<AutobotAccount> autobot_account_ptr_;
};

}
#endif // BOT_EDIT_WINDOW_H
