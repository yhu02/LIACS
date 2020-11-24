#include "grootGetal.h"
#include "stack.h"
#include <iostream>
#include <string>

//Id voor iedere getallenreeks
static unsigned int idCount = 0;

//Cijfervak grootte
//Undefined behavior as getal in cijfervak  > 2^64
//Dat is mogelijk vanaf grootte = 20
static unsigned int grootte = 3;

//Constructor
cijfervak::cijfervak(){
    next = prev = nullptr;
    data = 0;
}

grootGetal::grootGetal(){
    id = idCount; 
    idCount++;
    total = 0;
    begin = end = nullptr;
}

//Submenu
void grootGetal::submenu(grootGetal* getal2, grootGetal* getal3,
                         stack<grootGetal> &stapel){
    while(true){
        //Delete in stack destructor
        grootGetal* temp = new grootGetal;

        for(int j = 0; j < 44; j++){
            std::cout << "*";
        }
        std::cout << std::endl;
        
        //Print 1 van de drie getallen 
        switch(this->id){
        case(0):
            this->drukAf(getal2, getal3);
            break;
        case(1):
            getal3->drukAf(this, getal2);
            break;
        case(2):
            getal2->drukAf(getal3, this);
            break;
        }

        std::cout << std::endl                                              << std::endl;
        std::cout << "Wat wil je met dit getal doen? Kies uit:"             << std::endl;
        std::cout << "(G) Een getal invoeren"                               << std::endl;
        std::cout << "(F) Een zoveelste fibonacci getal invoeren"           << std::endl;
        std::cout << "(O) De andere getallen met elkaar optellen"           << std::endl;
        std::cout << "(V) De andere getallen met elkaar vermenigvuldigen"   << std::endl;
        std::cout << "(S) Een stap terug doen"                              << std::endl;
        std::cout << "(T) Teruggaan naar het hoofdmenu"                     << std::endl;
        std::cout << "Stack:" << stapel.size                                << std::endl;
        std::cout << std::endl                                              << std::endl;

        switch(leesKarakter()){
        case('G'):
        case('g'):
            this->leesGetal();
            temp->kopieer(this);
            stapel.push(temp);
            break;
        case('F'):
        case('f'):
            this->fibonaccif();
            temp->kopieer(this);
            stapel.push(temp);
            continue;
            break;
        case('O'):
        case('o'):
            this->add(getal2, getal3);
            temp->kopieer(this);
            stapel.push(temp);
            continue;
            break;
        case('V'):
        case('v'):
            this->vermenigvuldig(getal2, getal3);
            temp->kopieer(this);
            stapel.push(temp);
            break;
        case('S'):
        case('s'):
            this->vernietig();
            stapel.pop();
            // Check of er nog elementen in de stack zitten
            if(stapel.size){
                this->kopieer(stapel.peek());
            }
            break;
        case('T'):
        case('t'):
            return;
        default:
            std::cout << "Ongeldige keuze, probeer opnieuw";
            continue;
        }
    }
}

//Print alle 3 getallenreeksen
void grootGetal::drukAf(grootGetal* getal2, grootGetal* getal3){
    std::cout << "A:";
    this->displayList();
    std::cout << "B:";
    getal2->displayList();
    std::cout << "C:";
    getal3->displayList();
    return;
}


//Kopieer getallenreeks B naar huidige getallenreeks
void grootGetal::kopieer(grootGetal* B){
    cijfervak* tempA = new cijfervak;
    cijfervak* temp;
    cijfervak* tempB;
    unsigned int num;

    //Reset
    this->vernietig();

    //Stel begin nieuwe getallenreeks
    this->begin = tempA;

    //Doe niets als de getallenreeks van B leeg is
    if(B->begin == nullptr) {
        return;
    }else{
        tempB = B->begin;
    }
    //Kopieer cijfervak van B naar een nieuw cijfervak van de nieuwe getallenreeks
    while(tempB->next !=nullptr){
        num = tempB->data;
        tempA->data = num;
        tempB = tempB->next;
        //Maak nieuw cijfervak aan en verbind deze met elkaar
        tempA->next = new cijfervak;
        temp = tempA;
        tempA = tempA->next;
        tempA->prev = temp;
    }
    //Herhaal nog een keer
    num = tempB->data;
    tempA->data = num;
    tempA->next = nullptr;
    //Stel einde nieuwe getallenreeks
    this->end = tempA;
    this->total = B->total;
}

//Vernietig huidige getallenreeks
void grootGetal::vernietig(){
    this->total = 0;
    cijfervak* temp = this->begin;

    if(temp == nullptr){
        return;
    }
    while(temp->next != nullptr){
        delete temp->prev;
        temp = temp->next;
    }

    delete temp;
    this->begin = nullptr;
    this->end = nullptr;
}

//Voeg een cijfervak toe aan de voorkant van de list
void grootGetal::voegvoor(unsigned long long new_data){
    struct cijfervak* newcijfervak = new cijfervak;
    struct cijfervak* first = this->begin; //Stel begin cijfervak naar first
    newcijfervak->data = new_data;

    //Maak nieuw cijfervak als de getallenreeks leeg is
    if(this->begin == nullptr) {
        this->begin = newcijfervak;
        return;
    }

    //Stel vorige node van first aan nieuwe node met nieuwe data
    first->prev = newcijfervak;
    //Stel volgende node van het nieuwe cijfervak aan de vorige eerste node (first)
    newcijfervak->next = first;
    //Stel begin van de getallenreeks aan het nieuwe cijfervak
    this->begin = newcijfervak;
    return;
}

//Voeg een cijfervak toe aan de achterkant van de list
void grootGetal::voegachter(unsigned long long new_data){
    cijfervak* newcijfervak = new cijfervak;
    //Stel het cijfervak last in als begin van de huidige getallenreeks
    cijfervak* last = this->begin;
    //Stel data voor het nieuwe cijfervak
    newcijfervak->data = new_data;
    //Check of getallenreeks leeg is stel het nieuwe cijfervak in als begin
    if (this->begin == nullptr) {
        this->begin = newcijfervak;
        return;
    }
    //Anders ga naar het laatste cijfervak
    while(last->next != nullptr){
        last = last->next;
    }
    //Stel newcijfervak als het laatste cijfervak
    last->next = newcijfervak;
    
    //Verbind de laatste cijfervak met de vorige
    newcijfervak->prev = last;
    return;
}

//Vermenigvuldig getallenreeks a en b en sla deze op in de huidige getallenreeks
void grootGetal::vermenigvuldig(grootGetal* a, grootGetal* b){
    cijfervak* cijfervakA = a->begin;
    cijfervak* cijfervakB = b->begin;
    unsigned long long sum = 0;
    unsigned long long numA = 0;
    unsigned long long numB = 0;
    unsigned int multA = 0;
    unsigned int multB = 0;
    unsigned int totalA = a->total;
    unsigned int totalB = b->total;
    grootGetal* temp;
    grootGetal* temp2 = new grootGetal;

    //Reset
    this->vernietig();

    //Vroegtijdige beeindiging als een van beide getallenreeks 0 is
    if (a->begin == nullptr || b->begin == nullptr ||
        cijfervakA->data == 0 || cijfervakB->data == 0){
        this->voegvoor(0);
        return;
    }
    //Check of nullptr pointer nog niet is bereikt en doorkruis de list
    while(cijfervakA->next != nullptr){
        cijfervakA = cijfervakA->next;
    }

    //Bereken het product van beide getallenreeksen
    while(totalA > 0){ 
        numA = cijfervakA->data;
        for(unsigned int i = 0; i < multA; i++){
            numA *= 10;
        }
        for(unsigned int i = 0; i < grootte;i++){
            multA++;
        }
        if(b->begin != nullptr){
            cijfervakB = b->begin;
        }
        //Check of nullptr pointer nog niet is bereikt en doorkruis de list
        while(cijfervakB->next != nullptr){
            cijfervakB = cijfervakB->next;
        }
        while(totalB > 0){
            temp = new grootGetal;
            numB = cijfervakB->data; 
            for(unsigned int i = 0; i < multB; i++){
                numB *= 10;
            }
            for(unsigned int i = 0; i < grootte; i++){
                multB++;
            }
            sum = numB * numA;
            if((numA != 0) && (sum / numA) != numB) {
                std::cout << "overflow mpotherfucker";
                //this->vernietig();
                return;
            }
            cijfervakB = cijfervakB->prev;
            totalB--;
            temp->leesList(sum);
            this->add(temp2,temp);
            temp2->kopieer(this);
            delete temp;
        }
        if(cijfervakA->prev != nullptr){
            cijfervakA= cijfervakA->prev;
        }
        multB = 0;
        totalA--;
        totalB = b->total;
    }
    delete temp2;
    return;
}

//Voeg getallenreeksen a en b bij elkaar en sla deze op in de huidige getallenreeks
void grootGetal::add(grootGetal* a, grootGetal* b){
    cijfervak* cijfervakA = a->begin;
    cijfervak* cijfervakB = b->begin;
    unsigned long long sum = 0;
    unsigned long long control = 1;
    bool endA = false;
    bool endB = false;
    unsigned int total = this->total = a->total >= b->total ? a->total : b->total;
    
    if(!total){
        this->voegvoor(0);
    }
    //Reset
    this->vernietig();
    //Grootste getal voor er carry optreedt
    for(unsigned int i = 0; i < grootte; i++){
        control *= 10;
    }
    if(a->begin == nullptr){
        cijfervakA = new cijfervak;
        cijfervakA->data = 0;
    }
    //Check of nullptr pointer nog niet is bereikt en doorkruis de list
    while(cijfervakA->next != nullptr){
        cijfervakA = cijfervakA->next;
    }
    if(b->begin == nullptr){
        cijfervakB = new cijfervak;
        cijfervakB->data = 0;
    }
    while(cijfervakB->next != nullptr){
        cijfervakB = cijfervakB->next;  
    }
    //Bereken door tot de laatste cijfervak van de grootste getallenreeks is bereikt
    for(int count = total; count > 0;){
        if(!endA){//Als laatste cijfervakje nog niet is bereikt, voeg waarde toe aan sum
            sum += cijfervakA->data;
        }
        if(!endB){//Als laatste cijfervakje nog niet is bereikt, voeg waarde toe aan sum
            sum += cijfervakB->data;
        }
        if(cijfervakA->prev == nullptr){
            endA = true;
        }else{
            cijfervakA = cijfervakA->prev;
        }
        if(cijfervakB->prev == nullptr){
            endB = true;
        }else{
            cijfervakB = cijfervakB->prev;
        }
        count--;
        //Bereken huidige cijfervak en verwerk carry naar volgende cijfervak
        if(sum >= control){ 
            if(count == 0){
                //Maak total groter als laatste cijfervak carry bevat
                //En herhaal nog een keer door count++
                count++;
                total++;
            }
            sum = sum - control;
            this->voegvoor(sum);
            sum = 1;
            continue;
        }else{
            this->voegvoor(sum);
            sum = 0;
        }
    }
    
    //Delete als memory is gealloceerd
    b->begin == nullptr ? delete cijfervakB : void();
    a->begin == nullptr ? delete cijfervakA : void();

    this->total = total;
    return;
}

//Bereken n-ste fibonacci getal; Na ongeveer n = 40 gaat het lang duren
long long grootGetal::fibonacci(unsigned int n){
    if  ((n == 0) || (n==1)){
        return n;
    }else{
        return (fibonacci(n-1) + fibonacci(n-2));
    }
}

//Maak een k-cijferige fibonacci getallenreeks van een enkelcijferige fibonacci getallenreeks
void grootGetal::fibonaccif(){
    unsigned int count = 0;
    unsigned int num = 0;
    char letter= std::cin.get();
    bool flag = false;
    //Reset
    this->vernietig();

    //Bereken fibonacci getal
    while(true){
        if(!isdigit(letter) && letter != '\n'){
            letter = std::cin.get();
            continue;
        }else if(letter == '\n'){
            break;
        }
        while(count < grootte){
            //Indien meer getallen, vermenigvuldig met 10
            num *= num > 0 ? 10 : num;
            num += (letter - '0');
            letter = std::cin.get();
            flag = true;
            count++;
            //Check voor het volgende karakter
            break;
        }
    }
    //Check of er een getal is ingevoerd
    if(flag){
        //Maak een getallenreeks van fibonacci getal
        this->leesList(fibonacci(num));
    }
    return;
}

//Splits een getal in een enkelcijferige getallenreeks
void grootGetal::splitGetal(unsigned long long i){
    //Recursie voor getallen groter dan 9
    i > 9 ? splitGetal(i / 10) : void();
    //Voeg getal toe aan getallenreeks
    long long getal = i % 10;
    this->voegachter(getal);
    this->total++;
}

//Maak een k-cijferige getallenreeks van een getal uit iostream
void grootGetal::leesGetal(){
    grootGetal* temp = new grootGetal;
    cijfervak* tempcijfervak;
    unsigned long long sum = 0;
    unsigned long long num;
    unsigned int count = 0;
    unsigned int mult = 0;
    bool flag = false;
    char letter = std::cin.get();
    //Reset
    this->vernietig();

    //Bereken grootte van het getal zodat de cijfers goed passen in de cijfervakken
    while(true){
        if(!isdigit(letter) && letter != '\n'){
            letter = std::cin.get();
            continue;
        }else if(letter == '\n'){
            break;
        }
        while(count < grootte){
            if(!isdigit(letter) && letter != '\n'){
                letter = std::cin.get();
                continue;
            }else if(letter == '\n'){
                break;
            }
            //Indien meer iteraties, vermenigvuldig met 10
            sum *= sum > 0 ? 10 : sum;
            sum += (letter - '0');
            letter = std::cin.get();
            flag = true;
            count++;
            temp->voegachter(sum);
            temp->total++;
            sum = 0;
            break;
        }
        count = 0;
    }

    //Ga verder als er een getal is ingevoerd
    if(flag){
        //Ceil aantal cijfers / grootte van cijfervak = aantal gebruikte cijfervakken
        count = this->total = temp->total%grootte == 0 ? temp->total/grootte : (temp->total/grootte) + 1;
        tempcijfervak = temp->begin;
        
        while(tempcijfervak->next != nullptr){
            tempcijfervak = tempcijfervak->next;
        }

        //Herorden de getallenreeks
        while(count > 0){
            for(unsigned int i = 0; i < grootte; i++){
                num = tempcijfervak->data;
                for(unsigned int i = 0; i < mult; i++){
                    num*= 10;
                }
                sum += num;
                if(tempcijfervak->prev == nullptr){
                    break;
                }
                mult++;
                tempcijfervak = tempcijfervak->prev;
            }
            this->voegvoor(sum);
            mult = 0;
            sum = 0;
            count--;
        }
    }
    delete temp;
    return;
}

//Maak een k-cijferige getallenreeks van een enkelcijferige getallenreeks
void grootGetal::leesList(unsigned long long i){
    grootGetal* temp = new grootGetal;
    cijfervak* tempcijfervak;
    unsigned long long sum = 0;
    unsigned long long num;
    unsigned int mult = 0;
    unsigned int count = 0;
    //Reset
    this->vernietig();

    //Maak een enkelcijferige getallenreeks van het getal
    temp->splitGetal(i);
    //Ceil aantal cijfers / grootte van cijfervak = aantal gebruikte cijfervakken
    count = this->total = temp->total % grootte == 0 ? temp->total / grootte : (temp->total / grootte) + 1;
    tempcijfervak = temp->begin;

    while(tempcijfervak->next != nullptr){
        tempcijfervak = tempcijfervak->next;
    }

    //Herorden de getallenreeks
    while(count > 0){
        for(unsigned int i = 0; i < grootte; i++){
            num = tempcijfervak->data;
            for(unsigned int i = 0; i < mult; i++){
                num*= 10;
            }
            sum += num;
            if(tempcijfervak->prev == nullptr){
                break;
            }
            mult++;
            tempcijfervak = tempcijfervak->prev;
        }
        this->voegvoor(sum);
        mult = 0;
        sum = 0;
        count--;
    }
    delete temp;
    return;
}

//Print de huidige getallenreeks
void grootGetal::displayList() {
    cijfervak* cijfer= this->begin;
    //Print NULL als cijfervak leeg is, anders print cijfervak en ga door naar volgende
    while(cijfer != nullptr) {
        std::cout<<cijfer->data<<"<==>";
        cijfer = cijfer->next;
    }
    if(cijfer == nullptr){
        std::cout<<"NULL";
    }
    std::cout << std::endl;
}

//Lees invoer per karakter
char leesKarakter(){
    while(true){
        std::string trash = "";
        char karakter = std::cin.get();
        getline(std::cin,trash);
        std::cin.clear();

        if(!trash.empty()){
            std::cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << std::endl;
            continue;
        }
        return karakter;
    }
}

//Infoblok
void infoBlok(){
    std::cout << std::endl << std::endl;
    for(int i = 1; i < 45; i++){
        std::cout << '*';
    } 
    std::cout << std::endl;
    std::cout << "* Yvo Hu studentnummer: 2962802            *" << std::endl;
    std::cout << "* Wietske Holwerda studentnummer: 2838192  *" << std::endl;
    std::cout << "* Studiejaar: 2020                         *" << std::endl;
    std::cout << "* Studierichting: Informatica              *" << std::endl;
    std::cout << "* Dit programma manipuleert grote getallen *" << std::endl;
    std::cout << "* met behulp van linked lists              *" << std::endl;
    for(int i = 1 ; i < 45; i++){
        std::cout << '*';
    }
    std::cout << std::endl;
}