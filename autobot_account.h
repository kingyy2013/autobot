#ifndef BOT_ACCOUNT_H
#define BOT_ACCOUNT_H

#include <string>

namespace auto_bot {
enum class AccountStatus {
  UNINITIALIZED,
  IDLE,
  WORKING,
  ERROR,
};

// The container for the bot accounts.
class AutobotAccount {
 public:
  AutobotAccount();
  bool Login();
  bool Speak();
  bool Logoff();
 private:
  std::string bot_name;
  std::string bot_password;
  int bot_status;
}; // AutobotAccount

} // namespace

#endif // BOT_ACCOUNT_H
