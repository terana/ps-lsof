/* Copyright 2019 Anastasiia Terenteva */

#include <iostream>

#include "./proc.h"

int main() {
  std::cout << "COMMAND" << '\t' << "PID" << std::endl;
  for (auto pid : get_pids()) {
    std::cout << get_cmd(pid) << '\t' << pid << std::endl;
  }

  return 0;
}
