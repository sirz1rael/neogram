#include "api/auth_manager.h"
#include "common/enums.h"

AuthManager::AuthManager() {}

void AuthManager::set_authorization_state(AuthorizationState auth_state) {
  this->current_authorization_state_ = auth_state;
}

AuthorizationState AuthManager::get_auth_state() {
  return this->current_authorization_state_;
}