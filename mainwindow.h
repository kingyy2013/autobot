#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "autobot_login_dialog.h"

namespace Ui {
class MainWindow;
}

namespace autobot {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

  void on_pushButton_add_clicked();

private:
    Ui::MainWindow *ui;
    AutobotLoginDialog bot_log_dialog;
};

} // namespace

#endif // MAINWINDOW_H
