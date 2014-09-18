/******************************************************************************
 *  @file list.h
 *  @description Templatized Linked List
 *  @author Lance Cho
 *****************************************************************************
 */

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
            output << current->data;
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
        string tick;
        input >> tick;
        list.retrieve(tick);
        
        for(;;){
            if (tick == "!quit")
                break;
            
            cout << "\nEnter a ticker ([!quit] to quit): " << endl;
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
    bool insert(T*);                     //insert data in the list
    void retrieve(const string);
    bool isEmpty() const;                //check if list is empty
    void makeEmpty();                    //deallocate memory
    void printADRavg();
    void printCommonavg();
    void removeADR();
//---------------------Private---------------------------------------------
private:
    struct Node {          // node in the linked list
        T data;            // T (Stock) data inside a node
        Node* next;        // pointer to the next node
    };//end struct
    
    Node* head;            // pointer to the first node
    int size;              // size of the linked list
    
};//end List

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
 *  @param infile file passed into the builder mehtod
 *  @param list linked list to be used after reading the file
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
    while (!infile.eof() )
    {
        // Getting the stock information
        getline (infile, data, ' ' );   // ignore whitespace
        getline (infile, data, ',' );
        stockptr->setTicker(data);
        
        getline (infile, data, ' ' );   // ignore whitespace
        getline (infile, data, ',' );
        stockptr->setType(data);
        
        getline (infile, data, ' ' );   // ignore whitespace or $ sign
        getline (infile, data, '$' );
        getline (infile, data, ' ' );
        stockptr->setPrice(data);
        
        getline (infile, data, '\n' );  // ignore the extra new line
        
        // insert stock into linked list
        this->insert(stockptr);
    }
    
}// end buildList

/**---------------------insert--------------------------------------------
 *  @brief insert
 *  @description insert the stock information into the linked list
 *  @param dataPtr - data to be inserted
 *  @return true if node is inserted correctly
 */
template <typename T>
bool List<T>::insert(T* dataPtr){
    
    Node* ptr = new Node;
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
    Node* current = this->head;
    while (current != NULL){
        if (input == current->data.getTicker() ||
            (input + " ") == current->data.getTicker()){
            cout << "Stock: "<< input << "  Price: $" <<
                current->data.getPrice() << endl;
        }
        current = current->next;
    }

}


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

    if (head == NULL)
        return;
    
    Node* current;
    Node* temp = head;
    
    
    while (temp != NULL)
    {
        current = temp->next;
        delete temp;
        head = head->next;
        temp->next = NULL;

        temp = current;
        size--;
    }//end while
    size = 0;
    head = NULL;        //set the head as empty
}//end makeEmpty


/**-------------------------printCommonavg--------------------------------
 *  @brief makeEmpty
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
            sum = sum + current->data.getPrice();
            counter++;
        }
        current = current->next;
    }
    
    avg = sum/counter;
    cout << "Common Stock Average: " << avg << endl;
}


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
            sum = sum + current->data.getPrice();
            counter++;
        }
        current = current->next;
    }
    
    avg = sum/counter;
    cout << "ADR Average: " << avg << endl;
}


/**-------------------------removeADR-------------------------------------
 *  @brief makeEmpty
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
                temp = current;
                current = current->next;
                previous->next = current;
                delete temp;
            } else {
                current = current->next;
                previous = previous->next;
            }
        }
    }
}


#endif
