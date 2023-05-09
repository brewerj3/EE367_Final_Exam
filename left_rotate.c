#include <stdlib.h>
#include <stdio.h>

struct node {
    int val;
    struct node *next;
};

struct node *create_node(int val);

void delete_list(struct node *head);

void display_list(struct node *head);

struct node *rotate_left(struct node *head, int k);

struct node *create_list(int n);

int main() {
    printf("\nOriginal list\n");
    struct node *head = create_list(7);
    display_list(head);

    printf("\nRotate left by 3\n");
    head = rotate_left(head, 3);
    display_list(head);

    printf("\nRotate left by 1\n");
    head = rotate_left(head, 1);
    display_list(head);

    printf("\nNew list\n");
    head = create_list(10);
    display_list(head);

    head = rotate_left(head, 74);
    printf("\nRotate left by 74\n");
    display_list(head);

    return 0;
}

struct node *create_node(int val) {
    struct node *p = (struct node *) malloc(sizeof(struct node));
    p->val = val;
    p->next = NULL;
    return p;
}

void delete_list(struct node *head) {
    for (struct node *p = head; p != NULL;) {
        struct node *temp = p;
        p = p->next;
        free(temp);
    }
}

void display_list(struct node *head) {
    printf("List: ");
    for (struct node *p = head; p != NULL; p = p->next) {
        printf("->%d", p->val);
    }
    printf("\n");
}

struct node *rotate_left(struct node *head, int amount) {
    return head;
}


struct node *create_list(int n) {
    struct node *last_node = create_node(0);
    struct node *head = last_node;
    for (int i = 1; i < n; i++) {
        last_node->next = create_node(i);
        last_node = last_node->next;
    }
    return head;
}

