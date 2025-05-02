#ifndef COUNTRY_LOADER_H
#define COUNTRY_LOADER_H

#include <atomic>
#include <glibmm/dispatcher.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class CountryLoader {
public:
  CountryLoader();
  ~CountryLoader();

  void load_async(const std::string &filename);
  std::vector<std::pair<std::string, std::string>> get_data() const;
  bool has_error() const;
  Glib::Dispatcher &get_dispatcher();

private:
  void worker_thread_func(const std::string &filename);

  mutable std::mutex m_data_mutex;
  std::atomic<bool> m_cancelled;
  std::thread m_worker_thread;
  Glib::Dispatcher m_dispatcher;
  std::vector<std::pair<std::string, std::string>> m_loaded_data;
  bool m_error_flag = false;
};

#endif // COUNTRY_LOADER_H