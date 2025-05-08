#pragma once

#include <cstdint>
#include <string>
class User {
public:
  User(const std::int64_t &id, const std::string &username,
       const std::string &first_name, const std::string &last_name);
  ~User() = default;

  std::int64_t get_id();
  void set_id(const std::int64_t &id);

  std::string get_username();
  std::string get_first_name();
  std::string get_last_name();

private:
  std::int64_t id_;
  std::string username_;
  std::string first_name_;
  std::string last_name_;
};