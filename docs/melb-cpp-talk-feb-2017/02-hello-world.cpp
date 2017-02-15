//!clang++ -std=c++03 -Weverything 02-hello-world.cpp -o z02.out && ./z02.out
//              ^^^^^ note C++ version varies from file to file

/*
    In this talk, we'll be running code from within the VS Code IDE.

    Each file has a "slashbang" compile-and-run shell command on the first line.
    Typically it compiles and runs the current source file.

    When I select Build (Ctrl-Shift-B), the IDE runs this command
    and shows the output.

    > This is achieved with a custom VS Code task in .vscode/tasks.json
    > that runs a Python script .vscode/ExecBangCmd.py
    > An alternative keybinding can be added in the settings, e.g.
    >   { "key": "ctrl+f5","command": "workbench.action.tasks.build" }

    If you're reading this in a different environment, provided that
    you have clang installed, you can cut-and-paste the commands and
    run them on the command line. The code should run fine with other
    C++11 compilers too.
*/

#include <iostream>

int main()
{
    std::cout << "hi there!\n";
}
