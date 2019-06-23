#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTreeWidgetItem>

#include "autobot_login_dialog.h"
#include "autobot_manager.h"
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
  void UpdateAccountToCurrentView(const std::shared_ptr<AutobotAccount>& account_ptr);
  void UpdateManagerToView();

  void AddAccount(const QString&, const QString&);
  void on_pushButton_add_clicked();
  void on_pushButton_delete_clicked();
  void on_treeWidget_accounts_itemDoubleClicked(QTreeWidgetItem *item,
                                                int column);
  void on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item, int column);

  void on_pushButton_saveall_clicked();

  void on_pushButton_loadall_clicked();

private:
    // Set the account to the tree view and the account manager.
    void SetAccountToView(
        const std::shared_ptr<AutobotAccount>& account_ptr,
        QTreeWidgetItem *autobot_item);

    Ui::MainWindow *ui;
    AutobotLoginDialog* bot_log_dialog_;
    AutobotEditWindow* autobot_edit_window_;
    QString last_directory_;

    QHash<QString, QTreeWidgetItem> account_to_tree_item_map_;
};

} // namespace

#endif // MAINWINDOW_H
