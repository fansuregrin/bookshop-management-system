#include "books.h"
#include "suppliers.h"
#include "purchases.h"
#include "config.h"

void config_db() {
    Config cfg;
    conn = mysql_init(0);
    std::cout << "Connecting to the database, please wait..." << std::endl;
    conn = mysql_real_connect(conn, cfg.host.c_str(), cfg.user.c_str(), 
            cfg.passwd.c_str(), cfg.db.c_str(), cfg.port, NULL, 0);
    if (!conn) {
        std::remove("./CONFIG");
        std::cerr << output_styles::RED << "Failed to connect to database. Error: "
                  << mysql_error(conn) << "!" << output_styles::RESET_ALL << std::endl;
        exit(-1);
    }
}

void main_menu() {
    char choice;
    std::string menu = 
        "+------------------------------------+\n"
        "|     BOOKSHOP MANAGEMENT SYSTEM     |\n"
        "+------------------------------------+\n"
        "|     1. BOOKS                       |\n"
        "|     2. SUPPLIERS                   |\n"
        "|     3. PURCHASES                   |\n"
        "|     4. EXIT                        |\n"
        "+------------------------------------+\n"
        "Enter your choice: ";
    std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
    while (true) {
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case '1':
                book_menu();
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '2':
                suppliers_menu();
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '3':
                purchases_menu();
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '4':
                return;
            default:
                std::cout << output_styles::ERASE_PREV_LINE << "Invalid input, please re-enter: ";
        }
    }
}

int main() {
    config_db();
    main_menu();
    mysql_close(conn);
}