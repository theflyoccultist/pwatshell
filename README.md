# Shell Clone in C++

My own shell implementation, written from scratch in **C++**.

This project is being built as part of the **Codecrafters "Build Your Own Shell"** challenge, where the goal was to build a POSIX compliant shell and learn about command parsing, executing programs within a REPL and deal with system calls on Linux.

> [!NOTE]
> This has mostly been done for education purposes only, it is by no means a 1:1 replacement of the POSIX Shell.

## Features

- Shell builtins such as `echo`, `type`, `pwd`, `history`, `jobs`, `cd`, `exit`.
- Support for running executables in `$PATH`.
- Filesystem Navigation with `pwd` and `cd`.
- Support for executables / file names within single and double quotes, and with escape (backslash) characters.
- Support for redirecting / appending a command's output to a file.
- Command completion for executables in `$PATH`.
- Filename completion (had to use the GNU Readline library for that one).
- Support for multi command pipelines.
- History navigation with the up and down arrow keys, listing history using the `history` command.
- History persistence using the `HISTFILE` environment variable.

## Work in progress

- Background Jobs

