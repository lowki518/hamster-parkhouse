#include "../include/car_lib.h"
#include "../include/queue_lib.h"
#include "../include/data_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
@brief tests the new_node function
*/
void test_new_node() {
    t_Car car* = car_arrives(100.00f, 50);

    t_Car_Node *node = new_Node(car);

    assert(node->pCar == car);
    assert(node->pNext == NULL);

    free(node);
    free(car);

}

/*
@brief tests the init_queue function
*/
void test_init_queue() {
    t_Queue *queue = init_queue();

    assert(queue->q_length == 0);
    assert(queue->first_pos == NULL);
    assert(queue->last_pos == NULL);

    free(queue);
}

/*
@brief tests the en_queue function
*/

void test_en_queue() {
    t_Queue *queue = init_queue();

    t_Car *car_1 = car_arrives(100.00f, 50);
    t_Car_Node *node_1 = new_Node(car_1);

    t_Car *car_2 = car_arrives(100.00f, 50);
    t_Car_Node *node_2 = new_Node(car_2);

    en_queue(queue, node_1);

    assert(queue->q_length == 1);

    en_queue(queue, node_2);

    assert(queue->q_length == 2);
    assert(queue->first_pos == node_1);
    assert(queue->first_pos->pNext == node_2;)
    assert(queue->last_pos == node_2);

    free(queue);
    free(car_1);
    free(car_2);
    free(node_1);
    free(node_2);

}


/*
@brief tests the de_queue function
*/
void test_de_queue() {
    t_Queue *queue = init_queue();

    t_Car *car_1 = car_arrives(100.00f, 50);
    t_Car_Node *node_1 = new_Node(car_1);

    t_Car *car_2 = car_arrives(100.00f, 50);
    t_Car_Node *node_2 = new_Node(car_2);

    en_queue(queue, node_1);
    en_queue(queue, node_2);

    de_queue(queue);

    assert(queue->q_length == 1);
    assert(queue->first_pos == node_2);
    assert(queue->last_pos == node_2);

    de_queue(queue);

    assert(queue->q_length == 0);
    assert(queue->first_pos == NULL);
    assert(queue->last_pos == NULL);

    free(queue);
    free(car_1);
    free(car_2);
    free(node_1);
    free(node_2);

}

/*
@brief tests the clear_queue function
*/
void test_de_queue() {
    t_Queue *queue = init_queue();

    t_Car *car_1 = car_arrives(100.00f, 50);
    t_Car_Node *node_1 = new_Node(car_1);

    t_Car *car_2 = car_arrives(100.00f, 50);
    t_Car_Node *node_2 = new_Node(car_2);

    en_queue(queue, node_1);
    en_queue(queue, node_2);

    clear_queue(queue);
    
    assert(queue == NULL);

    free(queue);
    free(car_1);
    free(car_2);
    free(node_1);
    free(node_2);

}