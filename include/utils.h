#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <iostream>
#include <string>
#include <limits>
#include <functional>
#include <iomanip>
#include <mysql/mysql.h>
#include <termios.h>
#include <unistd.h>

extern MYSQL * conn;
extern MYSQL_RES * res_set;
extern MYSQL_ROW row;
extern std::stringstream stmt;
extern const char * q;
extern std::string query;

struct date {
    int day;
    int month;
    int year;
};

struct output_styles {
    static const std::string ERASE_ENTIRE_SCREEN;
    static const std::string ERASE_PREV_LINE;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string RED;
    static const std::string RESET_ALL;
};

enum InputKey {
    BACKSPACE = 8,
    SPACE = 32,
};

void print_row_border(MYSQL_FIELD *, size_t);
void print_tb_header(MYSQL_FIELD *, size_t);
void print_row(MYSQL_FIELD *, size_t, MYSQL_ROW);
void display_res_set(MYSQL_RES *);

std::istream &get_varchar(std::istream &, std::string &);
std::istream &get_integer(std::istream &, int &,
    const std::string & = "Invalid input, please re-enter: ");

template <typename T>
bool check_field(const std::string &, const std::string &, const T &);

std::string take_passwd();

#endif