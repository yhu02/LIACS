#ifndef _grootGetal
#define _grootGetal
#include "stack.h"

//Node in grootgetal
struct cijfervak {
    cijfervak();
    ~cijfervak();
    unsigned long long data;
    cijfervak* next;
    cijfervak* prev;
};

//Linked list getallenreeks
class grootGetal{
    public:
        grootGetal();
        ~grootGetal();
        void submenu(grootGetal*, grootGetal*, stack<grootGetal>&);
        void drukAf(grootGetal*, grootGetal*);
    private:
        unsigned int total;
        unsigned int id;
        cijfervak* begin;
        cijfervak* end;

        void kopieer(grootGetal*);
        void vernietig();
        void voegvoor(unsigned long long);
        void voegachter(unsigned long long);

        void vermenigvuldig(grootGetal*, grootGetal*);
        void add(grootGetal*, grootGetal*);
        long long fibonacci(unsigned int);
        void fibonaccif();
        void splitGetal(unsigned long long);
        void leesGetal();
        void leesList(unsigned long long);
        void displayList();
};

char leesKarakter();
void infoBlok();
#endif