#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

//Programma van Yvo Hu en Wietske Holwerda
//Dit programma maakt een nonogram
//Laatste update 17-11-2020
//Gebruikte compiler GNU g++

using namespace std;
//A doubly linked list cijfervak

static int k = 2;//2^64 past in minimaal 19 karakters dus k  is <=19
struct cijfervak {
    cijfervak(){
        next = prev = nullptr;
    }
    ~cijfervak(){
    }
    unsigned long long data;
    struct cijfervak* next;
    struct cijfervak* prev;
}; //end struct cijfervak

class grootGetal{
    public:
        grootGetal(){
            begin = NULL;
            end = NULL;
            total = 0;
        }
        ~grootGetal(){
            delete begin;
            delete end;
        }
        int total;
        struct cijfervak* begin;
        struct cijfervak* end;
        void print();
        void leesin();
        void telop();
        void vermenigvuldig();
    private:
        void voegvoor();
        void voegachter();
        void vernietig();
        void kopieer();
}; //end class grootGetal

//insert new cijfervak at the end of the list
void insert_end(grootGetal& getal, unsigned long long new_data){
    //allocate memory for cijfervak
    struct cijfervak* newcijfervak = new cijfervak;
    struct cijfervak* last = getal.begin; //set last cijfervak value to begin

    //set data for new cijfervak
    newcijfervak->data = new_data;

    //check if list is empty, if so make new cijfervak the begin of list
    if (getal.begin == NULL) {
    newcijfervak->prev = NULL;
    getal.begin = newcijfervak;
    return;
    }

    //otherwise traverse the list to go to last cijfervak
    while(last->next != NULL){
        last = last->next;
    }

    //set next of last to new cijfervak
    last->next = newcijfervak;

    //set last to prev of new cijfervak
    newcijfervak->prev = last;
    return;
} //end insert_end

void splitGetal(long i, grootGetal& a) {
    //Recursie voor getallen groter dan 9
    i > 9 ? splitGetal(i / 10,a) : void();

    long getal = i % 10;
    insert_end(a,getal);
    a.total++;
    //cout << getal << ":";
} //end splitGetal

long fibonacci(unsigned int n){
    if ((n == 0) || (n==1)) {
        return n;
    }
    else {
    return (fibonacci(n-1) + fibonacci(n-2)); //use add function?
    }
} //end fibonacci

//vernietig contents van de grootgetal
void vernietig(grootGetal& getal){
    cijfervak* temp = getal.begin;
    if(temp == nullptr){
        return;
    }
    while(temp->next != nullptr){
        temp->prev = nullptr;
        temp = temp->next;
    }
    temp = nullptr;
    getal.begin = nullptr;
    getal.end = nullptr;
} //end vernietig

void kopieer(grootGetal& A, grootGetal& B){
    cijfervak* tempA;
    cijfervak* tempB;
    cijfervak* temp = new cijfervak;
    tempB = new cijfervak;
    int value;

    if(A.begin == NULL) {
        vernietig(B);
        return;
    }else{
        tempA = A.begin;
    }
    if(B.begin != NULL) {//delete linked list if there is already a number in it
        vernietig(B);
    }

    B.begin = tempB;
    while(tempA->next !=NULL){
        value = tempA->data;
        tempB->data = value;
        tempA = tempA->next;
        tempB->next = new cijfervak;
        temp = tempB;
        tempB = tempB->next;
        tempB->prev = temp;
    }

    value = tempA->data;
    tempB->data = value;
    tempB->next = nullptr;
    B.end = tempB;
} //end kopieer

void insert_front(grootGetal& getal, unsigned long long new_data){

    //allocate memory for cijfervak
    struct cijfervak* newcijfervak = new cijfervak;
    struct cijfervak* last = getal.begin; //set last cijfervak value to begin
    newcijfervak->data = new_data;
    if(getal.begin == NULL) {
        newcijfervak->prev = NULL;
        getal.begin = newcijfervak;
        return;
    }

    while(last->prev != NULL){
        last = last->prev;
    }

    //set next of last to new cijfervak
    last->prev = newcijfervak;

    //set last to prev of new cijfervak
    newcijfervak->next = last;

    getal.begin = newcijfervak;
    return;

    //cout << "ok" << last << endl;
} //end insert_front

//Prints contents of linked list starting from the given cijfervak
void displayList(grootGetal& getal) {

    cijfervak* cijfer= getal.begin;
    while (cijfer != NULL) {
        cout<<cijfer->data<<"<==>";
        cijfer = cijfer->next;
    }

    if(cijfer == NULL){
        cout<<"NULL";
    }
    cout << endl;
} //end displayList

/*
void add(grootGetal& a, grootGetal& b){
    cijfervak* cijfervakA = a.begin;
    cijfervak* cijfervakB = b.begin;
    cijfervak* cijfervakTemp = new cijfervak;
    grootGetal temp;
    long long a_data;
    long long b_data;

    int total = a.total >= b.total ? a.total : b.total;
    int sum = 0;

    cout<< endl << cijfervakA->data;
    while(cijfervakA->next != NULL){//Check of null pointer nog niet is bereikt en traverse de list
        cijfervakA = cijfervakA->next;
        //cout<< endl << cijfervakA->data;
    }

    cout<< endl << cijfervakB->data;
    while(cijfervakB->next != NULL){
        cijfervakB = cijfervakB->next;
        //cout<< endl << cijfervakB->data;
    }

    bool endA = false;
    bool endB = false;
    bool carry = false;
    long long control = 1;

    for(int i = 0;i < k; i++){
        control *= 10;
    }

    while(total > 0){
        a_data = cijfervakA->data;
        b_data = cijfervakB->data;
        if(!endA){//Als laatste cijfervakje nog niet is bereikt, voeg waarde toe aan sum
            sum += cijfervakA->data;
        }
        if(!endB){//Als laatste cijfervakje nog niet is bereikt, voeg waarde toe aan sum
            sum += cijfervakB->data;
        }
        if(carry){
            sum++;
        }
        if(cijfervakA->prev == nullptr){
            endA = true;
        }else{
            cijfervakA = cijfervakA->prev;
        }
        if(cijfervakB->prev == nullptr){
            endB = true;
        }else{
            cijfervakA = cijfervakA->prev;
        }

        if(sum >= control){
            carry = true;
            sum = sum - control;
            insert_front(temp,sum);
            sum = 0;
        }

    }
    return temp.begin;
} //end add
*/

unsigned long long leesGetal(unsigned int, grootGetal&);
char leesKarakter();
void infoBlok();
void grootGetal::print(){}
void grootGetal::leesin(){}
void grootGetal::telop(){}
void grootGetal::vermenigvuldig(){}
void grootGetal::voegvoor(){}
void grootGetal::voegachter(){}
void grootGetal::vernietig(){}
void grootGetal::kopieer(){}

unsigned long long leesGetal2(unsigned int grootte, grootGetal& getal){
    //Bereken grootte van het getal zodat de cijfers goed passen in de cijfervakken voor fibonacci
    grootGetal temp;
    cijfervak* tempcijfervak;
    bool flag = false;
    unsigned long long sum = 0;
    int count = 0;
    int totalcount = 4;

    splitGetal(fibonacci(20),temp);
    //ceil aantal cijfers / grootte van cijfervak = aantal gebruikte cijfervakken
    getal.total = totalcount%grootte == 0 ? totalcount/grootte : (totalcount/grootte) + 1;
    if(true){
        tempcijfervak = temp.begin;
        cout << "OK:" << tempcijfervak->data;

        while(tempcijfervak->next != nullptr){
            tempcijfervak = tempcijfervak->next;
        }

        int mult = 0;
        int num = tempcijfervak->data;
        //sum+= num;

        while(true){
            if(tempcijfervak->prev != nullptr){
                for(int i = 0; i < grootte; i++){
                    if(tempcijfervak->prev != nullptr){
                        tempcijfervak->next = nullptr;//cleanup
                        num = tempcijfervak->data;
                        //cout << endl << tempcijfervak->data << " no" << tempcijfervak << endl;
                        tempcijfervak = tempcijfervak->prev;
                        for(int i = 0; i < mult; i++){
                            num*= 10;
                        }
                        mult++;
                        sum += num;
                    }
                }
                if(tempcijfervak->prev == nullptr && mult != grootte){
                    num = tempcijfervak->data;
                    for(int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }else if(tempcijfervak->prev == nullptr && mult == grootte){
                    insert_front(getal,sum);
                    mult = 0;
                    sum = 0;
                    num = tempcijfervak->data;
                    for(int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }
                insert_front(getal,sum);
                mult = 0;
                sum = 0;
            }

            if(totalcount == 1){//Case als slechts 1 cijfer
                insert_front(getal,tempcijfervak->data);
            }
            if(tempcijfervak->prev == nullptr){//laatste cijfervak bereikt herhaal nog 1 keer
                break;
            }
        }
    }
    //Check of er een getal ingevuld is
    return 1;
} //end leesGetal2

//main program
int main() {
    cout << "Kies uit 1 van de volgende getallen: A, B, C" << endl;
    grootGetal A;
    grootGetal B;
    grootGetal C;

    while(true){
        cout << "A:";
        displayList(A);
        cout << "B:";
        displayList(B);
        cout << "C:";
        displayList(C);
        switch(leesKarakter()){

        case('A'):
        case('a'):
            if(leesGetal(k, A) == -1){
                cout << "Ongeldige invoer, typ een getal tussen de -2^63 en 2^63 in" << endl;
                continue;
            }
            break;

        case('B'):
        case('b'):
            if(leesGetal(k, B) == -1){
                cout << "Ongeldige invoer, typ een getal tussen de -2^63 en 2^63 in" << endl;
                continue;
            }
            break;

        case('C'):
        case('c'):
            if(leesGetal(k, C) == -1){
                cout << "Ongeldige invoer, typ een getal tussen de -2^63 en 2^63 in" << endl;
                continue;
            }
            break;

        case('D'):
        case('d'):
            vernietig(A);
            //add(A, B, C, k);
            break;

        case('E'):
        case('e'):
            leesGetal2(k,C);
            //kopieer(A,B);
            //add(A, B, C, k);
            break;

        default:
            cout << "Ongeldige keuze, probeer opnieuw";
            continue;
        }
        break;

    }
    system("pause");
    return 0;
} //end main

void infoBlok(){
    cout << endl << endl;
    for(unsigned int i = 1; i<44;i++){
        cout << '*';
    }
    cout << endl;
    cout << "* Yvo Hu studentnummer: 2962802           *" << endl;
    cout << "* Wietske Holwerda studentnummer: 2838192 *" << endl;
    cout << "* Studiejaar: 2020                        *" << endl;
    cout << "* Studierichting: Informatica             *" << endl;
    cout << "* Dit programma manipuleert linked lists  *" << endl;
    for(unsigned int i = 1; i<44;i++){
        cout << '*';
    }
    cout << endl;
} //end infoBlok

//leesGetal voor iostream
unsigned long long leesGetal(unsigned int grootte, grootGetal& getal){
    //Bereken grootte van het getal zodat de cijfers goed passen in de cijfervakken
    grootGetal temp;
    cijfervak* tempcijfervak;
    bool flag = false;
    unsigned long long sum = 0;
    char letter = cin.get();
    int count = 0;
    int totalcount = 0;

    while(true){
        if(!isdigit(letter) && letter != '\n'){
            letter = cin.get();
            continue;
        }else if(letter == '\n'){
            break;
        }

        while(count < grootte){
            if(!isdigit(letter) && letter != '\n'){
                letter = cin.get();
                continue;
            }else if(letter == '\n'){
                break;
            }
            //Indien meer iteraties, vermenigvuldig met 10

            sum *= sum > 0 ? 10 : sum;
            sum += (letter - '0');
            letter = cin.get();
            flag = true;
            count++;
            insert_end(temp, sum);
            totalcount++;
            sum = 0;
            break;
        }
        count = 0;
    }//ceil aantal cijfers / grootte van cijfervak = aantal gebruikte cijfervakken

    getal.total = totalcount%grootte == 0 ? totalcount/grootte : (totalcount/grootte) + 1;
    if(flag){
        tempcijfervak = temp.begin;

        while(tempcijfervak->next != nullptr){
            tempcijfervak = tempcijfervak->next;
        }
        int mult = 0;
        int num = tempcijfervak->data;
        //sum+= num;

        while(true){
            if(tempcijfervak->prev != nullptr){
                for(int i = 0; i < grootte; i++){
                    if(tempcijfervak->prev != nullptr){
                        tempcijfervak->next = nullptr;//cleanup
                        num = tempcijfervak->data;
                        //cout << endl << tempcijfervak->data << " no" << tempcijfervak << endl;
                        tempcijfervak = tempcijfervak->prev;
                        for(int i = 0; i < mult; i++){
                            num*= 10;
                        }
                        mult++;
                        sum += num;
                    }
                }
                if(tempcijfervak->prev == nullptr && mult != grootte){
                    num = tempcijfervak->data;
                    for(int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }else if(tempcijfervak->prev == nullptr && mult == grootte){
                    insert_front(getal,sum);
                    mult = 0;
                    sum = 0;
                    num = tempcijfervak->data;
                    for(int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }

                insert_front(getal,sum);
                mult = 0;
                sum = 0;
            }
            if(totalcount == 1){//Case als slechts 1 cijfer
                insert_front(getal,tempcijfervak->data);
            }
            if(tempcijfervak->prev == nullptr){//laatste cijfervak bereikt herhaal nog 1 keer
                break;
            }
        }
    }
    //Check of er een getal ingevuld is
    if(flag){
        return sum;
    }else{
        return -1;
    }
} //end leesGetal

char leesKarakter(){
    while(true){
        string trash = "";
        char karakter = cin.get();
        getline(cin,trash);
        cin.clear();

        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }
        return karakter;
    }
} //end lessKarakter
