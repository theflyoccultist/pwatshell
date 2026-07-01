#pragma once

#include <string>
#include <vector>

class History {
  public:
    History();
    ~History();

    History(History &) = delete;
    History &operator=(History &) = delete;
    History(History &&) = delete;
    History &operator=(History &&) = delete;

    void listHistory(int num_entries = 0);
    void parseHistoryFlag(const std::vector<std::string> &args);

  private:
    void readHistoryFromFile(const char *filename);
    void writeHistoryToFile(const char *filename);
    void appendHistoryToFile(const char *filename);
};
