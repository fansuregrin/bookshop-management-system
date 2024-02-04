#ifndef BOOKS_H
#define BOOKS_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include "utils.h"

class books {
public:
    books();
    void add();
    void update_price();
    void update_author();
    void search();
    void update();
    void display();
private:
    int id;  // primary key
    std::string name;
    std::string author;
    int price; // amount in cents
    int qty;  // quantity
};

void book_menu();

#endif