/* Copyright 2019 Anastasiia Terenteva */

#include <iostream>

#include "./proc.h"

int main() {
  std::cout << "PID" << '\t' << "CMD" << std::endl;
  for (auto pid : get_pids()) {
    std::cout << pid << '\t' << get_cmd(pid) << std::endl;
  }

  return 0;
}
