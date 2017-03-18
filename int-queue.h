#ifndef INT_QUEUE_H
#define INT_QUEUE_H


#include <iostream>     /* File: int-queue.h */
#include <cstdlib>
using namespace std;

enum color_enum{WHITE, GRAY, BLACK};
struct Node {
	color_enum color;
	Node** pred;
	int distance;
	int shortestPathNum=0, predNum=0;
};

class int_queue // Circular queue
{
  private:
    Node** data; // Use an array to store data
    int num_items;         // Number of items on the queue
    int first;             // Index of the first item; start from 0
    const int BUFFER_SIZE;

  public:
    // CONSTRUCTOR member functions
    int_queue();           // Default constructor
    int_queue(int);
    
    // DESTRUCTOR member function
    ~int_queue();

    // ACCESSOR member functions: const => won't modify data members
    bool empty() const;    // Check if the queue is empty
    bool full() const;
    int size() const;      // Give the number of data currently stored
    Node* front() const;     // Retrieve the value of the front item
    // MUTATOR member functions
    void enqueue(Node*);     // Add a new item to the back of the queue
    void dequeue();        // Remove the front item from the queue
};

#endif
