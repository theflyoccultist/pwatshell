#pragma once

#include "autocomplete.hpp"
#include <string>

class AutoCompleteManager {
  public:
    void completion(int &tabCount, std::string &usrInput);

  private:
    AutoComplete autocomplete;

    void execCompletion(int &tabCount, const std::vector<FileInfo> &matchingExecutables,
                        std::string &usrInput);
    void fileCompletion(int &tabCount, const std::vector<FileInfo> &matchingFiles,
                        std::string &usrInput, const std::string &fileName);
    void nestedFileCompletion(const std::vector<FileInfo> &matchingFiles, std::string &usrInput);
};
