#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int   id;
    char *body;     
} Msg;

#define QCAP 16
typedef struct {
    Msg *inbox[QCAP];   int head, tail, inbox_n;   
    Msg *log[QCAP];     int log_n;          
} Broker;

typedef void (*Subscriber)(Msg *m);

static Msg *msg_new(int id, const char *body) {
    Msg *m = malloc(sizeof *m);
    if (!m) exit(1);
    m->id = id;
    m->body = malloc(strlen(body) + 1);
    if (!m->body) exit(1);
    strcpy(m->body, body);
    return m;
}

static void msg_free(Msg *m) {
    free(m->body);
    free(m);
}

static void publish(Broker *b, int id, const char *body) {
    if (b->log_n == QCAP) return;

    Msg *m = msg_new(id, body);
    b->inbox[b->tail] = m;
    b->tail = (b->tail + 1) % QCAP;
    b->log[b->log_n++] = m;
    b->inbox_n++;   
}

static void deliver(Broker *b, Subscriber sub) {
    while (b->inbox_n > 0) {
        Msg *m = b->inbox[b->head];
        b->head = (b->head + 1) % QCAP;
        sub(m);
        b->inbox_n--;                    
    }
}

static void on_message(Msg *m) {
    printf("recv #%d: %s\n", m->id, m->body);                        
}

static void broker_shutdown(Broker *b) {
    for (int i = 0; i < b->log_n; i++){
      msg_free(b->log[i]);
      b->log[i] = NULL;
      b->inbox[i] = NULL;
    }

    b->head = b->tail = 0;
    b->inbox_n = b->log_n = 0;
}

int main(void) {
    Broker b = { .head = 0, .tail = 0, .inbox_n = 0, .log_n = 0 };

    publish(&b, 1, "hello");
    publish(&b, 2, "world");
    publish(&b, 3, "broker");

    deliver(&b, on_message);             

    broker_shutdown(&b);                 
    printf("done\n");
    return 0;
}
