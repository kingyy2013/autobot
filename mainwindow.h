#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTreeWidgetItem>

#include "autobot_login_dialog.h"
#include "account_manager.h"
#include "autobot_edit_window.h"

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
  void on_pushButton_delete_clicked();
  void on_treeWidget_accounts_itemDoubleClicked(QTreeWidgetItem *item,
                                                int column);
  void on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    AutobotLoginDialog bot_log_dialog_;
    AccountManager account_manager_;
    AutobotEditWindow* autobot_edit_window_;
};

} // namespace

#endif // MAINWINDOW_H
