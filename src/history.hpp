#pragma once

#include <string>
#include <vector>

class History {
  public:
    History();

    void writeOnExit();
    void listHistory(int num_entries = 0);
    void parseHistoryFlag(const std::vector<std::string> &args);

  private:
    void readHistoryFromFile(const char *filename);
    void writeHistoryToFile(const char *filename);
    void appendHistoryToFile(const char *filename);
};
