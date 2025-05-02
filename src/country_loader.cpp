#include "country_loader.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

CountryLoader::CountryLoader() : m_cancelled(false) {}

CountryLoader::~CountryLoader() {
  m_cancelled = true;
  if (m_worker_thread.joinable()) {
    m_worker_thread.join();
  }
}

void CountryLoader::load_async(const std::string &filename) {
  m_cancelled = false;
  m_worker_thread =
      std::thread(&CountryLoader::worker_thread_func, this, filename);
}

void CountryLoader::worker_thread_func(const std::string &filename) {
  try {
    std::vector<std::pair<std::string, std::string>> temp;

    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file: " + filename);
    }

    json data = json::parse(file);
    for (const auto &country : data) {
      if (m_cancelled)
        break;

      std::string name, code;
      if (country.contains("name") && country["name"].contains("common")) {
        name = country["name"]["common"];
      }
      if (country.contains("idd") && country["idd"].contains("root")) {
        code = country["idd"]["root"];
      }

      if (!name.empty() && !code.empty()) {
        temp.emplace_back(std::move(name), std::move(code));
      }
    }

    {
      std::lock_guard lock(m_data_mutex);
      m_loaded_data.swap(temp);
    }

    m_dispatcher.emit();
  } catch (...) {
    std::lock_guard lock(m_data_mutex);
    m_error_flag = true;
    m_dispatcher.emit();
  }
}

std::vector<std::pair<std::string, std::string>>
CountryLoader::get_data() const {
  std::lock_guard lock(m_data_mutex);
  return m_loaded_data;
}

bool CountryLoader::has_error() const {
  std::lock_guard lock(m_data_mutex);
  return m_error_flag;
}

Glib::Dispatcher &CountryLoader::get_dispatcher() { return m_dispatcher; }