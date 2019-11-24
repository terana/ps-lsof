/* Copyright 2019 Anastasiia Terenteva */

#include "./proc.h"

#include <dirent.h>
#include <pwd.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>


const char kProcDir[] = "/proc/";
const size_t kBufSize = 1024;

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

/* Get Uid from /proc/[pid]/status.
 * Expecting a string like:
 * Uid: <real>  <effective> <saved set> <filesystem> */
static std::string get_uid(const std::string &pid) {
  std::string uid_line;

  std::fstream proc_stat;
  std::string filename = kProcDir + pid + "/status";
  proc_stat.open(filename);

  while (!proc_stat.eof()) {
    getline(proc_stat, uid_line);
    if (uid_line.find("Uid:") == 0) {
      break;
    }
  }

  if (proc_stat.eof()) {
    std::stringstream err;
    err << "UID is not provided in /proc/" << pid.c_str() << "/status";
    throw std::runtime_error(err.str());
  }

  /* We are interested in the real Uid. */
  size_t first_delim = uid_line.find("\t");
  size_t second_delim = uid_line.find("\t", first_delim + 1);
  size_t substr_len = second_delim - first_delim - 1;

  return uid_line.substr(first_delim + 1, substr_len);
}

std::string get_user(const std::string &pid) {
  std::string uid = get_uid(pid);

  struct passwd pw;
  struct passwd *result;
  char buf[kBufSize];

  getpwuid_r(std::stoi(uid), &pw, buf, sizeof(buf), &result);
  if (result == NULL) {
    throw std::system_error(errno, std::generic_category(),
      "Failed to get username from passwd.");
  }

  return pw.pw_name;
}

std::vector<std::string> get_open_files(const std::string &pid) {
  std::string fd_dir = kProcDir + pid + "/fd/";
  std::vector<std::string> fds = list_dir(fd_dir);
  std::vector<std::string> result;
  char link_contents[kBufSize];

  for (auto fd : fds) {
    int len = readlink((fd_dir + fd).c_str(), link_contents,
        sizeof(link_contents) - 1);
    if (len < 0) {
      /* Skipping this file descriptor. */
      continue;
    }
    link_contents[len] = 0;
    result.push_back(link_contents);
  }
  return result;
}

