/* Copyright 2019 Anastasiia Terenteva */

#pragma once

#include <vector>
#include <string>

std::vector<std::string> get_pids();
std::string get_cmd(const std::string &pid);
std::string get_uid(const std::string &pid);
std::string get_user(const std::string &pid);
std::vector<std::string> get_open_files(const std::string &pid);
