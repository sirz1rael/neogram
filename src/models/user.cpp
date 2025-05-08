#include "models/user.h"

User::User(const std::int64_t &id, const std::string &username,
           const std::string &first_name, const std::string &last_name) {
  this->id_ = id;
  this->username_ = username;
  this->first_name_ = first_name;
  this->last_name_ = last_name;
}

std::int64_t User::get_id() { return this->id_; }
void User::set_id(const std::int64_t &id) { this->id_ = id; }

std::string User::get_username() { return this->username_; }

std::string User::get_first_name() { return this->first_name_; }

std::string User::get_last_name() { return this->last_name_; }