#define _POSIX_SOURCE

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>

using std::cerr;
using std::cout;
using std::endl;

void enforce_background();
void sig_handler(int);

int main() {
  cout.setf(std::ios_base::unitbuf); // turn off buffering for cout
  enforce_background();              // make sure we're in a bg process group
  struct sigaction sa;               // sigaction struct object
  sa.sa_handler = sig_handler;       // set disposition
  if (sigaction(SIGCONT, &sa, nullptr) == -1) perror("sigaction");
  if (sigaction(SIGTSTP, &sa, nullptr) == -1) perror("sigaction");
  if (sigaction( SIGINT, &sa, nullptr) == -1) perror("sigaction");
  while (true) pause();              // pause until signal is received
  return EXIT_SUCCESS; 
} // main

void enforce_background() {
  int pgid;
  if ((pgid = tcgetpgrp(STDOUT_FILENO)) == getpgrp()) {
    cerr << "need to be in background" << endl;
    exit(EXIT_FAILURE);
  } else if (pgid == -1) {
    perror("tcgetpgrp");
    exit(EXIT_FAILURE);
  } // if
} // enforce_background

void sig_handler(int signo) {
  cout << "Caught signal " 
       << signo << " (" << strsignal(signo) << ")"
       << endl;
} // sig_handler

