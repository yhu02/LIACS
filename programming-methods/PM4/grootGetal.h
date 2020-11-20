#ifndef _grootGetal
#define _grootGetal
#include "stack.h"

struct cijfervak {
    cijfervak();
    unsigned long long data;
    cijfervak* next;
    cijfervak* prev;
};

class grootGetal{
    public:
        grootGetal();
        unsigned int id;
        unsigned int total;
        struct cijfervak* begin;
        struct cijfervak* end;
        long long fibonacci(unsigned int);
        void fibonaccif();
        void leesGetal();
        void vermenigvuldig(grootGetal*, grootGetal*);
        void add(grootGetal*, grootGetal*);
        void leesList(unsigned long long);
        void displayList();
        void drukAf(grootGetal*, grootGetal*);
        void submenu(grootGetal*, grootGetal*, stack<grootGetal>&);
        void splitGetal(long long);
    private:
        void kopieer(grootGetal*);
        void vernietig();
        void voegvoor(unsigned long long);
        void voegachter(unsigned long long);
};

char leesKarakter();
void infoBlok();
#endif
