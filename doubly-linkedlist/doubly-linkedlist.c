#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    Node* tail;
    int size;
} List;

// --- Function Prototypes ---
Node* create_node(int num);
List* init_list();
void destroy_list(List* list);

Node* insert_at_beginning(List* list, int num);
Node* insert_at_end(List* list, int num);
Node* insert_before(List* list, Node* node, int num);
Node* insert_after(List* list, Node* node, int num);

int delete_node(List* list, Node* node);

void print_node(Node* node);
void traverse_and_execute(List* list, void (*callback)(Node*));
Node* search_list(List* list, int target);

// --- Implementation of Functions ---
Node* create_node(int num) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        perror("Failed to allocate memory for a new node.\n");
        return NULL;
    }
    node->key = num;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

List* init_list() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("Failed to allocate memory for a new list.\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void destroy_list(List* list) {
    if (!list) { return; }
    Node* curr = list->head;
    Node* next;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
    printf("List destroyed successfully.\n");
}

void print_node(Node* node) {
    if (node) {
        printf("[%d] ", node->key);
    }
}

Node* insert_at_beginning(List* list, int num) {
    if (!list) {
        return NULL;
    }
    Node* node = create_node(num);
    if (node == NULL) {
        return NULL;
    }
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->size++;
    return node;
}

Node* insert_at_end(List* list, int num) {
    if (!list) {
        return NULL;
    }
    Node* node = create_node(num);
    if (node == NULL) {
        return NULL;
    }
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return node;
}

Node* insert_before(List* list, Node* node, int num) {
    if (!list || !node) {
        return NULL;
    }
    Node* newNode = create_node(num);
    if (newNode == NULL) {
        return NULL;
    }
    if (node == list->head) {
        return insert_at_beginning(list, num);
    } else {
        node->prev->next = newNode;
        newNode->prev = node->prev;
        newNode->next = node;
        node->prev = newNode;
    }
    list->size++;
    return newNode;
}

Node* insert_after(List* list, Node* node, int num) {
    if (!list || !node) {
        return NULL;
    }
    Node* newNode = create_node(num);
    if (newNode == NULL) {
        return NULL;
    }
    if (node == list->tail) {
        return insert_at_end(list, num);
    } else {
        newNode->next = node->next;
        node->next->prev = newNode;
        node->next = newNode;
        newNode->prev = node;
    }
    list->size++;
    return newNode;
}

int delete_node(List* list, Node* node) {
    if (!list || !node) {
        return 0;
    }
    if (node == list->head) {
        list->head = node->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        } else {
            list->tail = NULL;
        }
        node->next = NULL;
    } else if (node == list->tail) {
        list->tail = node->prev;
        if (list->tail != NULL) {
            list->tail->next = NULL;
        } else {
            list->head = NULL;
        }
        node->prev = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev = NULL;
        node->next = NULL;
    }
    free(node);
    list->size--;
    return 1;
}

void traverse_and_execute(List* list, void (*callback)(Node*)) {
    if (!list) { return; }
    Node* curr = list->head;
    printf("List: Head -> ");
    while (curr != NULL) {
        callback(curr);
        curr = curr->next;
    }
    printf("<- Tail\n");
}

Node* search_list(List* list, int target) {
    if (!list) { return NULL; }
    Node* curr = list->head;
    while (curr != NULL) {
        if (curr->key == target) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
