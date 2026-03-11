#include "../../include/queue_lib.h"    
#include "../../include/data_types.h"
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
    node->pNext = NULL;
    return node;
}


/*
@brief Initializes an empty queue.

@return returns the initialized queue.
*/
t_Queue *init_queue() {
    t_Queue *queue = malloc(sizeof(queue));
    if(!queue) {
        return NULL;
    }
 
    queue->q_length = NULL;

    queue->p_first_pos = NULL;
    queue->p_last_pos = NULL;

    return queue;
}


/*
@brief dequeues the first car from the queue.

@param[1] queue A pointer to the queue to dequeue from.

@return void
*/
void de_queue(t_Queue *queue, t_Car_Node *node) {  //is there a better way where we dont need to include the t_Car_Node *node?if not needs to be changed in .h
    if (queue->p_first_pos == NULL && queue->p_last_pos == NULL) {
        return -1;
    }
    else {
    queue ->p_first_pos = node;

    t_Queue *pTmp = queue->p_first_pos;
    queue->p_first_pos = pTmp->pNext;
    pTmp->pNext = NULL;
    }

    //do we want to change the cars index value here?

    free (pTmp);
    
    //does it make sense?
}


/*
@brief Enqueues a car in the queue.

@param[1] queue A pointer to the queue to enqueue in.
@param[2] car The car to enqueue in the queue.

@return void
*/
void en_queue(t_Queue *queue, t_Car_Node *node) {  //same thing here with t_Car_Node

    pNewNode->pNext = pNode->pNext;
    pNode->pNext = pNewNode;

    node->pNext = NULL;

    if (queue->p_first_pos == NULL && queue->p_last_pos == NULL) {
        queue->p_first_pos = node;
        queue->p_last_pos = node;
    }
    else {
        queue->p_last_pos->pNext = node;
        queue->p_last_pos = node;
    }

}


/*
@brief clears the queue.

@param[1] queue A pointer to the queue to delete

@return void
*/
void clear_queue(t_Queue *queue) {
    while (queue->p_first_pos != NULL) {
        de_queue(queue);
    }

    free (queue);
    *queue = NULL;
}
