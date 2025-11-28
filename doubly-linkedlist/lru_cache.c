#include <stdio.h>
#include <stdlib.h>

// --- Game Constants ---
#define CACHE_CAPACITY 5 // Maximum number of items the cache can hold
#define HASH_SIZE 100    // Size of the array used for hash map simulation. Keys must be < HASH_SIZE.

// --- Data Structures ---

/**
 * @struct CacheNode
 * Represents an element in the DLL and stores the key/value pair.
 * The DLL maintains the recency order (Head=MRU, Tail=LRU).
 */
typedef struct CacheNode {
    int key;            // The key (used for hash map lookup)
    int value;          // The actual data/value
    struct CacheNode* prev; // Pointer to the previous node (for DLL)
    struct CacheNode* next; // Pointer to the next node (for DLL)
} CacheNode;

/**
 * @struct LRUCache
 * Holds the core structure components.
 */
typedef struct LRUCache {
    int capacity;       // Max size of the cache
    int size;           // Current number of items in the cache

    // Doubly Linked List Pointers (Recency Order)
    CacheNode* head;    // Most Recently Used (MRU)
    CacheNode* tail;    // Least Recently Used (LRU)

    // Hash Map Simulation (Lookup: Key -> Node Pointer)
    CacheNode* map[HASH_SIZE]; // Array where index is the key, value is the Node pointer
} LRUCache;


// --- DLL Helper Prototypes (Specialized for LRU) ---
CacheNode* create_node(int key, int value);
void remove_node(LRUCache* cache, CacheNode* node);
void push_to_head(LRUCache* cache, CacheNode* node);
CacheNode* pop_tail(LRUCache* cache);

// --- LRU Cache Interface Prototypes ---
LRUCache* lru_create(int capacity);
int lru_get(LRUCache* cache, int key);
void lru_put(LRUCache* cache, int key, int value);
void lru_print(LRUCache* cache);


// --- Main Function (Demonstration) ---

int main() {
    // Create a cache with a capacity of 3
    LRUCache* cache = lru_create(3);
    printf("--- LRU Cache Demonstration (Capacity: 3) ---\n");

    // 1. Put (Miss: 1 is MRU)
    printf("1. PUT(1, 10)\n");
    lru_put(cache, 1, 10);
    lru_print(cache);

    // 2. Put (Miss: 2 is MRU)
    printf("2. PUT(2, 20)\n");
    lru_put(cache, 2, 20);
    lru_print(cache);

    // 3. Put (Miss: 3 is MRU)
    printf("3. PUT(3, 30)\n");
    lru_put(cache, 3, 30);
    lru_print(cache); // List: [3, 2, 1]

    // 4. Get (Hit: 1 is accessed and moved to MRU/Head)
    printf("4. GET(1) -> Value: %d\n", lru_get(cache, 1));
    lru_print(cache); // List: [1, 3, 2] (2 is now LRU)

    // 5. Put (Miss: Cache is full, evict LRU [2])
    printf("5. PUT(4, 40) -> Cache full, evicting LRU node (2)\n");
    lru_put(cache, 4, 40);
    lru_print(cache); // List: [4, 1, 3]

    // 6. Put (Hit: Update key 3, move to MRU)
    printf("6. PUT(3, 33) -> Updating key 3, moving to MRU\n");
    lru_put(cache, 3, 33);
    lru_print(cache); // List: [3, 4, 1]

    // 7. Get (Miss: 2 was evicted)
    printf("7. GET(2) -> Value: %d\n", lru_get(cache, 2));

    free(cache);
    return EXIT_SUCCESS;
}


// --- LRU Cache Interface Implementation ---

/**
 * @brief Creates and initializes the LRU Cache structure.
 * @param capacity The maximum size of the cache.
 * @return Pointer to the new LRUCache structure.
 */
LRUCache* lru_create(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    if (!cache) return NULL;

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;

    // Initialize the Hash Map array to NULL
    for (int i = 0; i < HASH_SIZE; i++) {
        cache->map[i] = NULL;
    }
    return cache;
}

/**
 * @brief Retrieves a value from the cache based on the key (Cache Hit/Miss).
 * O(1) time complexity.
 * @param cache The LRU cache.
 * @param key The key to look up.
 * @return The value if found (>= 0), or -1 if the key is not in the cache (Miss).
 */
int lru_get(LRUCache* cache, int key) {
    // 1. O(1) Lookup using the Hash Map
    if (key < 0 || key >= HASH_SIZE || cache->map[key] == NULL) {
        return -1; // Cache Miss
    }

    CacheNode* node = cache->map[key];

    // 2. Cache Hit: Update recency by moving the node to the Head (MRU)
    remove_node(cache, node);
    push_to_head(cache, node);

    // The key is still pointing to the correct node in the map
    return node->value;
}

/**
 * @brief Inserts or updates a key-value pair in the cache.
 * O(1) time complexity.
 * @param cache The LRU cache.
 * @param key The key to insert/update.
 * @param value The value to store.
 */
void lru_put(LRUCache* cache, int key, int value) {
    if (key < 0 || key >= HASH_SIZE) return;

    CacheNode* node = cache->map[key];

    if (node != NULL) {
        // --- Case 1: Cache Hit (Update existing key) ---

        // 1. Update the value
        node->value = value;

        // 2. Update recency: Move existing node to the Head (MRU)
        remove_node(cache, node);
        push_to_head(cache, node);

    } else {
        // --- Case 2: Cache Miss (New key) ---

        // 1. Check for capacity and evict the LRU item if full
        if (cache->size == cache->capacity) {
            CacheNode* lru_node = pop_tail(cache);
            if (lru_node != NULL) {
                // Remove evicted node from the Hash Map
                cache->map[lru_node->key] = NULL;
                free(lru_node); // Free the memory
            }
        }

        // 2. Create the new node and insert it at the Head (MRU)
        CacheNode* newNode = create_node(key, value);
        push_to_head(cache, newNode);

        // 3. Update the Hash Map to point to the new node
        cache->map[key] = newNode;
    }
}

/**
 * @brief Prints the current state of the cache's DLL (from MRU to LRU).
 */
void lru_print(LRUCache* cache) {
    printf("Cache DLL (MRU -> LRU): [");
    CacheNode* current = cache->head;
    while (current) {
        printf("{%d:%d}", current->key, current->value);
        if (current->next) {
            printf(", ");
        }
        current = current->next;
    }
    printf("] Size: %d/%d\n", cache->size, cache->capacity);
}


// --- DLL Helper Function Implementations (O(1) specialized operations) ---

/**
 * @brief Allocates and initializes a new CacheNode.
 */
CacheNode* create_node(int key, int value) {
    CacheNode* newNode = (CacheNode*)malloc(sizeof(CacheNode));
    if (!newNode) return NULL;
    newNode->key = key;
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/**
 * @brief Detaches a node from the DLL without freeing memory.
 * This is crucial for the move_to_head operation. O(1).
 */
void remove_node(LRUCache* cache, CacheNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        // If there is no previous node, we are removing the Head
        cache->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        // If there is no next node, we are removing the Tail
        cache->tail = node->prev;
    }
    cache->size--; // Decrease size here as the node is removed from order
}

/**
 * @brief Pushes a node (new or moved) to the Head (MRU) of the DLL. O(1).
 */
void push_to_head(LRUCache* cache, CacheNode* node) {
    node->prev = NULL; // New Head has no previous node
    node->next = cache->head;

    if (cache->head != NULL) {
        cache->head->prev = node;
    } else {
        // List was empty, this is also the Tail
        cache->tail = node;
    }

    cache->head = node; // Update the Head pointer
    cache->size++;      // Increase size here as the node is added to order
}

/**
 * @brief Removes the Tail (LRU) node from the DLL. O(1).
 * @return The detached LRU node, which must be freed by the caller.
 */
CacheNode* pop_tail(LRUCache* cache) {
    if (cache->tail == NULL) {
        return NULL;
    }

    CacheNode* lru_node = cache->tail;

    // Move the tail pointer to the previous node
    cache->tail = lru_node->prev;

    if (cache->tail != NULL) {
        // The new tail's next pointer must be NULL
        cache->tail->next = NULL;
    } else {
        // If the list becomes empty, the head must also be NULL
        cache->head = NULL;
    }

    cache->size--;

    // Return the detached node
    return lru_node;
}
