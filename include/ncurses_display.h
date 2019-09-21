#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include "system.h" // System
// Included and needed in system.h:
// process.h which includes <string> and declares Process

#include <curses.h> // WINDOW

namespace NCursesDisplay {
void Display(System& system, int n = 10);
//void Display(int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n, unsigned int);
//void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif
