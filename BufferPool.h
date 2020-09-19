//
// Created by hassan on 2020-09-17.
//

#ifndef FINAL_PROJECT_BUFFERPOOL_H
#define FINAL_PROJECT_BUFFERPOOL_H

#include <string>

class BufferPool {
    /*
        Will be used when performing search
        Will be bypassed when performing import
    */
    // Should be capable of buffering up to 15 records (will use LRU replacement)
    // You may use any structure you like to organize the pool slots
    // It can both store GIS record objects or just strings

    // You must be able to display the contents of the buffer pool, listed from MRU to LRU entry, in a readable manner.
    public:
        BufferPool() = default;
        BufferPool( const BufferPool & rhs ) = default; // Copy Constructor
        BufferPool( BufferPool && rhs ) = default; // Move Constructor
        BufferPool & operator= ( const BufferPool & rhs ) = default; // Copy Assignment
        BufferPool & operator= ( BufferPool && rhs ) = default; // Move Assignment
        ~BufferPool() = default;

        std::string str();
};


#endif //FINAL_PROJECT_BUFFERPOOL_H
