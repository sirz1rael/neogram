#pragma once

enum class ClientState {
  UNAUTHORIZED,
  AUTHORIZED
  //... idk
};

enum class PageTypes {
  MAIN_LOGIN_PAGE,
  TWO_FACTOR_LOGIN_PAGE,
  QR_LOGIN_PAGE,
  CLOUD_PASSWORD_PAGE
};