#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Задача: https://tproger.ru/problems/list-copy/
 *
 * Есть однонаправленный список из структур. В нём random указывает
 * на какой-то еще элемент этого же списка. Требуется написать функцию,
 * которая копирует этот список с сохранением структуры.
 * O(n), константная дополнительная память + память
 * под элементы нового списка. Нельзя сразу выделить память под все
 * данные одник куском т.е. список должен быть честным, разбросанным
 * по частям, а не единым блоком, как массив.
 */

typedef struct Node {
    int data;
    struct Node *next;
    struct Node *random;
} Node;

Node *create_node() {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = rand() % 100;
    new_node->next = NULL;
    new_node->random = NULL;
}

Node *add_node(Node *head, Node *next) {
    Node *cur = head;
    while (cur != NULL && cur->next != NULL) {
        cur = cur->next;
    }
    if (cur == NULL) {
        return next;
    }
    cur->next = next;
}

/*
 * Генерирует список из n элементов
 */
Node *generate_list(int n) {
    if (n <= 0) {
        return NULL;
    }
    Node *head = create_node();
    Node **ptr_list = (Node **)malloc(n * sizeof(Node*));
    ptr_list[0] = head;
    for (int i = 1; i < n; i++) {
        Node *new_node = create_node();
        ptr_list[i] = new_node;
        add_node(head, new_node);
    }
    for (Node *cur = head; cur != NULL; cur = cur -> next) {
        cur->random = ptr_list[rand() % n];
    }
    return head;
}

Node *copy_list(Node *head) {
    if (head == NULL) {
        return NULL;
    }

    Node *cur = head;
    while (cur != NULL) {
        // Создаем полный дубликат текущего узла
        Node *dup = (Node *)malloc(sizeof(Node));
        dup->data = cur->data;
        dup->next = cur->next;
        dup->random = cur->random;

        // В next текущего узла добавляем указатель
        // на созданный дубликат
        Node *prev = cur;
        cur = cur->next;
        prev->next = dup;
    }

    cur = head;
    while (cur != NULL) {
        // cur->next - дубликат текущего узла,
        // его random сейчас указывает на
        // элемент настоящего списка, а тот
        // в свою очередь в next'е хранит свой дубль
        cur->next->random = cur->random->next;

        // Передвигаемся по настоящему списку
        cur = cur->next->next;
    }

    Node *result = head->next;

    // Сейчас у списка-дубликата next хранит адреса
    // элементов настощего списка
    // А в next'ах настоящего списка лежат свои дубликаты
    // Нужно поменять их местами
    cur = head;
    Node *real_next = cur->next->next;
    while (real_next != NULL) {
        cur->next->next = real_next->next;
        cur->next = real_next;
        cur = real_next;

        real_next = cur->next->next;
    }
    // У последнего элемента по-прежнему в next
    // лежит дубликат
    cur->next = NULL;

    return result;
}

/*
 * p.s. Не гарантирует верный результат,
 * т.к. проверка происходит по значению, а всегда
 * есть вероятность, что значения разных узлов совпадут
 */
int compare_lists(Node *list1, Node *list2) {
    Node *cur1 = list1;
    Node *cur2 = list2;
    if (cur1 == NULL && cur2 == NULL) {
        return 1;
    }
    if (cur1 == NULL || cur2 == NULL) {
        return 0;
    }

    int result = 1;
    while (cur1->next != NULL && cur2->next != NULL
           && cur1->random != NULL && cur2->random != NULL) {
        if (cur1->data != cur2->data
            || cur1->next->data != cur2->next->data
            || cur1->random->data != cur2->random->data) {
            result = 0;
        }
        cur1 = cur1->next;
        cur2 = cur2->next;
    }
    return (cur1->next == NULL && cur2->next == NULL) && result;
}

int main() {
    srand((unsigned int)time(NULL));
    Node *head = generate_list(1000);
    Node *copy = copy_list(head);
    printf(compare_lists(head, copy) == 1 ? "Equal\n" : "Different\n");
    return 0;
}