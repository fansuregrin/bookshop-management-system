#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include <string>
#include <iostream>

class suppliers {
private:
    int id;  // primary key
    std::string name;
    std::string phone_number;
    std::string addr;
public:
    suppliers();
    void add();
    void remove();
    void search();
    void display();
};

void suppliers_menu();

#endif