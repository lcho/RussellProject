/******************************************************************************
 *  @file main.cpp
 *  @description Main or Driver file for the custom linked list
 *  @author Lance Cho
 *****************************************************************************
 */

#include "stock.h"
#include "list.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{
    List<Stock> stock;
    string data;
    ifstream infile("StockList.csv");
    if (!infile) {
        cout << "File could not be opened." << endl;
        return 1;
    } else {
        // build linked list using *filename* and *linkedlist_name*
        stock.buildList(infile, stock);
    }
    
    // print out the linked list
//    cout << "Stocks: " << stock << endl;
    
    stock.printCommonavg();
    stock.printADRavg();
    stock.removeADR();
//    cout << "\n\nStocks After Remove: " << stock << endl;
    
    cout << "Enter a ticker ([!quit] to quit): " << endl;
    cin >> stock;
    
    cout << "Printed the updated list on a csv file" << endl;
    cout << "Have a nice day!" << endl;
    return 0;
}

