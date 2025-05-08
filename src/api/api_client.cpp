#include "api/api_client.h"
#include "api/auth_manager.h"
#include "common/enums.h"
#include <memory>
#include <stdexcept>
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>
#include <td/tl/TlObject.h>
#include <utility>

ClientApi::ClientApi() {
  td::ClientManager::execute(
      td_api::make_object<td_api::setLogVerbosityLevel>(1));
  client_manager_ = std::make_unique<td::ClientManager>();
  auth_manager_ = std::make_unique<AuthManager>();
  client_id_ = client_manager_->create_client_id();
  send_query(td_api::make_object<td_api::getOption>("version"), {});
}

void ClientApi::send_query(td_api::object_ptr<td_api::Function> f,
                           std::function<void(Object)> handler) {
  //...

  auto query_id = next_query_id();

  if (handler) {
    handlers_.emplace(query_id, std::move(handler));
  }

  client_manager_->send(client_id_, query_id, std::move(f));
}

std::uint64_t ClientApi::next_query_id() { return ++current_query_id_; }

bool ClientApi::isAuthorized() { return this->is_authorized_; }

auto ClientApi::create_authentication_query_handler() {
  return [this, id = authentication_query_id_](Object object) {
    if (id == authentication_query_id_) {
      check_authentication_error(std::move(object));
    }
  };
}

void ClientApi::check_authentication_error(Object object) {
  if (object->get_id() == td_api::error::ID) {
    auto error = td::move_tl_object_as<td_api::error>(object);
    std::cerr << "Error: " << to_string(error) << std::flush;
    on_authorization_state_update();
  }
}

void ClientApi::on_authorization_state_update() {
  authentication_query_id_++;
  td_api::downcast_call(
      *authorization_state_,
      overloaded(
          [this](td_api::authorizationStateReady &) {
            is_authorized_ = true;
            auth_manager_->set_authorization_state(AuthorizationState::READY);
            std::cout << "Authorization is completed" << std::endl;
          },
          [this](td_api::authorizationStateLoggingOut &) {
            is_authorized_ = false;
            auth_manager_->set_authorization_state(
                AuthorizationState::LOGGING_OUT);
            std::cout << "Logging out" << std::endl;
          },
          [this](td_api::authorizationStateClosing &) {
            auth_manager_->set_authorization_state(AuthorizationState::CLOSING);
            std::cout << "Closing" << std::endl;
          },
          [this](td_api::authorizationStateClosed &) {
            auth_manager_->set_authorization_state(AuthorizationState::CLOSED);
            is_authorized_ = false;
            need_restart_ = true;
            std::cout << "Terminated" << std::endl;
          },
          [this](td_api::authorizationStateWaitPhoneNumber &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_PHONE_NUMBER);
            if (!phone_number_.empty()) {
              std::cout << this->phone_number_ << std::flush;
              send_query(
                  td_api::make_object<td_api::setAuthenticationPhoneNumber>(
                      this->phone_number_, nullptr),
                  create_authentication_query_handler());
            } else {
              throw std::runtime_error("error....");
            }
          },
          [this](td_api::authorizationStateWaitPremiumPurchase &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_PREMIUM_PURCHASE);
            std::cout << "Telegram Premium subscription is required"
                      << std::endl;
          },
          [this](td_api::authorizationStateWaitEmailAddress &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_EMAIL_ADDRESS);
            std::cout << "Enter email address: " << std::flush;
            std::string email_address;
            std::cin >> email_address;
            send_query(
                td_api::make_object<td_api::setAuthenticationEmailAddress>(
                    email_address),
                create_authentication_query_handler());
          },
          [this](td_api::authorizationStateWaitEmailCode &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_EMAIL_CODE);
            std::cout << "Enter email authentication code: " << std::flush;
            std::string code;
            std::cin >> code;
            send_query(
                td_api::make_object<td_api::checkAuthenticationEmailCode>(
                    td_api::make_object<td_api::emailAddressAuthenticationCode>(
                        code)),
                create_authentication_query_handler());
          },
          [this](td_api::authorizationStateWaitCode &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_CODE);
            std::cout << "Enter authentication code: " << std::flush;
            std::string code;
            std::cin >> code;
            send_query(
                td_api::make_object<td_api::checkAuthenticationCode>(code),
                create_authentication_query_handler());
          },
          [this](td_api::authorizationStateWaitRegistration &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_REGISTRATION);
            std::string first_name;
            std::string last_name;
            std::cout << "Enter your first name: " << std::flush;
            std::cin >> first_name;
            std::cout << "Enter your last name: " << std::flush;
            std::cin >> last_name;
            send_query(td_api::make_object<td_api::registerUser>(
                           first_name, last_name, false),
                       create_authentication_query_handler());
          },
          [this](td_api::authorizationStateWaitPassword &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_PASSWORD);
            std::cout << "Enter authentication password: " << std::flush;
            std::string password;
            std::getline(std::cin, password);
            send_query(td_api::make_object<td_api::checkAuthenticationPassword>(
                           password),
                       create_authentication_query_handler());
          },
          [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_OTHER_DEVICE_CONFIRMATION);
            std::cout << "Confirm this login link on another device: "
                      << state.link_ << std::endl;
          },
          [this](td_api::authorizationStateWaitTdlibParameters &) {
            auth_manager_->set_authorization_state(
                AuthorizationState::WAIT_TDLIB_PARAMETERS);
            auto request = td_api::make_object<td_api::setTdlibParameters>();
            // TODO: App need to pick all confidential data from .env file!
            request->database_directory_ = "tdlib";
            request->use_message_database_ = true;
            request->use_secret_chats_ = true;
            request->api_id_ = 25187902;
            request->api_hash_ = "cea53cb75850c65db13cd7d89377ff0a";
            request->system_language_code_ = "en";
            request->device_model_ = "Desktop";
            request->application_version_ = "1.0";
            send_query(std::move(request),
                       create_authentication_query_handler());
          }));
}

void ClientApi::process_update(td_api::object_ptr<td_api::Object> update) {
  td_api::downcast_call(
      *update,
      overloaded(
          [this](td_api::updateAuthorizationState &update_authorization_state) {
            authorization_state_ =
                std::move(update_authorization_state.authorization_state_);
            on_authorization_state_update();
          },
          [this](td_api::updateNewChat &update_new_chat) {
            chat_title_[update_new_chat.chat_->id_] =
                update_new_chat.chat_->title_;
          },
          [this](td_api::updateChatTitle &update_chat_title) {
            chat_title_[update_chat_title.chat_id_] = update_chat_title.title_;
          },
          [this](td_api::updateUser &update_user) {
            auto user_id = update_user.user_->id_;
            users_[user_id] = std::move(update_user.user_);
          },
          [this](td_api::updateNewMessage &update_new_message) {
            auto chat_id = update_new_message.message_->chat_id_;
            std::string sender_name;
            td_api::downcast_call(
                *update_new_message.message_->sender_id_,
                overloaded(
                    [this, &sender_name](td_api::messageSenderUser &user) {
                      sender_name = get_user_name(user.user_id_);
                    },
                    [this, &sender_name](td_api::messageSenderChat &chat) {
                      sender_name = get_chat_title(chat.chat_id_);
                    }));
            std::string text;
            if (update_new_message.message_->content_->get_id() ==
                td_api::messageText::ID) {
              text = static_cast<td_api::messageText &>(
                         *update_new_message.message_->content_)
                         .text_->text_;
            }
            std::cout << "Receive message: [chat_id:" << chat_id
                      << "] [from:" << sender_name << "] [" << text << "]"
                      << std::endl;
          },
          [](auto &update) {}));
}

std::string ClientApi::get_user_name(std::int64_t user_id) const {
  //...
  auto it = users_.find(user_id);
  if (it == users_.end()) {
    return "unknown user";
  }
  return it->second->first_name_ + " " + it->second->last_name_;
}
std::string ClientApi::get_chat_title(std::int64_t chat_id) const {
  auto it = chat_title_.find(chat_id);
  if (it == chat_title_.end()) {
    return "unknown chat";
  }
  return it->second;
};

void ClientApi::process_response(td::ClientManager::Response response) {
  if (!response.object) {
    return;
  }

  if (response.request_id == 0) {
    return process_update(std::move(response.object));
  }

  auto it = handlers_.find(response.request_id);
  if (it != handlers_.end()) {
    it->second(std::move(response.object));
    handlers_.erase(it);
  }
}

bool ClientApi::send_auth_query(const std::string &phone_number) {
  this->phone_number_ = phone_number;
  if (phone_number.empty()) {
    return false;
  } else {
    process_response(client_manager_->receive(10));
  }
  return true;
}

ClientApi::~ClientApi() = default;