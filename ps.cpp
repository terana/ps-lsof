#include <iostream>

#include "proc.h"

int main() {
  std::cout << "PID" << std::endl;
  for (auto pid : get_pids()) {
    std::cout << pid << std::endl;
  }

  return 0;
}
