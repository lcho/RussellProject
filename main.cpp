/****************************************************************************
 *  @file main.cpp                                                          *
 *  @description Main or Driver file for the custom linked list             *
 *  @author Lance Cho                                                       *
 ****************************************************************************/

#include "stock.h"
#include "list.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{
    List<Stock> stock;
    ifstream infile("StockList.csv");
    ofstream outfile("StockListEdited.csv");
    
    if (!infile) {
        cout << "File could not be opened." << endl;
        return 1;
    } else {
        // Helper method that performs all necessary implementations
        //  in the background
        stock.mainOperationsHelper(infile, outfile);
    }

    return 0;
}

