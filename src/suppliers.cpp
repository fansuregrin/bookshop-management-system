#include "suppliers.h"
#include "utils.h"

suppliers::suppliers() {
    q = "CREATE TABLE IF NOT EXISTS suppliers ("
        "id INT PRIMARY KEY AUTO_INCREMENT,"
        "name VARCHAR(255),"
        "phone_number VARCHAR(255),"
        "addr VARCHAR(255)"
        ");";
    if (mysql_query(conn, q) || mysql_commit(conn)) {
        std::cerr << output_styles::RED << mysql_error(conn) << output_styles::RESET_ALL << std::endl;
        exit(-1);
    }
}

void suppliers::add() {
    std::cout << "Enter the supplier name: ";
    get_varchar(std::cin, name);
    std::cout << "Enter the phone number: ";
    get_varchar(std::cin, phone_number);
    std::cout << "Enter the address: ";
    get_varchar(std::cin, addr);
    stmt.str("");
    stmt << "INSERT INTO suppliers(name,phone_number,addr) VALUES ('"
         << name << "','" << phone_number << "','" << addr << "');";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn) << output_styles::RESET_ALL << std::endl;
    } else {
        std::cout << output_styles::GREEN << "\nSupplier record inserted successfully!"
                  << output_styles::RESET_ALL << std::endl;
    }
}

void suppliers::remove() {
    std::cout << "Enter the id of the supplier: ";
    get_integer(std::cin, id, "Invalid input, please re-enter the id: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    stmt.str("");
    stmt << "DELETE FROM suppliers WHERE id=" << id;
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn) << output_styles::RESET_ALL << std::endl;
        return;
    }
    if (mysql_affected_rows(conn)) {
        std::cout << output_styles::GREEN << "Removed the supplier successfully!" 
                  << output_styles::RESET_ALL << std::endl;
    } else {
        std::cout << output_styles::YELLOW 
                  << "The supplier do not exist, no supplier was removed!" 
                  << output_styles::RESET_ALL << std::endl;
    }
}

void suppliers::search() {
    std::string keyword;
    std::cout << "Enter the keyword: ";
    get_varchar(std::cin, keyword);
    stmt.str("");
    stmt << "SELECT * FROM suppliers WHERE LOWER(name) LIKE LOWER('%" << keyword 
         << "%') OR LOWER(addr) LIKE LOWER('%" << keyword << "%')";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn) << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    if (res_set && mysql_num_rows(res_set)) {
        display_res_set(res_set);
        mysql_free_result(res_set);
        res_set = nullptr;
    } else {
        std::cout << output_styles::YELLOW << "No supplier found!"
                  << output_styles::RESET_ALL << std::endl;
    }
}

void suppliers::display() {
    q = "SELECT * FROM suppliers";
    if (mysql_query(conn, q)) {
        std::cerr << output_styles::RED << mysql_error(conn) << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    if (res_set) {
        display_res_set(res_set);
        mysql_free_result(res_set);
        res_set = nullptr;
    }
}

void suppliers_menu() {
    suppliers supp;
    char choice;
    std::string menu = 
        "+------------------------------------+\n"
        "|          SUPPLIERS MENU            |\n"
        "+------------------------------------+\n"
        "|     1. ADD                         |\n"
        "|     2. REMOVE                      |\n"
        "|     3. SEARCH                      |\n"
        "|     4. DISPLAY ALL                 |\n"
        "|     5. RETURN TO MAIN MENU         |\n"
        "+------------------------------------+\n"
        "Enter your choice: ";
    std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
    while (true) {
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case '1':{
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    supp.add();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;}
                break;
            case '2':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    supp.remove();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '3':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    supp.search();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '4':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    supp.display();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '5':
                return;
            default:
                std::cout << output_styles::ERASE_PREV_LINE << output_styles::YELLOW 
                          << "Invalid input, please re-enter: " << output_styles::RESET_ALL;
        }
    }
}