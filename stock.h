/******************************************************************************
 *  @file stock.h
 *  @description Header file for the Stock Class
 *  @author Lance Cho
 *****************************************************************************
 */

#ifndef STOCK_H
#define STOCK_H
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;


class Stock {
    friend ostream& operator<<(ostream &, const Stock &);
    
public:
    Stock ();
    ~Stock();
    string getTicker();
    void setTicker(string);
    string getType();
    void setType(string);
    double getPrice();
    void setPrice(string);
    
    bool operator<(const Stock& S) const;
private:
    string ticker;
    string type;
    double price;
};

#endif
