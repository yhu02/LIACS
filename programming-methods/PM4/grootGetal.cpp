#include "grootGetal.h"
#include "stack.h"
#include <iostream>
#include <string>
static int idCount = 0;
static int k = 2;//2^64 past in minimaal 19 en maximaal 20 karakters dus k is maximaal 19 anders undefined behavior
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
//leesGetal voor iostream
void grootGetal::leesGetal(){
    this->vernietig();
    //Bereken grootte van het getal zodat de cijfers goed passen in de cijfervakken
    grootGetal temp;
    cijfervak* tempcijfervak;
    unsigned long long sum = 0;
    unsigned int grootte = k;
    unsigned int count = 0;
    unsigned int totalcount = 0;
    bool flag = false;
    char letter = std::cin.get();
    
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
            temp.voegachter(sum);
            totalcount++;
            sum = 0;
            break;
        }
        count = 0;
    }//ceil aantal cijfers / grootte van cijfervak = aantal gebruikte cijfervakken
    this->total += totalcount%grootte == 0 ? totalcount/grootte : (totalcount/grootte) + 1;
    if(flag){
        tempcijfervak = temp.begin;
        
        while(tempcijfervak->next != nullptr){
            tempcijfervak = tempcijfervak->next;
        }
        unsigned int mult = 0;
        int num = tempcijfervak->data;

        while(true){
            if(tempcijfervak->prev != nullptr){
                for(unsigned int i = 0; i < grootte; i++){
                    if(tempcijfervak->prev != nullptr){
                        num = tempcijfervak->data;
                        tempcijfervak = tempcijfervak->prev;
                        for(unsigned int i = 0; i < mult; i++){
                            num*= 10;
                        }
                        mult++;
                        sum += num;
                    }
                }
                if(tempcijfervak->prev == nullptr && mult != grootte){
                    num = tempcijfervak->data;
                    for(unsigned int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }else if(tempcijfervak->prev == nullptr && mult == grootte){
                    this->voegvoor(sum);
                    mult = 0;
                    sum = 0;
                    num = tempcijfervak->data;
                    for(unsigned int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }

                this->voegvoor(sum);
                mult = 0;
                sum = 0;
            }
            if(totalcount == 1){//Case als slechts 1 cijfer
                this->voegvoor(tempcijfervak->data);
            }
            if(tempcijfervak->prev == nullptr){//laatste cijfervak bereikt herhaal nog 1 keer
                break;
            }
        }
    }
    //Check of er een getal ingevuld is
    if(!flag){
        std::cout << "Ongeldige invoer, typ een getal tussen de -2^63 en 2^63 in" << std::endl;
        return;
    }
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
        newcijfervak->prev = nullptr;
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
//Maak een k-cijferige getallenreeks van een enkelcijferige getallenreeks
void grootGetal::maakList(unsigned long long i){
    grootGetal* temp = new grootGetal;
    grootGetal* temp2 = new grootGetal;
    cijfervak* tempcijfervak;
    unsigned long long sum = 0;
    unsigned int grootte = k;
    temp->splitGetal(i);
    temp2->total = temp->total%grootte == 0 ? temp->total/grootte : (temp->total/grootte) + 1;

    if(true){
        tempcijfervak = temp->begin;

        while(tempcijfervak->next != nullptr){
            tempcijfervak = tempcijfervak->next;
        }
        
        unsigned int mult = 0;
        unsigned long long num = tempcijfervak->data;

        while(true){
            if(tempcijfervak->prev != nullptr){
                for(unsigned int i = 0; i < grootte; i++){
                    if(tempcijfervak->prev != nullptr){
                        tempcijfervak->next = nullptr;//Cleanup
                        num = tempcijfervak->data;
                        tempcijfervak = tempcijfervak->prev;
                        for(unsigned int i = 0; i < mult; i++){
                            num*= 10;
                        }
                        mult++;
                        sum += num;
                    }
                }
                if(tempcijfervak->prev == nullptr && mult != grootte){
                    num = tempcijfervak->data;
                    for(unsigned int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum+= num;
                }else if(tempcijfervak->prev == nullptr && mult == grootte){
                    temp2->voegvoor(sum);
                    mult = 0;
                    sum = 0;
                    num = tempcijfervak->data;
                    for(unsigned int i = 0; i < mult; i++){
                        num*= 10;
                    }
                    sum += num;
                }
                temp2->voegvoor(sum);
                mult = 0;
                sum = 0;
            }
            if(temp->total == 1){//Case als slechts 1 cijfer
                temp2->voegvoor(num);
            }
            if(tempcijfervak->prev == nullptr){//laatste cijfervak bereikt 
                break;
            }
        }
    }
    temp2->displayList();
    this->begin = temp2->begin;
    this->end = temp2->end;
    this->total = temp2->total;
    this->displayList();
    return;
}
//Splits een getal in een enkelcijferige getallenreeks
void grootGetal::splitGetal(long long i){
    //Recursie voor getallen groter dan 9
    i > 9 ? splitGetal(i / 10) : void();
    //Voeg getal toe aan getallenreeks
    long long getal = i % 10;
    this->voegachter(getal);
    this->total++;
}
//Bereken n-ste fibonacci getal; Na ongeveer n = 40 gaat het lang duren
long long grootGetal::fibonacci(unsigned int n){
    if  ((n == 0) || (n==1)){
        return n;
    }else{
        return (fibonacci(n-1) + fibonacci(n-2));
    }
}
//Vernietig huidige getallenreeks
void grootGetal::vernietig(){
    this->total = 0;
    cijfervak* temp = this->begin;
    if(temp == nullptr){
        return;
    }
    while(temp->next != nullptr){
        temp->prev = nullptr;
        temp = temp->next;
    }
    temp = nullptr;
    this->begin = nullptr;
    this->end = nullptr;
}
//Kopieer getallenreeks B naar huidige getallenreeks
void grootGetal::kopieer(grootGetal* B){
    cijfervak* tempA = new cijfervak;
    cijfervak* temp = new cijfervak;
    cijfervak* tempB;
    int num;

    //Reset
    this->vernietig();
    //Doe niets als de getallenreeks van B leeg is
    if(B->begin != nullptr) {
        tempB = B->begin;
    }else{
        return;
    }
    //Stel begin nieuwe getallenreeks
    this->begin = tempA;
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
}
//Voeg een cijfervak toe aan de voorkant van de list
void grootGetal::voegvoor(unsigned long long new_data){
    struct cijfervak* newcijfervak = new cijfervak;
    struct cijfervak* first = this->begin; //Stel begin cijfervak naar first
    newcijfervak->data = new_data;

    //Maak nieuw cijfervak als de getallenreeks leeg is
    if(this->begin == nullptr) {
        newcijfervak->prev = nullptr;
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
//Maak een k-cijferige fibonacci getallenreeks van een enkelcijferige fibonacci getallenreeks
void grootGetal::fibonaccif(){
    //Bereken grootte van het getal zodat de cijfers goed passen in de cijfervakken
    int num = 0;
    char letter;
    unsigned int count = 0;
    bool flag = false;
    unsigned int grootte = k;
    this->vernietig();
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
            num *= num > 0 ? 10 : num;
            num += (letter - '0');
            letter = std::cin.get();
            flag = true;
            count++;
            break;
        }
    }
    this->maakList(fibonacci(num));
    return;
}
//Vermenigvuldig getallenreeks a en b en sla deze op in de huidige getallenreeks
void grootGetal::vermenigvuldig(grootGetal* a, grootGetal* b){
    grootGetal* temp = new grootGetal;
    cijfervak* cijfervakA = a->begin;
    cijfervak* cijfervakB = b->begin;
    unsigned long long sum = 0;
    unsigned long long numA = 0;
    unsigned long long numB = 0;
    unsigned int grootte = k;
    unsigned int multA = 0;
    unsigned int multB = 0;
    //Reset
    this->vernietig();
    
    if(a->begin == nullptr || b->begin == nullptr || cijfervakA->data == 0 || cijfervakB->data == 0){
        this->voegvoor(0);
        return;
    }
    while(cijfervakA->next != nullptr){//Check of nullptr pointer nog niet is bereikt en doorkruis de list
        cijfervakA = cijfervakA->next;
    }
    while(cijfervakA->prev != nullptr){ 
        numA = cijfervakA->data;
        for(int i = 0; i < multA; i++){
            numA *= 10;
        }
        for(unsigned int i = 0; i < grootte;i++){
            multA++;
        }
        if(b->begin != nullptr){
            cijfervakB = b->begin;
        }
        while(cijfervakB->next != nullptr){//Check of nullptr pointer nog niet is bereikt en doorkruis de list
            cijfervakB = cijfervakB->next;
        }
        while(cijfervakB->prev != nullptr){
            numB = cijfervakB->data; 
            for(int i = 0; i < multB; i++){
                numB *= 10;
            }
            for(unsigned int i = 0; i < grootte;i++){
                multB++;
            }
            sum+= numB * numA;
            cijfervakB = cijfervakB->prev;

        }
        numB = cijfervakB->data;
        for(int i = 0; i < multB; i++){
            numB *= 10;
        }
        sum += numB * numA;

        if(cijfervakA->prev != nullptr){
            cijfervakA= cijfervakA->prev;
        }
        if(sum >= INT64_MAX || sum < 0){
            std::cout << std::endl;
            std::cout << "Error: Operatie kan niet worden verwerkt";
            std::cout << std::endl;
            std::cout << "Vermenigvuldiging van twee getallen geeft een getal groter dan 2^63";
            std::cout << std::endl;
            return;
        }
        temp->maakList(sum);
        this->add(temp,this);
        multB = 0;
    }
    numA = cijfervakA->data;
    for(int i = 0; i < multA; i++){
        numA *= 10;
    }
    if(b->begin != nullptr){
        cijfervakB = b->begin;
    }
    while(cijfervakB->next != nullptr){//Check of nullptr pointer nog niet is bereikt en doorkruis de list
        cijfervakB = cijfervakB->next; 
    }
    while(cijfervakB->prev != nullptr){
        numB = cijfervakB->data; 
        for(int i = 0; i < multB; i++){
            numB *= 10;
        }
        for(unsigned int i = 0; i < grootte;i++){
            multB++;
        }
        sum+= numB * numA;
        cijfervakB = cijfervakB->prev;
    }
    numB = cijfervakB->data;
    for(int i = 0; i < multB; i++){
        numB *= 10;
    }
    sum += numB * numA;

    if(cijfervakA->prev != nullptr){
        cijfervakA= cijfervakA->prev;
    }
    if(sum >= INT64_MAX || sum < 0){
        std::cout << std::endl;
        std::cout << "Error: Operatie kan niet worden verwerkt";
        std::cout << std::endl;
        std::cout << "Vermenigvuldiging van twee getallen geeft een getal groter dan 2^63";
        std::cout << std::endl;
        return;
    }
    temp->maakList(sum);
    this->add(temp,this);
    return;
}
//Voeg getallenreeksen a en b bij elkaar en sla deze op in de huidige getallenreeks
void grootGetal::add(grootGetal* a, grootGetal* b){;
    cijfervak* cijfervakA = a->begin;
    cijfervak* cijfervakB = b->begin;
    unsigned long long sum = 0;
    unsigned long long control = 1;
    bool endA = false;
    bool endB = false;
    bool carry = false;
    int total = a->total >= b->total ? a->total : b->total;

    //Reset
    this->vernietig();
    //Check voor carry
    for(int i = 0;i < k; i++){
        control *= 10;
    }

    if(a->begin == nullptr){
        cijfervakA = new cijfervak;
        cijfervakA->data = 0;
    }
    while(cijfervakA->next != nullptr){//Check of nullptr pointer nog niet is bereikt en doorkruis de list
        cijfervakA = cijfervakA->next;
    }
    if(b->begin == nullptr){
        cijfervakB = new cijfervak;
        cijfervakB->data = 0;
    }
    while(cijfervakB->next != nullptr){
        cijfervakB = cijfervakB->next;  
    }

    while(total > 0){
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
        //Bereken huidige cijfervak en verwerk carry naar volgende cijfervak
        if(sum >= control){
            carry = true;
            sum = sum - control;
            this->voegvoor(sum);
            sum = 1;
            total--;
            continue;
        }else{
            this->voegvoor(sum);
            sum = 0;
            total--;
        }

    }
    return;
}
//Submenu
void grootGetal::submenu(grootGetal* getal2, grootGetal* getal3, stack<grootGetal> &stapel){
    while(true){
        grootGetal* temp = new grootGetal;
        for(int j = 0; j < 44; j++){
            std::cout << "*";
        }
        std::cout << std::endl;
        //Print 1 van de drie getallen 
        if(this->id == 0){
            this->drukAf(getal2, getal3);
        }
        else if(this->id == 1){
            getal3->drukAf(this, getal2);
        }
        else if(this->id == 2){
            getal2->drukAf(getal3, this);
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
            this->kopieer(stapel.peek());
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