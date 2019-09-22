#include "ncurses_display.h"
#include "format.h" // ElapsedTime

#include <chrono> // milliseconds
#include <thread> // sleep_for

// Included and needed in ncurses_display.h:
// system.h which includes:
//   process.h which includes <string> // to_string
//   <vector>
// <curses.h> // https://pubs.opengroup.org/onlinepubs/7908799/xcurses/curses.h.html

using std::string;
using std::to_string;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
string NCursesDisplay::ProgressBar(float percent) {
  string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Cpu().Utilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(vector<Process>& processes,
                                      WINDOW* window, int n, unsigned int pp) {
//                                    WINDOW* window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{19};
  int const ram_column{27};
  int const time_column{35};
  int const command_column{46};
  static unsigned int ppp=0;
  // bool eraser = false;
  if(pp>ppp) {
    ppp = pp;
    // eraser = true;
  }
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  float cpu;
  for (int i = 0; i < n; ++i) {
    // if(eraser) {
    //   // mvwprintw(window, row, user_column, string("       ").c_str());
    //   wmove(window, row, 0);
    //   // wprintw(window, "========");
    //   wclrtoeol(window);
    //   wrefresh(window);
    // }
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, string(to_string(103 - pp)).c_str());
    // mvwprintw(window, row, user_column, string(to_string(pp)+" "+to_string(ppp)+" "+ppps).c_str());
    // if(pp<2)
    // mvwprintw(window, row, user_column, processes[i].User().c_str());
    // else
    // mvwprintw(window, row, user_column, string(":)").c_str());
    // else if(pp<4)
    // mvwprintw(window, row, user_column, string("       ").c_str());
    // else if(pp<6)
    // mvwprintw(window, row, user_column, string(":)").c_str());
    // This proves that the issue is to do with ncurses.
    cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 5).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
  // wrefresh(window);
}

void NCursesDisplay::Display(System& system, int n) {
//void NCursesDisplay::Display(int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(9, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {
    werase(process_window);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
//  System system;
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n, system.pp());
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    // leaveok(process_window, false);
    // redrawwin(process_window);
    // wredrawln(process_window, 0, n);
    sleep_for(milliseconds(1));
//  system.~System(); This causes double free runtime error that dumps core.
//  Placing system hear did not affect process refresh rate, nor did it
//  remove the appearance the "rootant".
  }
  endwin();
}
