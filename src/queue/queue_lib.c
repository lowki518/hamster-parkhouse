#include "queue_lib.h"    
#include "../data_types.h"
#include <stdlib.h>

/*
@brief Creates a new node for the queue

@param[1] car A pointer to the car to add into the node.

@return the created node
*/
t_Car_Node *new_Node(t_Car *car) {
    t_Car_Node *node = malloc(sizeof(node));
    if(!node) {
        return -1;
    }

    node->pCar = *car;
    node->pNext
    return node;

    return NULL; // Placeholder
}


/*
@brief Initializes an empty queue.

@return returns the initialized queue.
*/
t_Queue *init_queue(t_Car_Node *node) {
    t_Queue *queue = malloc(sizeof(queue));
    if(!queue) {
        return -1;
    }
 
    queue->q_length = g_q; //global variable for queue length

    node->pNext = NULL;
    queue->first_pos = NULL;

    //does it need anything else for intitialization?????

    return queue;

    return NULL; // Placeholder
}


/*
@brief dequeues the first car from the queue.

@param[1] queue A pointer to the queue to dequeue from.

@return void
*/
void de_queue(t_Queue *queue,t_Car_Node *node) {
    node->pNext
    
    
    /*
    RENEW QUEUE HEAD
    DELETE/FREE REMOVED NODE
    */
}


/*
@brief Enqueues a car in the queue.

@param[1] queue A pointer to the queue to enqueue in.
@param[2] car The car to enqueue in the queue.

@return void
*/
void en_queue(t_Queue *queue, t_Car *car) {
    pNewNode->pNext = pNode->pNext;
    pNode->pNext = pNewNode;

    *node
    node->

/*
    GOTO LAST ELEMENT IN QUEUE -> ADD new_node(car)
    */
}


/*
@brief clears the queue.

@param[1] queue A pointer to the queue to delete

@return void
*/
void clear_queue(t_Queue *queue) {
    for (g_q > FALSE, int i) {
        free (node);
        node=next
    }
    

    /*
    DELETE/FREE EVERY ELEMENT IN queue
    DELETE/FREE queue
    */
}
