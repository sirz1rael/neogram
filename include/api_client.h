#pragma once

#include "states.h"

class ApiClient {
public:
private:
  ClientState current_client_state = ClientState::UNAUTHORIZED;
};