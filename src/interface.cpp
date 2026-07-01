#include "interface.hpp"
#include "pipeline.hpp"
#include "autocompletemanager.hpp"
#include "str.hpp"

#include <memory>
#include <string>
#include <cerrno>
#include <clocale>
#include <csignal>
#include <cstdio>
#include <cstdlib>

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/select.h>

int interface::sigwinch_received{};
void interface::sighandler(int sig) { sigwinch_received = 1; }

interface *interface::s_instance = nullptr;

void interface::start_loop() {
    fd_set fds;
    int r{};

    s_instance = this;

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

void interface::cb_linehandler(char *raw_line) {
    std::unique_ptr<char, decltype(&std::free)> line(raw_line, std::free);

    if (line == nullptr) {
        rl_callback_handler_remove();
        s_instance->running = false;
        return;
    }

    if (*line) {
        add_history(line.get());

        std::string inputLine(line.get());
        if (inputLine == "exit") {
            rl_callback_handler_remove();
            s_instance->running = false;
            return;
        }

        auto tokens = str::tokenize(inputLine);
        PipelinePlan plan = s_instance->parser.parse(tokens);
        s_instance->shell.run(plan);
    }
}
