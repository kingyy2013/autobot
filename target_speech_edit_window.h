#ifndef TARGET_SPEECH_EDIT_WINDOW_H
#define TARGET_SPEECH_EDIT_WINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class TargetSpeechEditWindow;
class TargetSpeechDialog;
}

namespace autobot {

class TargetSpeechEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit TargetSpeechEditWindow(QWidget *parent = nullptr);
  ~TargetSpeechEditWindow();

  void Clear();

signals:
  void FigureAssignedSpeechSelection(const QString& speech_name);

public slots:
  void SetSpeechSelection(const QString& speech_name);
  void UpdateAllSpeechToView();

private slots:
  void UpdateSelectedSpeechToView(const QStringList& selected_accounts);

  void SpeechDialogAdd();

  void on_pushButton_speech_words_new_clicked();

  void on_treeWidget_speech_names_itemClicked(QTreeWidgetItem *item);

  void on_pushButton_speech_words_save_clicked();

  void on_pushButton_speech_words_delete_clicked();

  void on_pushButton_speech_words_set_clicked();

  void on_treeWidget_speech_names_itemSelectionChanged();

  void on_treeWidget_speech_names_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
  void SetSpeechToView(const QString& speech_name);

  Ui::TargetSpeechEditWindow *ui;
  Ui::TargetSpeechDialog* speech_input_dialog_ui_;
  QDialog *speech_input_dialog_;
  // Container to store previous selected speech name.
  QTreeWidgetItem* prev_list_widge_;
  QHash<QString, QTreeWidgetItem*> speech_to_tree_item_map_;
};
}// namespace

#endif // TARGET_SPEECH_EDIT_WINDOW_H
