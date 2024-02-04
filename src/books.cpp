#include "books.h"

void books::add() {
    std::cout << "Enter the name of the book: ";
    get_varchar(std::cin, name);
    std::cout << "Enter the author of the book: ";
    get_varchar(std::cin, author);
    std::cout << "Enter the price: ";
    get_integer(std::cin, price, "Invalid input, please re-enter the price: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter the quantity received: ";
    get_integer(std::cin, qty, "Invalid input, please re-enter the qty: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    stmt.str("");
    stmt << "INSERT INTO books(name,author,price,qty) VALUES ('"
         << name << "','" << author << "'," << price << "," << qty << ");";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn) 
                  << output_styles::RESET_ALL << std::endl;
    } else {
        std::cout << output_styles::GREEN << "Book record inserted successfully!"
                  << output_styles::RESET_ALL << std::endl;
    }
}

void books::update_price() {
    char choice;
    std::cout << "Enter the id of the book: ";
    get_integer(std::cin, id, "Invalid input, please re-enter the id: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    stmt.str("");
    stmt << "SELECT name,author,price FROM books WHERE id=" << id << ";";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    if (res_set && mysql_num_rows(res_set)) {
        display_res_set(res_set);
        std::cout << "Do you want to update the price [y/n]: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Enter the new price: ";
            get_integer(std::cin, price, "Invalid input, please re-enter the price: ");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            stmt.str("");
            stmt << "UPDATE books SET price=" << price << " WHERE id=" << id << ";";
            if (mysql_query(conn, stmt.str().c_str())) {
                std::cerr << output_styles::RED << mysql_error(conn) 
                          << output_styles::RESET_ALL << std::endl;
            } else {
                std::cout << output_styles::GREEN << "Book price updated successfully!" 
                          << output_styles::RESET_ALL << std::endl;
            }
        }
    } else {
        std::cout << output_styles::YELLOW << "No book found!" 
                  << output_styles::RESET_ALL << std::endl;
    }
    mysql_free_result(res_set);
    res_set = nullptr;
}

void books::update_author() {
    char choice;
    std::cout << "Enter the id of the book: ";
    get_integer(std::cin, id, "Invalid input, please re-enter the id: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    stmt.str("");
    stmt << "SELECT name,author,price FROM books WHERE id=" << id << ";";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn) 
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    if (res_set && mysql_num_rows(res_set)) {
        display_res_set(res_set);
        std::cout << "Do you want to update the author [y/n]: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Enter the new author: ";
            get_varchar(std::cin, author);
            stmt.str("");
            stmt << "UPDATE books SET author='" << author << "' WHERE id=" << id << ";";
            if (mysql_query(conn, stmt.str().c_str())) {
                std::cerr << output_styles::RED << mysql_error(conn)
                          << output_styles::RESET_ALL << std::endl;
            } else {
                std::cout << output_styles::GREEN << "Book author updated successfully!" 
                          << output_styles::RESET_ALL << std::endl;
            }
        }
    } else {
        std::cout << output_styles::YELLOW << "No book found!"
                  << output_styles::RESET_ALL << std::endl;
    }
    mysql_free_result(res_set);
    res_set = nullptr;
}

void books::search() {
    std::string keyword;
    std::cout << "Enter the keyword: ";
    get_varchar(std::cin, keyword);
    stmt.str("");
    stmt << "SELECT * FROM books WHERE LOWER(name) LIKE LOWER('%" << keyword 
         << "%') OR LOWER(author) LIKE LOWER('%" << keyword << "%')";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    if (res_set && mysql_num_rows(res_set)) {
        display_res_set(res_set);
    } else {
        std::cout << output_styles::YELLOW << "No book found!"
                  << output_styles::RESET_ALL << std::endl;
    }
    mysql_free_result(res_set);
    res_set = nullptr;
}

void books::update() {
    std::vector<int> book_ids, qtys;
    stmt.str("");
    stmt << "SELECT book_id,qty FROM purchases WHERE received='T' AND inv IS NULL;";
    if (mysql_query(conn, stmt.str().c_str())) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        return;
    }
    res_set = mysql_store_result(conn);
    while (row = mysql_fetch_row(res_set)) {
        book_ids.push_back(std::stoi(row[0]));
        qtys.push_back(std::stoi(row[1]));
    }
    mysql_free_result(res_set);
    res_set = nullptr;
    stmt.str("");
    stmt << "UPDATE purchases SET inv=1 WHERE received='T' AND inv IS NULL;";
    mysql_query(conn, stmt.str().c_str());
    for (size_t i=0; i<book_ids.size(); ++i) {
        stmt.str("");
        stmt << "UPDATE books SET qty=qty+" << qtys[i] << " WHERE id=" << book_ids[i] << ";";
        mysql_query(conn, stmt.str().c_str());
    }
    std::cout << output_styles::GREEN << "The orders received have been updated!"
              << output_styles::RESET_ALL << std::endl;
}

void books::display() {
    mysql_query(conn, "SELECT * FROM books;");
    res_set = mysql_store_result(conn);
    if (res_set) {
        display_res_set(res_set);
        mysql_free_result(res_set);
        res_set = nullptr;
    }
}

books::books() {
    q = "CREATE TABLE IF NOT EXISTS books ("
            "id INT PRIMARY KEY AUTO_INCREMENT,"
            "name VARCHAR(255),"
            "author VARCHAR(255),"
            "price INT,"
            "qty INT"
        ");";
    if (mysql_query(conn, q) || mysql_commit(conn)) {
        std::cerr << output_styles::RED << mysql_error(conn)
                  << output_styles::RESET_ALL << std::endl;
        exit(-1);
    }
}

void book_menu() {
    books bks;
    char choice;
    std::string menu = 
        "+------------------------------------+\n"
        "|              BOOK MENU             |\n"
        "+------------------------------------+\n"
        "|     1. ADD                         |\n"
        "|     2. UPDATE PRICE                |\n"
        "|     3. UPDATE AUTHOR               |\n"
        "|     4. SEARCH                      |\n"
        "|     5. UPDATE STATUS               |\n"
        "|     6. DISPLAY ALL                 |\n"
        "|     7. RETURN TO MAIN MENU         |\n"
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
                    bks.add();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;}
                break;
            case '2':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    bks.update_price();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '3':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    bks.update_author();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '4':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    bks.search();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '5':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    bks.update();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '6':
                do {
                    std::cout << output_styles::ERASE_ENTIRE_SCREEN;
                    bks.display();
                    std::cout << "Want to continue? [y/n]: ";
                    std::cin >> choice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (choice == 'Y' || choice == 'y');
                std::cout << output_styles::ERASE_ENTIRE_SCREEN << menu << std::flush;
                break;
            case '7':
                return;
            default:
                std::cout << output_styles::ERASE_PREV_LINE << output_styles::YELLOW 
                          << "Invalid input, please re-enter: " << output_styles::RESET_ALL;
        }
    }
}