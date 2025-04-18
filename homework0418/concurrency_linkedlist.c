#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct node_t {
    int key;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* head;
    pthread_mutex_t lock;
    int len;
} linked_list_t;

void init_linked_list(linked_list_t *L);
void destroy_linked_list(linked_list_t *L);
int insert_linked_list(linked_list_t *L, int key, int pos);
int lookup_linked_list(linked_list_t*L, int key);
void traverse_linked_list(linked_list_t* L);
int delete(linked_list_t* L, int pos);

int main() {
    linked_list_t L;
    init_linked_list(&L);

    for (int i = 0; i < 10; i++) {
        insert_linked_list(&L, i*2, 0);
    }
    insert_linked_list(&L, 2025, 2);
    insert_linked_list(&L, 1609, 11);
    traverse_linked_list(&L);
    delete(&L, 12);
    traverse_linked_list(&L);
    delete(&L, 11);
    traverse_linked_list(&L);
    delete(&L, 0);
    traverse_linked_list(&L);

    destroy_linked_list(&L);
    return 0;
}

void init_linked_list(linked_list_t *L) {
    L->head = NULL;
    L->len = 0;
    pthread_mutex_init(&L->lock, NULL);
}

void destroy_linked_list(linked_list_t *L) {
    node_t* curr = L->head;
    while (curr != NULL) {
        node_t* temp = curr;
        curr = curr -> next;
        free(temp);
    }
}

int insert_linked_list(linked_list_t *L, int key, int pos) {
    if (pos < 0 || pos > L->len) {
        perror("Insert failed. No access to this position!");
        pthread_mutex_unlock(&L->lock);
        return -1;
    }
    node_t* new = malloc(sizeof(node_t));
    if (new == NULL) {
        perror("Failed to create new node");
        pthread_mutex_unlock(&L->lock);
        return -1;
    }
    new->key = key;
    // critical section
    pthread_mutex_lock(&L->lock);
    if (pos == 0) {
        new->next = L->head;
        L->head = new;
    }
    else if (0 < pos <= (L->len)-1) {
        node_t* pre_node = L->head;
        for (int i = 0; i < pos-1; i++) {
            pre_node = pre_node->next;
        }
        new->next = pre_node->next;
        pre_node->next = new;
    } else {
        node_t* last_node = L->head;
        for (int i = 0; i <= ((L->len)-1); i++)
            last_node = last_node -> next;
        last_node->next = new;
        new->next = NULL;
    }
    (L->len)++;
    printf("[Insert] %4d at %2d position\n", key, pos);
    pthread_mutex_unlock(&L->lock);
    return 0;
}

int delete(linked_list_t* L, int pos) {
    pthread_mutex_lock(&L->lock);
    if (pos >= (L->len) || pos < 0) {
        fprintf(stderr, "[Delete failed], no access to %d, current len is %d\n", pos, L->len);
        pthread_mutex_unlock(&L->lock);
        return -1;
    }
    node_t* del_node;
    if (pos == 0) {
        del_node = L->head;
        L->head = del_node->next;
    } else {
        int n_cnt = 0;
        node_t* pre = L->head;
        while (n_cnt != pos - 1) {
            pre = pre->next;
            n_cnt++;
        }
        del_node = pre->next;
        pre->next = NULL;
    }
    free(del_node);
    (L->len)--;
    pthread_mutex_unlock(&L->lock);
    printf("[Delete] node at %d\n", pos);
    return 0;
}

int lookup_linked_list(linked_list_t* L, int key) {
    int status = -1; // sucess indicator
    pthread_mutex_lock(&L->lock);
    node_t* curr = L->head;
    while(curr) {
        if (curr->key == key) {
            status = 0;
            break;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return status;
}

void traverse_linked_list(linked_list_t* L) {
    pthread_mutex_lock(&L->lock);
    node_t* curr = L->head;
    printf("[Traverse] ");
    while(curr) {
        printf("%d -> ", curr->key);
        curr = curr->next;
    }
    printf("NULL\n");
    pthread_mutex_unlock(&L->lock);
}
