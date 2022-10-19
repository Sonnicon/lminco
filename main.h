#ifndef LMINCO_MAIN_H
#define LMINCO_MAIN_H

#define VERSION "1.0"

void showHelp();

#define FLAG_VERBOSE 1 << 0
#define FLAG_STDIN 1 << 1
extern char flags;

#endif //LMINCO_MAIN_H