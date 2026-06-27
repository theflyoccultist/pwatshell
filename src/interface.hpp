#include "autocompletemanager.hpp"
#include "parser.hpp"
#include "pipeline.hpp"
#include "shell.hpp"
#include "str.hpp"

#include <cerrno>
#include <clocale>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/select.h>

namespace {
const Shell shell;
const Parser parser;
} // namespace

class interface {
  private:
    static bool running;
    static int sigwinch_received;
    const char *prompt = "$ ";

    static void sighandler(int sig) { sigwinch_received = 1; }

    static void cb_linehandler(char *raw_line) {
        std::unique_ptr<char, decltype(&std::free)> line(raw_line, std::free);

        if (line == nullptr) {
            rl_callback_handler_remove();
            running = false;
            return;
        }

        if (*line) {
            add_history(line.get());

            std::string inputLine(line.get());

            auto tokens = str::tokenize(inputLine);
            PipelinePlan plan = parser.parse(tokens);
            shell.executePipeline(plan, running);
        }
    }

  public:
    interface() {
        fd_set fds;
        int r{};

        std::setlocale(LC_ALL, "");

        signal(SIGWINCH, sighandler);

        rl_attempted_completion_function = AutoCompleteManager::shellCompletion;
        rl_callback_handler_install(prompt, cb_linehandler);

        while (running) {
            FD_ZERO(&fds);
            FD_SET(fileno(rl_instream), &fds);

            r = select(FD_SETSIZE, &fds, nullptr, nullptr, nullptr);
            if (r < 0 && errno != EINTR) {
                perror("shell: select");
                rl_callback_handler_remove();
                break;
            }

            if (sigwinch_received) {
                rl_resize_terminal();
                sigwinch_received = 0;
            }

            if (r < 0) {
                continue;
            }

            if (FD_ISSET(fileno(rl_instream), &fds)) {
                rl_callback_read_char();
            }
        }
    }
};
