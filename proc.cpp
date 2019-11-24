/* Copyright 2019 Anastasiia Terenteva */

#include "./proc.h"

#include <dirent.h>

#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>


const char kProcDir[] = "/proc/";

static std::vector<std::string> list_dir(const std::string &dirname) {
  DIR *dir = opendir(dirname.c_str());
  if (dir == NULL) {
    throw std::system_error(errno, std::generic_category(),
      "Failed to open /proc/");
  }

  std::vector<std::string> result;
  struct dirent *dir_entry = readdir(dir);
  while (dir_entry != NULL) {
    std::string full_name = std::string(dir_entry->d_name);
    if (dir_entry->d_name[0] != '.') {
      result.push_back(dir_entry->d_name);
    }
    dir_entry = readdir(dir);
  }

  closedir(dir);

  return result;
}

/* Return numerical filenames from /proc/ directory. */
std::vector<std::string> get_pids() {
  std::vector<std::string> filenames = list_dir(kProcDir);

  auto nondigit_filename = [](const std::string &filename) {
    for (auto ch : filename) {
      if (ch > '9' || ch < '0')
        return true;
    }
    return false;
  };
  filenames.erase(
      std::remove_if(filenames.begin(), filenames.end(), nondigit_filename),
      filenames.end());

  return filenames;
}

/* Returns the complete command line for the process from 
 * /proc/[pid]/cmdline. */
std::string get_cmd(const std::string &pid) {
  std::string result;

  std::fstream cmdline;
  std::string filename = kProcDir + pid + "/cmdline";
  cmdline.open(filename);

  /* The command-line arguments appear as a set of strings separated by '\0',
   * with a further null byte after the last string. */
  while (!cmdline.eof()) {
    std::string line;
    std::getline(cmdline, line);
    result += line;
  }

  return result;
}
