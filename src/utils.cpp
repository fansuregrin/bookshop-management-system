#include "utils.h"

MYSQL * conn = nullptr;
MYSQL_RES * res_set = nullptr;
MYSQL_ROW row = nullptr;
std::stringstream stmt;
const char * q = nullptr;
std::string query;

const std::string output_styles::ERASE_ENTIRE_SCREEN = "\033[H\033[2J";
const std::string output_styles::ERASE_PREV_LINE = "\033[F\033[2K";
const std::string output_styles::GREEN = "\033[32m";
const std::string output_styles::YELLOW = "\033[33m";
const std::string output_styles::RED = "\033[31m";
const std::string output_styles::RESET_ALL = "\033[0m";

void print_row_border(MYSQL_FIELD * fields, size_t num_fields) {
    std::string sep = "+-";
    for (size_t i=0; i<num_fields; ++i) {
        std::cout << sep 
                  << std::string(
                        std::max(
                            fields[i].max_length, 
                            static_cast<unsigned long>(fields[i].name_length)
                        ), '-');
        sep = "-+-";
    }
    std::cout << "-+\n";
}

void print_tb_header(MYSQL_FIELD * fields, size_t num_fields) {
    std::string sep = "| ";
    for (size_t i=0; i<num_fields; ++i) {
        std::cout << sep << std::setw(std::max(fields[i].max_length, 
                    static_cast<unsigned long>(fields[i].name_length)))
                  << std::left << fields[i].name;
        sep = " | ";
    }
    std::cout << " |\n";
}

void print_row(MYSQL_FIELD * fields, size_t num_fields, MYSQL_ROW row) {
    std::string sep = "| ";
    for (size_t i=0; i<num_fields; ++i) {
        std::cout << sep << std::setw(std::max(fields[i].max_length,
                    static_cast<unsigned long>(fields[i].name_length))) 
                    << std::left << (row[i]?row[i]:" ");
        sep = " | ";
    }
    std::wcout << " |\n";
}

void display_res_set(MYSQL_RES * res_set) {
    auto num_fields = mysql_num_fields(res_set);
    auto fields = mysql_fetch_fields(res_set);
    print_row_border(fields, num_fields);
    print_tb_header(fields, num_fields);
    print_row_border(fields, num_fields);
    while (row = mysql_fetch_row(res_set)) {
        print_row(fields, num_fields, row);
    }
    print_row_border(fields, num_fields);
}

std::istream &get_varchar(std::istream &is, std::string &str) {
    str.clear();
    std::string::value_type ch; 
    while ((ch = std::cin.get()) != '\n' && ch != EOF) {
        if (ch == '\'') str.push_back('\\');
        str.push_back(ch);
    }
    return is;
}

std::istream &get_integer(std::istream &is, int &val,
    const std::string &msg) {
    while (!(std::cin >> val)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << output_styles::YELLOW << msg << output_styles::RESET_ALL;
    }
    return is;
}

template <typename T>
bool check_field(const std::string &tb_name, const std::string &field_name, const T &field_val) {
    stmt.str("");
    stmt << "SELECT " << field_name << " FROM " << tb_name << " WHERE " << field_name
         << "=" << field_val << ";";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        exit(-1);
    }
    res_set = mysql_store_result(conn);
    if (res_set && mysql_num_rows(res_set)) {
        return true;
    } else {
        return false;
    }
}

template bool check_field<int>(const std::string &, const std::string &, const int&);

std::string take_passwd() {
    termios old_t;
    tcgetattr(STDIN_FILENO, &old_t);
    termios new_t = old_t;
    new_t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
    std::string passwd;
    std::cout << "Enter Password:";
    while (true) {
        auto ch = getchar();
        if (ch < InputKey::SPACE && ch != InputKey::BACKSPACE) break;
        if (ch == InputKey::BACKSPACE) {
            if (passwd.size()) passwd.pop_back();
            continue;
        }
        passwd.push_back(ch);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_t);
    std::cout << std::endl;
    return passwd;
}