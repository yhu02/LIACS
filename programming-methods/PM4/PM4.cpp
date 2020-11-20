#include <string>
#include <iostream>

#include "stack.h"
#include "grootGetal.h"

using namespace std;
//Programma van Yvo Hu en Wietske Holwerda
//Dit programma maakt een nonogram
//Laatste update 17-11-2020
//Gebruikte compiler GNU g++

//Main
int main(){
    infoBlok();
    grootGetal *A = new grootGetal;
    grootGetal *B = new grootGetal;
    grootGetal *C = new grootGetal;
    stack<grootGetal>ggA, ggB, ggC;
    while(true){
        cout << "Kies uit 1 van de volgende getallen: A, B, C of (X)Stop" << endl;
        A->drukAf(B,C);
        switch(leesKarakter()){
        case('A'):
        case('a'):
            A->submenu(B, C, ggA);
            continue;
        case('B'):
        case('b'):
            B->submenu(C, A, ggB);
            continue;
        case('C'):
        case('c'):
            C->submenu(A, B, ggC);
            continue;
        case('X'):
        case('x'):
            break;
        default:
            cout << "Ongeldige keuze, probeer opnieuw";
            continue;
        }
        break;
    }
    return 0;
}