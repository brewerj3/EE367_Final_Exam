#include <stdlib.h>
#include <stdio.h>

#define INFINITY 10000
#define NO_PRED -1

#define TEST
//#define DEBUG

struct adjlist_node { // Node in the adjacency list
    int node_id;
    int weight;  // Weight of edge that goes to the node
    struct adjlist_node *next;
};

struct graph { // Nodes of the graph are 0, 1, ..., num_nodes-1
    int num_nodes;
    struct adjlist_node **adjlist; // Adjacency list
};

struct algm_node {
    int dist;  // Estimated distance from the source node
    int pred;  // Parent node in the shortest path tree
};

struct algm_data {
    int num_nodes;
    struct algm_node *node;
};

struct priorityq {
    int length;
    int *node;
    int *in_q;
    int capacity;
};

struct Heap {
    int *arr;
    int size;
    int capacity;
};

/* Dijkstra's algorithm and managing its data structures */
struct algm_data *algm_data_create(struct graph *g, int source);

void algm_data_destroy(struct algm_data *a);

void algm_data_display(struct algm_data *a);

struct algm_data *dijkstra(struct graph *g, int source);

/* Priority queue for Dijkstra's aglrothm */
struct priorityq *priorityq_create(struct algm_data *a);

void priorityq_destroy(struct priorityq *pq);

int priorityq_not_empty(struct priorityq *pq);

int priorityq_get(struct priorityq *pq, struct algm_data *a);

int priorityq_in_q(struct priorityq *pq, int node);

void priorityq_update(struct priorityq *pq, struct algm_data *a, int node);

/* Managing graphs */
struct graph *graph_create(int num_nodes);

void graph_destroy(struct graph *g);

void graph_insert_link(struct graph *g, int head_node, int tail_node, int weight);

void graph_display(struct graph *g);

struct graph *create_graph0();

struct graph *create_graph1();

void heapify(struct Heap *minheap, int index, struct algm_data *a);

struct Heap *heap_create(struct algm_data *a);

void heapUpdater(struct Heap *h, int index, struct algm_data *a);

int heap_get(struct Heap *pq, struct algm_data *a);

void heap_destroy(struct Heap *h);

int in_heap(struct Heap *pq, int node);

void heap_update(struct Heap *pq, int node, struct algm_data *a);

void printHeap(struct Heap *pq, struct algm_data *a);

int main() {
    printf("Graph 0:\n");
    struct graph *g = create_graph0();
    graph_display(g);

    printf("Output of Dijkstra's algorithm\n");
    struct algm_data *a = dijkstra(g, 0);
    algm_data_display(a);

    algm_data_destroy(a);
    graph_destroy(g);
    printf("\n");

    printf("Graph 1:\n");
    g = create_graph1();
    graph_display(g);

    printf("Output of Dijkstra's algorithm\n");
    a = dijkstra(g, 0);
    algm_data_display(a);

    algm_data_destroy(a);
    graph_destroy(g);

    return 0;
}


struct algm_data *algm_data_create(struct graph *g, int source) {
    struct algm_data *a = (struct algm_data *) malloc(sizeof(struct algm_data));
    a->num_nodes = g->num_nodes;

    a->node = (struct algm_node *) malloc(sizeof(struct algm_node) * a->num_nodes);
    for (int i = 0; i < a->num_nodes; i++) {
        if (i != source) {
            a->node[i].dist = INFINITY;
        } else {
            a->node[i].dist = 0;
        }
        a->node[i].pred = NO_PRED;
    };

    return a;
}

void algm_data_destroy(struct algm_data *a) {
    free(a->node);
    free(a);
}

void heapify(struct Heap *minheap, int index, struct algm_data *a) {
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int min = index;
#ifdef DEBUG
    //printf("left  = %i\n", left);
    //printf("right = %i\n", right);
    //printf("index = %i\n", index);
#endif
    if (left >= minheap->size || left < 0) {
        left = -1;
    }
    if (right >= minheap->size || right < 0) {
        right = -1;
    }
    if (left != -1 && a->node[minheap->arr[left]].dist < a->node[minheap->arr[index]].dist) {
        min = left;
    }
    if (right != -1 && a->node[minheap->arr[right]].dist < a->node[minheap->arr[index]].dist) {
        min = right;
    }
    if (min != index) {
        int temp = minheap->arr[min];
        minheap->arr[min] = minheap->arr[index];
        minheap->arr[index] = temp;
        heapify(minheap, min, a);
    }
}

struct Heap *heap_create(struct algm_data *a) {
    struct Heap *minHeap = (struct Heap *) malloc(sizeof(struct Heap));
#ifdef DEBUG
    //printf("allocated memory for minHeap\n");
#endif
    minHeap->size = 0;
    minHeap->capacity = a->num_nodes;
#ifdef DEBUG
    //printf("will now allocate memory for arr\n");
    //printf("a->num_nodes = %i\n", a->num_nodes);
#endif
    minHeap->arr = (int *) malloc(a->num_nodes * sizeof(int));
#ifdef DEBUG
    //printf("arr allocated\n");
#endif
    int i;
    for (i = 0; i < a->num_nodes; i++) {
#ifdef DEBUG
        //printf("minHeap->arr[%i] = %i\n", i, i);
#endif
        minHeap->arr[i] = i;
    }
#ifdef DEBUG
    //printf("minHeap->size = %i\n", i);
#endif
    minHeap->size = i;
    i = (minHeap->size - 2) / 2;
#ifdef DEBUG
    //printf("starting while loop\n");
#endif
    while (i >= 0) {
        heapify(minHeap, i, a);
        i--;
    }
#ifdef DEBUG
    //printf("returning minHeap\n");
#endif
    return minHeap;
}

void heap_destroy(struct Heap *h) {
    free(h->arr);
    free(h);
}

void heapUpdater(struct Heap *h, int index, struct algm_data *a) {
    int parent = (index - 1) / 2;

    if (a->node[h->arr[parent]].dist > a->node[h->arr[index]].dist) {
        int temp = h->arr[parent];
        h->arr[parent] = h->arr[index];
        h->arr[index] = temp;
        heapUpdater(h, parent, a);
    }
}

int heap_get(struct Heap *pq, struct algm_data *a) {
    if (pq->size <= 0) {
        return -1;
    }

    int min_node = pq->arr[0];;
    int min_dist = a->node[pq->arr[0]].dist;
    for (int i = 0; i < pq->size; i++) {
        if (a->node[pq->arr[i]].dist < min_dist) {
            min_dist = a->node[pq->arr[i]].dist;
            min_node = pq->arr[i];
        }
    }
    pq->arr[0] = pq->arr[pq->size - 1];
    pq->size--;
    heapify(pq, 0, a);
    return min_node;
}

int in_heap(struct Heap *pq, int node) {
    // Return 1 if the node is in the heap, 0 otherwise
    for(int i = 0; i < pq->size; i++) {
        if(node == pq->arr[i]) {
            return 1;
        }
    }
    return 0;
}

void printHeap(struct Heap *pq, struct algm_data *a) {
    if(pq->size <= 0) {
        printf("empty heap\n");
        return;
    }
    printf("printing Heap\n");
    for(int i = 0; i < pq->size; i++) {
        //printf(" [%d] = %d ", i, pq->arr[i]);
        printf("%d ", a->node[pq->arr[i]].dist);
    }
    printf("\n");
}

void heap_update(struct Heap *pq, int node, struct algm_data *a) {
#ifdef DEBUG
    printf("heap_update node %i\n",node);
#endif
    if(pq->size <= 0) {
        return;
    }
    for(int i = 0; i < pq->size; i++) {
        if(pq->arr[i] == node) {
            heapUpdater(pq, i, a);
            return;
        }
    }
    /*for(int i = pq->size; i >= 0; i--) {
        heapify(pq, i, a);
    }
    for(int i = 0; i <= pq->size; i++) {
        heapify(pq, i, a);
    }
    for(int i = pq->size; i >= 0; i--) {
        heapify(pq, i, a);
    }*/
}

struct priorityq *priorityq_create(struct algm_data *a) {
    struct priorityq *pq = (struct priorityq *) malloc(sizeof(struct priorityq));

    pq->node = (int *) malloc(sizeof(int) * a->num_nodes);
    pq->in_q = (int *) malloc(sizeof(int) * a->num_nodes);
    pq->length = 0;

    for (int i = 0; i < a->num_nodes; i++) {
        pq->node[pq->length] = i;
        pq->in_q[pq->length] = 1;
        pq->length++;
    }

    return pq;

}

void priorityq_destroy(struct priorityq *pq) {
    free(pq->node);
    free(pq->in_q);
    free(pq);
}

int priorityq_not_empty(struct priorityq *pq) {
    if (pq->length > 0) return 1;
    else return 0;
}

int priorityq_in_q(struct priorityq *pq, int node) {
    return pq->in_q[node];
}


int priorityq_get(struct priorityq *pq, struct algm_data *a) {
    if (pq->length <= 0) return -1;

/* Find the nearest node */
    int min_node = pq->node[0];
    int min_dist = a->node[pq->node[0]].dist;
    for (int i = 0; i < pq->length; i++) {
        if (a->node[pq->node[i]].dist < min_dist) {
            min_dist = a->node[pq->node[i]].dist;
            min_node = pq->node[i];
        }
    }

    int k;
    for (k = 0; pq->node[k] != min_node; k++);
    for (int i = k; i + 1 < pq->length; i++) {
        pq->node[i] = pq->node[i + 1];
    }
    pq->length--;
    pq->in_q[min_node] = 0;

    return min_node;
}

void priorityq_update(struct priorityq *pq, struct algm_data *a, int node) {
    return;
}

struct graph *graph_create(int num_nodes) {
    struct graph *g = (struct graph *) malloc(sizeof(struct graph));
    g->num_nodes = num_nodes;
    g->adjlist = (struct adjlist_node **) malloc(sizeof(struct adjlist_node *) * num_nodes);
    for (int i = 0; i < num_nodes; i++) {
        g->adjlist[i] = NULL;
    }

    return g;
}

void graph_destroy(struct graph *g) {
    for (int i = 0; i < g->num_nodes; i++) {
        for (struct adjlist_node *p = g->adjlist[i]; p != NULL;) {
            struct adjlist_node *temp = p;
            p = p->next;
            free(temp);
        }
    }
    free(g->adjlist);
    free(g);
}

void graph_insert_link(struct graph *g, int head_node, int tail_node, int weight) {
    struct adjlist_node *new_node = (struct adjlist_node *) malloc(sizeof(struct adjlist_node));
    new_node->weight = weight;
    new_node->node_id = tail_node;
    new_node->next = g->adjlist[head_node];
    g->adjlist[head_node] = new_node;
}

void graph_display(struct graph *g) {
    printf("Graph: -> node_id (weight)\n");
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %2d: ", i);
        for (struct adjlist_node *p = g->adjlist[i]; p != NULL; p = p->next) {
            printf("->%2d (%2d) ", p->node_id, p->weight);
        }
        printf("\n");
    }
}


struct algm_data *dijkstra(struct graph *g, int source) {
    struct algm_data *a = algm_data_create(g, source);
#ifndef TEST
    struct priorityq *pq = priorityq_create(a);

    while (priorityq_not_empty(pq)) {
        int new_node = priorityq_get(pq, a);
        for (struct adjlist_node *p = g->adjlist[new_node]; p != NULL; p = p->next) {
            if (priorityq_in_q(pq, p->node_id) == 1) {                          // should not need a for loop to check if the node is in queue
#ifdef DEBUG
                printf("new_node = %i\n", new_node);
#endif
                if (a->node[p->node_id].dist > a->node[new_node].dist + p->weight) {
                    a->node[p->node_id].dist = a->node[new_node].dist + p->weight;
                    a->node[p->node_id].pred = new_node;
                    priorityq_update(pq, a, p->node_id);    // Does not do anything
                }
            }
        }
    }

    priorityq_destroy(pq);
    return a;
#endif
#ifdef TEST
    struct Heap *pq = heap_create(a);
#ifdef DEBUG
    //printHeap(pq);
#endif
    while (pq->size > 0) {
#ifdef DEBUG
        printf("\n");
        printHeap(pq, a);
#endif
        int new_node = heap_get(pq, a);

#ifdef DEBUG
        printf("heap_get returned %i\n",new_node);
        printHeap(pq, a);
        printf("\n");
#endif
        for (struct adjlist_node *p = g->adjlist[new_node]; p != NULL; p = p->next) {
            if (in_heap(pq, p->node_id) == 1) {
#ifdef DEBUG
                //printf("test\n");
#endif
                if (a->node[p->node_id].dist > a->node[new_node].dist + p->weight) {
                    a->node[p->node_id].dist = a->node[new_node].dist + p->weight;
                    a->node[p->node_id].pred = new_node;
                    heap_update(pq, p->node_id, a);
                }
            }
        }
    }
#ifdef DEBUG
    //printf("count = %i\n", count);
#endif
    heap_destroy(pq);
    return a;
#endif
}

void algm_data_display(struct algm_data *a) {
    for (int i = 0; i < a->num_nodes; i++) {
        printf("Node %2d: dist = %2d, pred = %2d\n", i, a->node[i].dist, a->node[i].pred);
    }
}

struct graph *create_graph0() {
    struct graph *g = graph_create(6);
    graph_insert_link(g, 0, 1, 4);
    graph_insert_link(g, 0, 2, 1);
    graph_insert_link(g, 1, 3, 2);
    graph_insert_link(g, 1, 4, 2);
    graph_insert_link(g, 2, 1, 1);
    graph_insert_link(g, 2, 3, 5);
    graph_insert_link(g, 2, 4, 4);
    graph_insert_link(g, 3, 5, 4);
    graph_insert_link(g, 4, 2, 1);
    graph_insert_link(g, 4, 5, 1);
    return g;
}

struct graph *create_graph1() {
    struct graph *g = graph_create(10);
    graph_insert_link(g, 0, 1, 2);
    graph_insert_link(g, 0, 2, 1);
    graph_insert_link(g, 1, 3, 2);
    graph_insert_link(g, 2, 5, 10);
    graph_insert_link(g, 2, 4, 20);
    graph_insert_link(g, 3, 5, 1);
    graph_insert_link(g, 3, 6, 10);
    graph_insert_link(g, 4, 2, 1);
    graph_insert_link(g, 4, 6, 1);
    graph_insert_link(g, 4, 9, 1);
    graph_insert_link(g, 5, 7, 1);
    graph_insert_link(g, 6, 4, 1);
    graph_insert_link(g, 6, 8, 1);
    graph_insert_link(g, 7, 8, 1);
    graph_insert_link(g, 8, 6, 1);
    graph_insert_link(g, 8, 7, 1);
    return g;
}

