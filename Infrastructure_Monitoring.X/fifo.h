/* 
 * File:   fifo.h
 * Author: eidelon
 *
 * Created on April 9, 2015, 6:56 PM
 */

#ifndef FIFO_H
#define	FIFO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef struct FIFO_STRUCT{
        char data[256];
        unsigned char write_index;
        unsigned char read_index;
        unsigned char size;
    }
    fifo_t;

    void fifo_init(fifo_t * this);
    inline void fifo_push(fifo_t * this, char element);
    inline char fifo_pop(fifo_t * this);

#ifdef	__cplusplus
}
#endif

#endif	/* FIFO_H */

