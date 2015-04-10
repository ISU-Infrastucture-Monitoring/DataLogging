
#include "fifo.h"

void fifo_init(fifo_t* this) {
    this->write_index = 0;
    this->read_index = 0;
    this->size = 0;
    this->data[0] = 0;
}

inline void fifo_push(fifo_t* this, char element) {
    if (!element) return;
    this->data[this->write_index] = element;
    this->write_index++;
    this->size++;
    this->data[this->write_index] = 0;
}

inline char fifo_pop(fifo_t* this) {
    char ret = this->data[this->read_index];
    if (ret) {
        this->read_index++;
        this->size--;
    }
    return ret;
}
