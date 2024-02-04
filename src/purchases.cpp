#include "purchases.h"

purchases::purchases() {
    q = "CREATE TABLE IF NOT EXISTS purchases("
            "id INT PRIMARY KEY AUTO_INCREMENT,"
            "book_id INT, FOREIGN KEY (book_id) REFERENCES books(id),"
            "supp_id INT, FOREIGN KEY (supp_id) REFERENCES suppliers(id),"
            "qty INT,"
            "ord_date DATE,"
            "eta DATE,"
            "received CHAR(1) DEFAULT 'F',"
            "inv INT"
        ");";
    if (mysql_query(conn, q) || mysql_commit(conn)) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        exit(-1);
    }
}

void purchases::new_order() {
    std::cout << "Enter the book id: ";
    get_integer(std::cin, book_id, "Invalid input, please re-enter the book id: ");
    if (!check_field("books", "id", book_id)) {
        std::cerr << output_styles::YELLOW << "book id not exists!"
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    std::cout << "Enter the supplier id: ";
    get_integer(std::cin, supp_id, "Invalid input, please re-enter the supplier id: ");
    if (!check_field("suppliers", "id", supp_id)) {
        std::cerr << output_styles::YELLOW << "suppliers id not exists!"
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    std::cout << "Enter the quantity: ";
    get_integer(std::cin, qty, "Invalid input, please re-enter the quantity: ");
    std::cout << "Estimated time of arrival (in days): ";
    get_integer(std::cin, eta, "Invalid input, please re-enter the eta: ");
    stmt.str("");
    stmt << "INSERT INTO purchases (book_id,supp_id,qty,ord_date,eta) VALUES ("
         << book_id << "," << supp_id << "," << qty << ",CURDATE(),DATE_ADD(CURDATE(), INTERVAL "
         << eta << " DAY));";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    std::cout << output_styles::GREEN << "New order added!"
              << output_styles::RESET_ALL << std::endl;
}

void purchases::receive_order() {
    std::cout << "Enter the order id: ";
    get_integer(std::cin, id, "Invalid input, please re-enter order id: ");
    stmt.str("");
    stmt << "UPDATE purchases SET received='T' WHERE id=" << id << ";";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    if (mysql_affected_rows(conn)) {
        std::cout << output_styles::GREEN << "The order was received successfully!"
                  << output_styles::RESET_ALL << std::endl;
    } else {
        std::cout << output_styles::YELLOW << "No such order!"
                  << output_styles::RESET_ALL << std::endl;
    }
}

void purchases::cancel_order() {
    std::cout << "Enter the order id: ";
    get_integer(std::cin, id, "Invalid input, please re-enter order id: ");
    stmt.str("");
    stmt << "UPDATE purchases SET received='C' WHERE id=" << id << ";";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    if (mysql_affected_rows(conn)) {
        std::cout << output_styles::GREEN << "The order was canceled successfully!"
                  << output_styles::RESET_ALL << std::endl;
    } else {
        std::cout << output_styles::YELLOW << "No such order!"
                  << output_styles::RESET_ALL << std::endl;
    }
}

void purchases::display() {
    std::string menu = 
        "+------------------------------------+\n"
        "|         DISPLAY PURCHASES          |\n"
        "+------------------------------------+\n"
        "|   1. DISPLAY ORDERS NOT RECEIVED   |\n"
        "|   2. DISPLAY ORDERS RECEIVED       |\n"
        "|   3. DISPLAY ORDERS CANCELED       |\n"
        "|   4. DISPLAY ALL ORDERS            |\n"
        "+------------------------------------+\n"
        "Enter your choice: ";
    char choice;
    std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
    while (true) {
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == '1') {
            received = 'F';
            break;
        } else if (choice == '2') {
            received = 'T';
            break;
        } else if (choice == '3') {
            received = 'C';
            break;
        } else if (choice == '4') {
            received = '*';
            break;
        } else {
            std::cout << output_styles::ERASE_PREV_LINE << output_styles::YELLOW 
                      << "Invalid input, please re-enter: " << output_styles::RESET_ALL;
        }
    }
    if (received != '*') {
        stmt.str("");
        stmt << "SELECT "
                "p.id AS id, "
                "b.name AS 'book name', "
                "s.name AS 'supplier name', "
                "p.qty, "
                "p.ord_date, "
                "p.eta "
                "FROM purchases p "
                "JOIN books b ON p.book_id = b.id "
                "JOIN suppliers s ON p.supp_id = s.id "
                "WHERE p.received='" << received << "';";
    } else {
        stmt.str("SELECT "
                "p.id AS id, "
                "b.name AS 'book name', "
                "s.name AS 'supplier name', "
                "p.qty, "
                "p.ord_date, "
                "p.eta, "
                "p.received "
                "FROM purchases p "
                "JOIN books b ON p.book_id = b.id "
                "JOIN suppliers s ON p.supp_id = s.id");
    }
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    if (res_set) {
        display_res_set(res_set);
        mysql_free_result(res_set);
        res_set = nullptr;
    }
}

void purchases_menu() {
    purchases pur;
    char choice;
    std::string menu = 
        "+------------------------------------+\n"
        "|           PURCHASES MENU           |\n"
        "+------------------------------------+\n"
        "|     1. NEW ORDER                   |\n"
        "|     2. RECEIVE ORDER               |\n"
        "|     3. CANCEL ORDER                |\n"
        "|     4. DISPLAY                     |\n"
        "|     5. RETURN TO MAIN MENU         |\n"
        "+------------------------------------+\n"
        "Enter your choice: ";
    std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
    while (true) {
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case '1':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    pur.new_order();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '2':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    pur.receive_order();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '3':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    pur.cancel_order();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '4':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    pur.display();
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
