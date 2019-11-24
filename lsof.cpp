/* Copyright 2019 Anastasiia Terenteva */

#include <iostream>

#include "./proc.h"

int main() {
  std::cout << "COMMAND" << '\t' << "PID" << '\t' << "USER" << '\t' << "FD"
    << std::endl;
  for (auto pid : get_pids()) {
    auto cmd = get_cmd(pid);
    auto user = get_user(pid);
    for (auto file : get_open_files(pid)) {
      std::cout << cmd << '\t' << pid << '\t' << user << '\t' << file
        << std::endl;
    }
  }

  return 0;
}
