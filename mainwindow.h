#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTreeWidgetItem>

#include "autobot_manager.h"
#include "autobot_edit_window.h"
#include "target_room_edit_window.h"
#include "target_speech_edit_window.h"

namespace Ui {
class MainWindow;
class AutobotLoginDialog;
}

namespace autobot {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void Clear();
  void UpdateAllAccountToView();

private slots:
  void AddManagerToView();
  void RemoveRoomsFromUi(const QStringList& selected_rooms);

//  void UpdateAccountToCurrentView(const QString& account_name);
  void AddAccountFromUi();
  void on_pushButton_account_add_clicked();
  void on_pushButton_account_delete_clicked();
  void on_treeWidget_accounts_itemDoubleClicked(QTreeWidgetItem *item,
                                                int column);
  void on_treeWidget_accounts_itemClicked(QTreeWidgetItem *item, int column);

  void on_pushButton_saveall_clicked();

  void on_pushButton_loadall_clicked();


  void UpdateSelectedAccountsToView(const QStringList& selected_accounts);

  void on_treeWidget_accounts_itemSelectionChanged();

private:
  void SetSelectedAcountToManager();

  // Set the account to the tree view and the account manager.
  void SetAccountToView(const QString& account_name);

  Ui::MainWindow *ui;
  Ui::AutobotLoginDialog* autobot_login_dialog_ui_;
  QDialog* autobot_login_dialog_;
  AutobotEditWindow* autobot_edit_window_;
  TargetRoomEditWindow* target_room_edit_window_;
  TargetSpeechEditWindow* target_speech_edit_window_;
  QString last_directory_;

  QHash<QString, QTreeWidgetItem*> account_to_tree_item_map_;
  QHash<QString,
  QHash<QString, QTreeWidgetItem*>> room_account_to_tree_item_map_;
};

} // namespace

#endif // MAINWINDOW_H
