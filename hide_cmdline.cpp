/* Copyright 2019 Anastasiia Terenteva */

#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <sstream>
#include <cstdint>
#include <iostream>

#include "./proc.h"


static const int kBufSize = 256;

void hide_cmdline(char **argv) {
  int ret = prctl(PR_SET_MM, PR_SET_MM_ARG_START,
      ((uint64_t) argv[0]) + strlen(argv[0]), 0, 0);

  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(),
      "ptrctl failed");
  }
}

int main(int argc, char **argv) {
  hide_cmdline(argv);

  std::stringstream pid;
  pid << getpid();
  std::cout << "/proc/" << pid.str() << "/cmdline: " << get_cmd(pid.str())
    << std::endl;

  std::cout << "argv[1] " << argv[1] << '\n';

  return 0;
}
