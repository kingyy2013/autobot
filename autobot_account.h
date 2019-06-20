#ifndef BOT_ACCOUNT_H
#define BOT_ACCOUNT_H

#include <QString>

namespace autobot {
enum class AccountStatus {
  UNINITIALIZED = 0,
  IDLE,
  WORKING,
  ERROR,
};

// The container for the bot accounts.
class AutobotAccount {
public:
  AutobotAccount(const QString& username, const QString& password);
  bool Login();
  bool Speak();
  bool Logoff();
  QString GetUsername();
  QString GetPassword();
  void SetUsername(const QString&);
  void SetPassword(const QString&);

  AccountStatus GetStatus();
private:
  QString bot_username_;
  QString bot_password_;
  AccountStatus bot_status_;
}; // AutobotAccount

} // namespace

#endif // BOT_ACCOUNT_H
