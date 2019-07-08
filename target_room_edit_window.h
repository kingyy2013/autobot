#ifndef TARGET_ROOM_EDIT_WINDOW_H
#define TARGET_ROOM_EDIT_WINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

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
  void UpdateAllRoomsToView();
  void Clear();

signals:
  void FigureAssignedRoomSelection(const QString& room_name);
  void FigureAttachedSpeechSelection(const QString& speech_name);

public slots:
  void SetRoomSelection(const QString&);
  void SetSpeechSelection(const QString&);

private slots:
  void SetRoomToView(const QString& room_name);
  void AddRoomFromDialog();
  void RemoveSpeechsFromUi(const QStringList& selected_speechs);
  void UpdateSelectedRoomsToView(const QStringList& selected_rooms);

  void on_pushButton_add_room_clicked();

  void on_pushButton_remove_room_clicked();

  void on_pushButton_set_room_clicked();

  void on_treeWidget_rooms_itemSelectionChanged();

  void on_treeWidget_rooms_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
  QStringList GetSelectedItemNames(bool top_level);
  Ui::TargetRoomEditWindow *ui;
  Ui::TargetRoomDialog* target_room_dialog_ui_;
  QDialog* target_room_dialog_;
  QHash<QString, QTreeWidgetItem*> room_to_tree_item_map_;
  QHash<QString, QHash<QString, QTreeWidgetItem*>>
  speech_to_room_tree_item_map_;
};

} // namespace

#endif // TARGET_ROOM_EDIT_WINDOW_H
