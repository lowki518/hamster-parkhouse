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
    unsigned int id = 0; 

    t_Car *car = car_arrives(100.00f, &id, 50);

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

    assert(queue != NULL);
    assert(queue->q_length == 0);
    assert(queue->p_first_pos == NULL);
    assert(queue->p_last_pos == NULL);

    free(queue);
}

/*
@brief tests the en_queue function
*/
void test_en_queue() {
    unsigned int id = 0;

    t_Queue *queue = init_queue();

    t_Car *car_1 = car_arrives(100.00f, &id, 50);
    t_Car *car_2 = car_arrives(100.00f, &id, 50);

    en_queue(queue, car_1);

    assert(queue->q_length == 1);
    assert(queue->p_first_pos->pCar == car_1);
    assert(queue->p_last_pos->pCar == car_1);

    en_queue(queue, car_2);

    assert(queue->q_length == 2);
    assert(queue->p_first_pos->pCar == car_1);
    assert(queue->p_first_pos->pNext->pCar == car_2);
    assert(queue->p_last_pos->pCar == car_2);

    clear_queue(queue);
}

/*
@brief tests the de_queue function
*/
void test_de_queue() {
    unsigned int id = 0;

    t_Queue *queue = init_queue();

    t_Car *car_1 = car_arrives(100.00f, &id, 50);
    t_Car *car_2 = car_arrives(100.00f, &id, 50);

    en_queue(queue, car_1);
    en_queue(queue, car_2);

    t_Car *c_from_queue_1 = de_queue(queue);

    assert(queue->q_length == 1);
    assert(queue->p_first_pos->pCar == car_2);
    assert(queue->p_last_pos->pCar == car_2);
    assert(c_from_queue_1 == car_1);

    t_Car *c_from_queue_2 = de_queue(queue);

    assert(queue->q_length == 0);
    assert(queue->p_first_pos == NULL);
    assert(queue->p_last_pos == NULL);
    assert(c_from_queue_2 == car_2);

    free(car_1);
    free(car_2);
    free(queue);
}

/*
@brief tests de_queue on an empty queue
*/
void test_de_queue_empty() {
    t_Queue *queue = init_queue();

    t_Car *result = de_queue(queue);
    assert(result == NULL); // empty queue -> NULL

    free(queue);
}

/*
@brief tests the clear_queue function
*/
void test_clear_queue() {
    unsigned int id = 0;

    t_Queue *queue = init_queue();

    t_Car *car_1 = car_arrives(100.00f, &id, 50);
    t_Car *car_2 = car_arrives(100.00f, &id, 50);

    en_queue(queue, car_1);
    en_queue(queue, car_2);

    clear_queue(queue);
}

/*
@brief tests the entire queue library
*/
int main() {
    test_new_node();
    test_init_queue();
    test_en_queue();
    test_de_queue();
    test_de_queue_empty();
    test_clear_queue();

    printf("All queue_lib tests passed!\n");

    return 0;
}

