#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "autobot_login_dialog.h"
#include "account_manager.h"

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
  void AddAccount(const QString&, const QString&);
  void on_pushButton_add_clicked();

private:
    QMessageBox messagebox;
    Ui::MainWindow *ui;
    AutobotLoginDialog bot_log_dialog;
    AccountManager account_manager;
};

} // namespace

#endif // MAINWINDOW_H
