/****************************************************************************
 *  @file list.h                                                            *
 *  @description Templatized Linked List                                    *
 *  @author Lance Cho                                                       *
 ****************************************************************************/

#ifndef LIST_H
#define LIST_H

#include "stock.h"
#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
class List {
/**---------------------operator <<---------------------------------------
 *  @brief << operator
 *  @description responsible for printing the data in the linked list
 *  @param output - output stream
 *  @param printlist - data in this linked list will be printed
 */
    friend ostream& operator<<(ostream& output, const List<T>& printlist) {
        Node* current = printlist.head;
        while (current != NULL){
            if(current->data.getTicker() != ""){
                output << current->data;
            }
            current = current->next;
        }//end while
        current = NULL;
        return output;
    }// end operator<<
/**---------------------operator >>---------------------------------------
 *  @brief >> operator
 *  @description responsible for getting the user inputs
 *  @param input - input stream
 *  @param list - search userinput from this linked list
 */
    friend istream& operator>>(istream& input, List<T>& list) {
        string tick;    // temp container for the data gathered
        input >> tick;
        list.retrieve(tick);
        
        // Won't stop asking for user input until user types in [!quit]
        for(;;){
            if (tick == "!quit")
                break;
            
            cout << "\nPlease use capital letters only." << endl;
            cout << "Enter a ticker ([!quit] to quit): " << endl;
            input >> tick;
            list.retrieve(tick);
        }
        return input;
    }// end operator>>
    
    
//---------------------Public---------------------------------------------
public:
    List();                              //default constructor
    ~List();                             //destructor
    void buildList(ifstream&, List<T>&); //build list from data file
    void buildNewList (ofstream&, List<T>&); //build edited csv file
    bool insert(T*);                     //insert data in the list
    void retrieve(const string);
    bool isEmpty() const;                //check if list is empty
    void makeEmpty();                    //deallocate memory
    void mainOperationsHelper(ifstream&, ofstream&); //helper method
    void doOperationsHelper();           //perform prints and removes
    void printADRavg();                  //prints the avg of all ADR stocks
    void printCommonavg();               //prints the avg of all Common stocks
    void removeADR();                    //removes all the ADR stocks
//---------------------Private---------------------------------------------
private:
    struct Node {          // node in the linked list
        T data;            // T (Stock) data inside a node
        Node* next;        // pointer to the next node
    };//end struct
    
    Node* head;            // pointer to the first node
    int size;              // size of the linked list
};//end List Class



/****************************************************************************
 *                          Implementations                                 *
 ****************************************************************************/

/**-------------------------Constructor-----------------------------------
 *  @brief default constructor
 */
template <typename T>
List<T>::List(){
    head = NULL;
    size = 0;
}//end List

/**--------------------------Destructor-----------------------------------
 *  @brief destructor
 */
template <typename T>
List<T>::~List(){
    makeEmpty();
}//end ~List

/**---------------------------buildList-----------------------------------
 *  @brief build list from data file
 *  @param infile - data in this file will be stored in the linked list
 *  @param list - linked list to be used after reading the file
 */
template <typename T>
void List<T>::buildList(ifstream& infile, List<T>& list){
    string data;
    T* stockptr = new Stock();
    
    // ignore the headers
    getline (infile, data, ',' );
    getline (infile, data, ',' );
    getline (infile, data, ' ' );
    getline (infile, data, ' ' );
    getline (infile, data, '\n' );
    
    while (!infile.eof()) {     // still have data to read
        // Getting the stock information
        getline (infile, data, ' ' );   // ignore whitespace
        getline (infile, data, ',' );
        stockptr->setTicker(data);
        
        getline (infile, data, ' ' );   // ignore whitespace
        getline (infile, data, ',' );
        stockptr->setType(data);
        
        getline (infile, data, ' ' );   // ignore whitespace and/or $ sign
        getline (infile, data, '$' );
        getline (infile, data, ' ' );
        stockptr->setPrice(data);
        
        getline (infile, data, '\n' );  // ignore the extra new line
        
        // insert stock into linked list
        this->insert(stockptr);
    }
    infile.close();
}// end buildList

/**---------------------------buildNewList--------------------------------
 *  @brief build new stock list by creating a new csv file
 *  @param outfile - file to be populated with data
 *  @param list - data of this linked list will be used to populate the file
 */
template <typename T>
void List<T>::buildNewList(ofstream& outfile, List<T>& list){
    // print out the headers
    outfile << " Ticker ,SecurityType , Price\n";
    
    Node* current = head;
    while(current != NULL) {
        if(current->data.getTicker() != "") {
            // output necessary data into the newly created csv file
            outfile << ' ';
            outfile << current->data.getTicker();
            outfile << ',';
            outfile << ' ';
            outfile << current->data.getType();
            outfile << ',';
            outfile << ' ';
            outfile << '$';
            outfile << current->data.getPrice();
            outfile << '\n';
        }
        current = current->next;    // move pointer to next node
    }
    outfile.close();    // close the file
}// end buildNewList

/**---------------------insert--------------------------------------------
 *  @brief insert
 *  @description insert the stock information into the linked list
 *  @param dataPtr - data to be inserted
 *  @return true if node is inserted correctly
 */
template <typename T>
bool List<T>::insert(T* dataPtr){
    
    Node* ptr = new Node;    //create a new placeholder (node)
    if (ptr == NULL){
        return false;
    }
    ptr->data = *dataPtr;    //save the data to the node (dereference ptr)
    
    // if list is empty or if node is the actual head (when sorted)
    if (isEmpty() || ptr->data < head->data){
        ptr->next = head;
        head = ptr;
    }
    
    // check where it should be located (sorted linked list)
    else
    {
        Node* current = head->next;     //to walk through the list
        Node* previous = head;
        
        //walk until end of the list or found position to insert
        while (current != NULL && current->data < ptr->data) {
            previous = current;         // walk to the next node
            current = current->next;
        }//end while
        
        //insert new node to correct position, reallign linked list
        ptr->next = current;
        previous->next = ptr;
        current = NULL;
        previous = NULL;
    }//end if
    
    size++;     // increase size of linked list
    return true;
}//end insert

/**----------------------------retrieve-----------------------------------
 *  @brief retrieve
 *  @description retrieve the price of the ticker provided
 *  @param input - the stock ticker information
 */
template <typename T>
void List<T>::retrieve(const string input) {
    Node* current = this->head; // point to the head of the linked list
    while (current != NULL){
        if (input == current->data.getTicker() ||
            (input + " ") == current->data.getTicker()){
            // print out the stock ticker info and price
            cout << "Stock: "<< input << "  Price: $" <<
                current->data.getPrice() << endl;
        }
        current = current->next;    // move pointer to the next node
    }
}//end retrieve

/**-----------------------isEmpty------------------------------------------
 *  @brief isEmpty
 *  @description check if linked list is empty
 *  @return true if linked list is empty
 */
template <typename T>
bool List<T>::isEmpty() const
{
    return head == NULL;
}//end isEmpty


/**-------------------------makeEmpty-------------------------------------
 *  @brief makeEmpty
 *  @description empty out list, deallocate memory
 */
template <typename T>
void List<T>::makeEmpty(){
    if (head == NULL)   //linked list is empty
        return;
    
    Node* current;
    Node* temp = head;
    
    while (temp != NULL) {
        current = temp->next;
        delete temp;    //deallocate memory
        head = head->next;
        temp->next = NULL;

        temp = current;
        size--;         //decrease linked list size
    }//end while
    size = 0;
    head = NULL;        //set the head as empty
}//end makeEmpty

/**-------------------------mainOperationsHelper---------------------------
 *  @brief mainOperationsHelper
 *  @description Helper method to hide all the main 
 *                  operations done from the user
 */
template <typename T>
void List<T>::mainOperationsHelper(ifstream& input, ofstream& output){
        // build linked list using *filename* and *linkedlist_name*
        this->buildList(input, *this);

        // perform averages of Common Stock and ADR, and removal of ADR stocks
        this->doOperationsHelper();

        // build the new csv file using *filename* and *linkedlist_name*
        this->buildNewList(output, *this);

        // Friendly greeting to the users
        cout << "\nCreated an updated list in csv format." << endl;
        cout << "Have a nice day!" << endl;
}//end mainOperationsHelper

/**-------------------------doOperationsHelper----------------------------
 *  @brief doOperationsHelper
 *  @description Helper method to hide all the operations from the user
 */
template <typename T>
void List<T>::doOperationsHelper(){
    this->printCommonavg();     //print the avg of Common Stock stocks
    this->printADRavg();        //print the avg of ADR stocks
    this->removeADR();          //remove all the ADR stocks from the list
    
    //(fencepost) ask for user input
    cout << "\nPlease use capital letters only." << endl;
    cout << "Enter a ticker ([!quit] to quit): " << endl;
    cin >> *this;
}//end doOperationsHelper

/**-------------------------printCommonavg--------------------------------
 *  @brief printCommonavg
 *  @description prints out the average price of all type Common Stock stocks
 */
template <typename T>
void List<T>::printCommonavg(){
    int counter = 0;
    double sum = 0, avg = 0;
    Node* current = head;
    while (current != NULL){
        if(current->data.getType() == "Common Stock" ||
           current->data.getType() == "Common Stock "){
            //found the correct Common Stock ticker, add the price
            sum = sum + current->data.getPrice();
            counter++;
        }
        current = current->next;    //check next node
    }
    avg = sum/counter;              //compute for the average value
    cout << "Common Stock Average: " << avg << endl;
}//end printCommonavg

/**-------------------------printADRavg-----------------------------------
 *  @brief printADRavg
 *  @description prints out the average price of all type ADR stocks
 */
template <typename T>
void List<T>::printADRavg(){
    int counter = 0;
    double sum = 0, avg = 0;
    Node* current = head;
    while (current != NULL){
        if(current->data.getType() == "ADR" ||
           current->data.getType() == "ADR "){
            //found the correct stock type, add the price to the sum
            sum = sum + current->data.getPrice();
            counter++;
        }
        current = current->next;    //move to the next node
    }
    avg = sum/counter;              //compute for the avg
    //print the average value
    cout << "ADR Average: " << avg << endl;
}//end printADRavg

/**-------------------------removeADR-------------------------------------
 *  @brief removeADR
 *  @description removes all type ADR stocks in the linked list
 */
template <typename T>
void List<T>::removeADR(){
    Node* current = head->next;
    Node* previous = head;
    Node* temp;
    
    // first node exists and first node must be removed
    if(head != NULL && (head->data.getType() == "ADR" ||
       head->data.getType() == "ADR ") ){
        temp = head;
        head = head->next;
        delete temp;
    } else {
        while (current != NULL){
            if(current->data.getType() == "ADR" ||
               current->data.getType() == "ADR "){
                // found an ADR type stock, remove it from the list
                temp = current;
                current = current->next;
                previous->next = current;
                delete temp;    //delete the stock from the list
                size--;         //decrease linked list size after removal
            } else {
                //move to the next node
                current = current->next;
                previous = previous->next;
            }//end inner if/else statement
        }//end while loop
    }//end outer if/else statement
}//end removeADR


#endif
