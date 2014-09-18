/****************************************************************************
 *  @file stock.cpp                                                         *
 *  @description Implementation file for the Stock Class                    *
 *  @author Lance Cho                                                       *
 ****************************************************************************/

#include "stock.h"

Stock::Stock() {
    ticker = "";
    type = "";
    price = 0.0;
}


Stock::~Stock(){}

string Stock::getTicker() {
    return ticker;
}

void Stock::setTicker(string tick) {
    ticker = tick;
}

string Stock::getType() {
    return type;
}

void Stock::setType(string typ) {
    type = typ;
}

double Stock::getPrice() {
    return price;
}

void Stock::setPrice(string value) {
    if(value != "") {
        price = stod(value);
    } else {
        price = 0.0;
    }
   
}

ostream& operator<<(ostream& output, const Stock& S) {
    output << S.ticker << S.type << S.price << endl;
    return output;
}

bool Stock::operator<(const Stock& S) const {
    if (ticker < S.ticker) return true;
    return false;
}
