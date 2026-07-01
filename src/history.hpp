#pragma once

#include <string>
#include <vector>

class History {
  public:
    History();
    void listHistory(int num_entries = 0);
    void parseHistoryFlag(int &numcmds, const std::vector<std::string> &args);

  private:
    void readHistoryFromFile(const char *filename);
    void writeHistoryToFile(const char *filename);
    void appendHistoryToFile(int &cmds_since_append, const char *filename);
};
