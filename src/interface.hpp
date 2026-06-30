#pragma once

class interface {
  private:
    static bool running;
    static int sigwinch_received;
    const char *prompt = "$ ";

    static void sighandler(int sig);
    static void cb_linehandler(char *raw_line);

  public:
    interface();
};
