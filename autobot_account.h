#ifndef BOT_ACCOUNT_H
#define BOT_ACCOUNT_H

#include <QString>

namespace autobot {
enum class AccountStatus {
  UNINITIALIZED,
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
private:
  QString bot_username_;
  QString bot_password_;
  int bot_status;
}; // AutobotAccount

} // namespace

#endif // BOT_ACCOUNT_H
