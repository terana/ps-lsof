/* Copyright 2019 Anastasiia Terenteva */

#include <iostream>

#include "./proc.h"

int main() {
  std::cout << "PID" << '\t' << "UID" << '\t' << "CMD" << std::endl;
  for (auto pid : get_pids()) {
    std::cout << pid << '\t' << get_uid(pid) << '\t' << get_cmd(pid)
      << std::endl;
  }

  return 0;
}
