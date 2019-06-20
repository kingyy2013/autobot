#ifndef BOT_LOGIN_DIALOG_H
#define BOT_LOGIN_DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
  class AutobotLoginDialog;
}

namespace autobot {

class AutobotLoginDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AutobotLoginDialog(QWidget *parent = nullptr);
  QString GetUsername() const;
  QString GetPassword() const;
  Ui::AutobotLoginDialog* GetUI() const {return ui;}
  ~AutobotLoginDialog();

signals:
  void AddNewAccount(const QString&, const QString&);

private slots:

  void on_pushButton_add_clicked();

  void on_pushButton_cancel_clicked();

private:
  Ui::AutobotLoginDialog *ui;

};

} // namespace

#endif // BOT_LOGIN_DIALOG_H
