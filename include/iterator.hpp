#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>

using namespace std;

class Iterator{
    private:

    public:
        // Iterator();
        virtual ~Iterator(){};
        virtual bool atEnd() = 0;
        virtual uint64_t key() = 0;
        virtual void seek(uint64_t seekKey) = 0;
        virtual void next() = 0;
        virtual void open() = 0;
        virtual void up() = 0;
        virtual void storeToFile(string file_name) = 0;
        virtual void loadFromFile(string file_name) = 0;
        virtual void backToStart() = 0;
        bool operator < (Iterator& it) {
            return (key() < it.key());
        }
        //Temporal
        virtual void getIteratorPos() = 0;
};

#endif