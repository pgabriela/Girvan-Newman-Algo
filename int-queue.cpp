#include "int-queue.h"  /* File: int-queue1.cpp */

using namespace std;

          /***** Default CONSTRUCTOR member function *****/
// Create an empty queue
int_queue::int_queue():BUFFER_SIZE(20) { first = 0; num_items = 0; data = NULL;}
int_queue::int_queue(int total):BUFFER_SIZE(total) {
	first=0;
	num_items = 0;
	data = new Node*[total];
}

		  /***** Destructor member function ****/
int_queue::~int_queue(){
	delete [] data;
	data = NULL;
}

          /***** ACCESSOR member functions *****/
// Check if the int_queue is empty
bool int_queue::empty() const { return (num_items == 0); }

// Give the number of data currently stored
int int_queue::size() const { return num_items; }

bool int_queue::full() const { return (num_items==BUFFER_SIZE) ; }
    
// Retrieve the value of the front item
Node* int_queue::front() const
{
    if (!empty())
        return data[first];

    cerr << "Warning: Queue is empty; can't retrieve any data!" << endl;
    exit(-1);
}

void int_queue::enqueue(Node* x)  // Add a new item to the back of the queue
{
    if (!full())
    {
        data[(first+num_items) % BUFFER_SIZE] = x;
        ++num_items;
    } else {
        cerr << "Error: Queue is full; can't add (" << x << ")!" << endl;
        exit(-1);
    }
}

void int_queue::dequeue()       // Remove the front item from the queue
{
    if (!empty())
    {
        first = (first+1) % BUFFER_SIZE;
        --num_items;
    } else {
        cerr << "Error: Queue is empty; can't remove any data!" << endl;
        exit(-1);
    }
}
