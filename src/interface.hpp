#pragma once

#include "parser.hpp"
#include "shell.hpp"

class interface {
  public:
    interface();

  private:
    static bool running;
    static int sigwinch_received;
    const char *prompt = "$ ";

    static void sighandler(int sig);
    static void cb_linehandler(char *raw_line);

    Shell shell;
    Parser parser;
    static interface *s_instance;
};
