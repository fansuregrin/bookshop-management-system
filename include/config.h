#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "utils.h"

struct Config {
    std::string host;
    std::string user;
    std::string passwd;
    std::string db;
    unsigned int port;

    Config();
private:
    void read_from_std_input();
    bool read_from_file();
    void save_to_file();
    bool check_host();
    inline bool check_port();
};

#endif