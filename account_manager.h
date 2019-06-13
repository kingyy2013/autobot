#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QThreadPool>

namespace autobot {

class AccountManager {
  AccountManager();

private:
  QThreadPool thread_pool_;
};
}// namespace
#endif // ACCOUNT_MANAGER_H
