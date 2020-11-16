#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

//Programma van Yvo Hu en Wietske Holwerda
//Dit programma maakt een nonogram
//Laatste update 08-11-2020
//Gebruikte compiler GNU g++

using namespace std;
// A doubly linked list cijfervak
struct cijfervak {
    cijfervak(){
        next = prev = nullptr;
    }
    unsigned long long data;
    struct cijfervak* next;
    struct cijfervak* prev;
};

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
        void fibonacci(unsigned int);
        void vermenigvuldig();
    private:
        void voegvoor();
        void voegachter();
        void vernietig();
        void kopieer();
};

//insert a new cijfervak at the end of the list
void insert_end(grootGetal& getal, unsigned long long new_data){
    //allocate memory for cijfervak
    struct cijfervak* newcijfervak = new cijfervak;

    struct cijfervak* last = getal.begin; //set last cijfervak value to begin

    //set data for new cijfervak
    newcijfervak->data = new_data;

    //check if list is empty, if yes make new cijfervak the begin of list
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
}

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
}

// This function prints contents of linked list starting from the given cijfervak
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
}

void add(grootGetal& a, grootGetal& b, grootGetal& c, unsigned int k){
    cijfervak* cijfervakA = a.begin;
    cijfervak* cijfervakB = b.begin;
    cijfervak* cijfervakC = c.begin;
    cijfervak* newcijfervak = newcijfervak;

    if(cijfervakC == nullptr){
        cijfervakC = new cijfervak;
    }

    int total = a.total >= b.total ? a.total : b.total;
    int sum = 0;

    cout<< endl << cijfervakA->data; 
    while(cijfervakA->next != NULL){//Check of null pointer nog niet is bereikt
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
    bool endA2 = false;
    bool endB2 = false;
    long long control = 1;

    for(int i = 0;i < k; i++){
        control *= 10;
    }
    while(total > -1){
        long long a_data = cijfervakA->data;
        long long b_data = cijfervakB->data;
        if(!endA){//Als laatste cijfervakje nog niet is bereikt, voeg waarde toe aan sum
            sum += cijfervakA->data;
        }
        if(cijfervakA->prev == nullptr){
            endA = true;
        }else{
            cijfervakA = cijfervakA->prev;
        }
        
        if(!endB){
            sum += cijfervakB->data;
        }
        if(cijfervakB->prev == nullptr){
            endB = true;
        }else{
            cijfervakB = cijfervakB->prev;
        }
        if(sum >= control){
            sum = sum - control;
            carry = true;
            //cout << "one" << sum << endl;
            insert_front(c,sum);
            sum = 1;
        }else{
           
            //cout << "two" << sum << endl;
            //cout << "control" << control << endl;
            insert_front(c,sum);
            carry = false;
            sum = 0;
        }
        total--;
        if(!carry){//Decrease total again if no carry occurred
            total--;
        }
    }
    //cout << endl << cijfervakA << endl << cijfervakB << endl;
}



unsigned long long leesGetal(unsigned int, grootGetal&);
char leesKarakter();
void infoBlok();

void grootGetal::print(){}
void grootGetal::leesin(){}
void grootGetal::telop(){}
void grootGetal::fibonacci(unsigned int){}
void grootGetal::vermenigvuldig(){}

void grootGetal::voegvoor(){}
void grootGetal::voegachter(){}
void grootGetal::vernietig(){}
void grootGetal::kopieer(){}


//main program
int main() {
    const unsigned int k = 5;//2^64 past in minimaal 19 karakters dus k is maximaal 19
    cout << "Kies uit 1 van de volgende getallen: A, B, C" << endl;
    grootGetal A;
    grootGetal B;
    grootGetal C;
    while(true){
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
                add(A, B, C, k);
                break;
            default:
                cout << "Ongeldige keuze, probeer opnieuw";
                continue;
            }
            break;
        }
    }
    system("pause");
    return 0;
}

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
}


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
}

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
}
