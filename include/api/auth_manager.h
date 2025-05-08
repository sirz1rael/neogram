#pragma once

#include "common/enums.h"
class AuthManager {
public:
  AuthManager();
  ~AuthManager() = default;

  void set_authorization_state(AuthorizationState auth_state);
  AuthorizationState get_auth_state();

private:
  ClientState client_state_ = ClientState::UNAUTHORIZED;
  AuthorizationState current_authorization_state_ = AuthorizationState::CLOSED;
};