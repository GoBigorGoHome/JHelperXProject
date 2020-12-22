//
// Created by zjsdu on 12/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_COLOR_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_COLOR_H_
// foreground colors
#define CSI "\033["
#define RESET CSI "0m"
#define BLACK CSI "30m"
#define RED CSI "31m"
#define GREEN CSI "32m"
#define YELLOW CSI "33m"
#define BLUE CSI "34m"
#define MAGENTA CSI "35m"
#define CYAN CSI "36m"
#define WHITE CSI "37m"
#define BRIGHT_BLACK CSI "90m"
#define BRIGHT_RED CSI "91m"
#define BRIGHT_GREEN CSI "92m"
#define BRIGHT_YELLOW CSI "93m"
#define BRIGHT_BLUE CSI "94m"
#define BRIGHT_MAGENTA CSI "95m"
#define BRIGHT_CYAN CSI "96m"
#define BRIGHT_WHITE CSI "97m"
void display_color();
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_COLOR_H_
