#ifndef QUEUE_LIB_H
#define QUEUE_LIB_H

#include "data_types.h"
#include <stdlib.h>

/*
@brief creates a new node for the queue

@param[1] car A pointer to the car to add into the node.

@return the created node
*/
t_Car_Node *new_Node(t_Car *car);


/*
@brief initializes an empty queue.

@return returns the initialized queue.
*/
t_Queue *init_queue();


/*
@brief dequeues the first car from the queue.

@param[1] queue A pointer to the queue to dequeue from.

@return void
*/
void de_queue(t_Queue *queue);


/*
@brief enqueues a car in the queue.

@param[1] queue A pointer to the queue to enqueue in.
@param[2] car The car to enqueue in the queue.

@return void
*/
void en_queue(t_Queue *queue, t_Car *car);


/*
@brief clears the queue.

@param[1] queue A pointer to the queue to delete

@return void
*/
void clear_queue(t_Queue *queue);


#endif