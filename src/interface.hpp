#pragma once

#include "parser.hpp"
#include "shell.hpp"

class interface {
  public:
    interface() = default;
    void start_loop();

  private:
    bool running = true;
    const char *prompt = "$ ";

    static int sigwinch_received;
    static void sighandler(int sig);
    static void cb_linehandler(char *raw_line);

    Shell shell;
    Parser parser;
    static interface *s_instance;
};
