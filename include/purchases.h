#ifndef PURCHASES_H
#define PURCHASES_H

#include <iostream>
#include "utils.h"

class purchases {
public:
    purchases();
    void new_order();
    void receive_order();
    void cancel_order();
    void display();
private:
    int id;           // order id, primary key
    int book_id;      // foreign key, ref (books)
    int supp_id;      // foreign key, ref (supplier)
    int qty;          // quantity
    date ord_date;    // ordered date
    int eta;          // estimatd time of arrival (in days)
    char received;    // 'T'(received)/'F'(not received)/'C'(canceled)
    int inv;          // Whether to add to inventory
};

void purchases_menu();

#endif