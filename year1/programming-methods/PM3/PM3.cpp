#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

//Programma van Yvo Hu en Wietske Holwerda
//Dit programma maakt een nonogram
//Laatste update 08-11-2020
//Gebruikte compiler GNU g++

using namespace std;

class nonogram{
    public:
        nonogram(unsigned int hoogte = nonogram::grootte[0], unsigned int breedte = nonogram::grootte[1]){
            grootte[0] = hoogte;
            grootte[1] = breedte;
            cursorX = 0;
            cursorY = 0;
            cursor = 0;//Haken om cursor
            rpercent = 50;//Percentage true waardes nonoArray bij vulRandom
        };
        static unsigned int grootte[2];;//hoogte, breedte

        static int hoofdMenu(nonogram&);
        static int stelParameterMenu(nonogram&);
        int stelGrootte();
        void stelSymbool();
        void stelPercentage();
        void stelCursor();

        int beweegCursor(char);
        void toggle();
        void updateBeschrijvingenf(unsigned int, unsigned int, unsigned int[][50], unsigned int[][50]);
        void updateBeschrijvingen(unsigned int); // Update beschrijving o.b.v. huidige nonogram
        void printCheck(unsigned int, unsigned int&); //Print Checkmark

        void maakSchoon();
        void drukAf();
        void vulRandom();
        void leesFile();
        void schrijfFile();

    private:
        static const unsigned int MAX = 50;//Puzzel limiet
        char symbool[2] = {'*',' '};//wit,zwart

        unsigned int rpercent;//Random percentage

        unsigned int cursor;//Symbool cursor
        unsigned int cursorX;//X coordinaat
        unsigned int cursorY;//Y coordinaat

        static bool nonoArray[MAX][MAX];//Nonogram waarden
        //Nonogram beschrijving
        unsigned int rijen[MAX][MAX] = {};
        unsigned int kolommen[MAX][MAX] = {};
        //Huidige beschrijving
        static unsigned int rijen2[MAX][MAX];
        static unsigned int kolommen2[MAX][MAX];
        //Checkmark
        static unsigned int checkArray[MAX][MAX];
};
//Forward declarations
void infoBlok();
string invoerf(ifstream&);
string uitvoerf();
char leesKarakter();

    //Polymorfisme
int leesGetal(unsigned int&, char&);
int leesGetal(unsigned int&, char&, ifstream&);

//Static initialisation
unsigned int nonogram::grootte[2] = {10,10};
unsigned int nonogram::rijen2[nonogram::MAX][nonogram::MAX] = {};
unsigned int nonogram::kolommen2[nonogram::MAX][nonogram::MAX] = {};
unsigned int nonogram::checkArray[nonogram::MAX][nonogram::MAX] = {};
bool nonogram::nonoArray[nonogram::MAX][nonogram::MAX] = {};

int main(){
    nonogram *nono = new nonogram(nonogram::grootte[0],nonogram::grootte[1]);
    nono->vulRandom();
    nono->updateBeschrijvingen(1);

    while(true){
        if(nonogram::hoofdMenu(*nono)){//Nieuw object als grootte van puzzel veranderd
            nono = new nonogram(nonogram::grootte[0], nonogram::grootte[1]);
            nono->updateBeschrijvingen(1);
            continue;
        }else{ //Stop programma
            break;
        }
    }
    return 0;
}
//Parameter menu
int nonogram::stelParameterMenu(nonogram& nono){
    while(true){
        nono.drukAf();
        cout << "Kies uit 1 van de volgende opties: (W,A,S,D), t(O)ggle, (U)pdate, (G)rootte, s(Y)mbool, (C)ursor, r(P)ercentage, (T)erug" << endl;
        char keuze = leesKarakter();
        switch(keuze){
        case('G'):
        case('g'):
            if(nono.stelGrootte()){
                return 1;
            }else{
                continue;
            }
        case('Y'):
        case('y'):
            nono.stelSymbool();
            continue;
        case('C'):
        case('c'):
            nono.stelCursor();
            continue;
        case('P'):
        case('p'):
            nono.stelPercentage();
            continue;
        case('O'):
        case('o'):
            nono.toggle();
            continue;
        case('U'):
        case('u'):
            nono.updateBeschrijvingen(1);
            continue;
        case('T'):
        case('t'):
            return 0;
        default:
            if(!nono.beweegCursor(keuze)){
                cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            }
            continue;
        }
    }
}
//Stel dimensies van nonogram in
int nonogram::stelGrootte(){
    char letter;
    unsigned int sum = 0;
    cout << "De huidige dimensies zijn:"<< endl;
    cout << "hoogte: " << this->grootte[0] << ", breedte:" << this->grootte[1] <<endl; 

    while(true){
        cout << "Wat moet de hoogte van de puzzel zijn?" << endl;
        if(leesGetal(sum,letter) == -1){
            cout << "Ongeldige invoer, typ een getal tussen de 1 en 50 in voor uw keuze." << endl;
            continue;
        };

        if(sum < 1 || sum > 50){
            cout << "Ongeldige invoer, typ een getal tussen de 1 en 50 in voor uw keuze." << endl;
            continue;
        }
        this->grootte[0] = sum;
        break;
    }
    while(true){
        cout << "Wat moet de breedte van de puzzel zijn?" << endl;
        if(leesGetal(sum,letter) == -1){
            cout << "Ongeldige invoer, typ een getal tussen de 1 en 50 in voor uw keuze." << endl;
            continue;
        };

        if(sum < 1 || sum > 50){
            cout << "Ongeldige invoer, typ een getal tussen de 1 en 50 in voor uw keuze." << endl;
            continue;
        }
        this->grootte[1] = sum;
        break;
    }
    this->cursorX = this->cursorY = 0; // Reset cursor positie
    cout << "Uw wijziging is opgeslagen" << endl;
    return 1;
}
//Stel symbolen voor nonogram in
void nonogram::stelSymbool(){
    cout << "De huidige keuze is Wit:" << nonogram::symbool[0] << ", " << "Zwart:" << nonogram::symbool[1] <<endl; 
    char keuze;

    while(true){
        cout << "Welk symbool moeten de witte pixels voorstellen?" << endl;
        keuze = leesKarakter();
        nonogram::symbool[0] = keuze;
        break;
    }
    while(true){
        cout << "Welk symbool moeten de zwarte pixels voorstellen?" << endl;
        keuze = leesKarakter();
        if(nonogram::symbool[0]==keuze){
            cout << "De zwarte en witte pixels hebben hetzelfde symbool" << endl;
            cout <<  "Typ een nieuw karakter in voor de zwarte pixels" << endl;
            continue;
        }
        nonogram::symbool[1] = keuze;
        break;
    }
    
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}
//Stel symbolen naast cursor in
void nonogram::stelCursor(){

    switch(nonogram::cursor){
    case(0):
        cout << "De huidige keuze is brackets[0]" <<endl; 
        break;
    case(1):
        cout << "De huidige keuze is braces{1}" <<endl; 
        break;
    case(2):
        cout << "De huidige keuze is parentheses(2)" <<endl; 
        break;
    }
    char keuze;

    while(true){
        cout << "Welke symbolen moeten er naast het punt staan bij het verplaatsen van de cursor?" << endl;
        cout << "Brackets[0], Braces{1}, Parentheses(2)" << endl;
        keuze = leesKarakter();
        
        if(((keuze - '0') >=0) && ((keuze - '0') <=2)){
            nonogram::cursor = keuze - '0';
            break;
        }else{
            cout << "Ongeldige invoer, typ een geldig karakter(0,1,2) in voor uw keuze." << endl;
            continue;
        }
    }
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}
//Stel randompercentage in
void nonogram::stelPercentage(){
    char letter;
    unsigned int sum = 0;
    cout << "Het huidige randompercentage is " << nonogram::rpercent << " procent" <<endl; 
    while(true){
        cout << "Welk random percentage(0-100) wilt u instellen?" << endl;

        if(leesGetal(sum,letter) == -1){
            cout << "Ongeldige invoer, typ een getal tussen de 0 en 100 in voor uw keuze." << endl;
            continue;
        };

        if(sum < 0 || sum > 100){
            cout << "Ongeldige invoer, typ een getal tussen de 0 en 100 in voor uw keuze." << endl;
            continue;
        }else{
            rpercent = sum;
            break;
        }
    }
    cout << "Uw wijziging is opgeslagen" << endl;   
    return;
}
//Hoofdmenu
int nonogram::hoofdMenu(nonogram& nono){
    while(true){
        nono.drukAf();
        cout << "Kies uit 1 van de volgende opties: (W,A,S,D), t(O)ggle, (U)pdate, (F)ile, s(C)hoon, (R)andom, (P)arameters, s(T)oppen" << endl;
        char keuze = leesKarakter();

        switch(keuze){
        case('F'):
        case('f'):
            while(true){
                cout << "Kies uit 1 van de volgende opties: (L)ezen, (S)chrijven, (T)erug" << endl;
                keuze = leesKarakter();
                switch(keuze){
                case('L'):
                case('l'):
                    nono.leesFile();
                    break;
                case('S'):
                case('s'):
                    nono.schrijfFile();
                    break;
                case('T'):
                case('t'):
                    break;
                default:
                    cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
                    continue;
                }
                break;
            }
            continue;
        case('C'):
        case('c'):
            nono.maakSchoon();
            continue;
        case('R'):
        case('r'):
            nono.vulRandom();
            continue;
        case('P'):
        case('p'):
            if(nonogram::stelParameterMenu(nono)){
                return 1;
            }else{
                continue;
            }
        case('O'):
        case('o'):
            nono.toggle();
            continue;
        case('U'):
        case('u'):
            nono.updateBeschrijvingen(1);
            continue;
        case('T'):
        case('t'):
            return 0;
        default:
            if(!nono.beweegCursor(keuze)){
                cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            }
            continue;
        }
    }
}
//Zero alle waarden in de array behorend tot de huidige dimensies van de nonogram
void nonogram::maakSchoon(){
    for (unsigned int i = 0; i < this->grootte[0]; i++ ) {
        for (unsigned int j = 0; j < this->grootte[1]; j++ ) {
            this->nonoArray[i][j] = false;
        }
    }
} 
//Print alles
void nonogram::drukAf(){
    infoBlok();
    this->updateBeschrijvingen(0);//Houd constant bij of de huidige nonogram voldoet aan de beschrijvingen

    for(unsigned int i = 0; i < this->grootte[0]; i++ ){
        cout << " ";
        for(unsigned int j = 0; j < this->grootte[1]; j++ ){
            if(i == this->cursorY && j == this->cursorX){
                switch(nonogram::cursor){
                case(0):
                    cout << "[";
                    break;
                case(1):
                    cout << "{";
                    break;
                case(2):
                    cout << "(";
                    break;
                }
            }else{
                cout << " ";
            }
            if(this->nonoArray[i][j]){
                cout << this->symbool[0];
            }else{
                cout << this->symbool[1];
            }
            if(i == this->cursorY && j == this->cursorX){
                switch(this->cursor){
                case(0):
                    cout << "]";
                    break;
                case(1):
                    cout << "}";
                    break;
                case(2):
                    cout << ")";
                    break;
                }
            }else{
                cout << " ";
            }
        }
        unsigned int j = 0;
        cout << "|";
        //Print checkmark rijen
        nonogram::printCheck(0,i);
        //Print beschrijvingen; Als waarde 0 is dan stop
        while(nonogram::rijen2[i][j]){
            cout << nonogram::rijen2[i][j]  << " ";
            j++;
        }
        cout << endl;
    }
    for(unsigned int i = 0; i < this->grootte[1];i++){
        cout << "---";
    }
    cout << endl << " ";
    //Print checkmark kolommen
    for(unsigned int i = 0; i < this->grootte[1];i++){
        nonogram::printCheck(1,i);
    }
    cout << endl;

    //Print en check grootte van beschrijvingen kolommen zodat het niet onnodig veel print
    unsigned int flag = this->grootte[1];
    for(unsigned int j = 0; (flag > 0); j++){
        cout << " ";
        flag = this->grootte[1];
        for(unsigned int i = 0;i < this->grootte[1];i++){
            if(!(nonogram::kolommen2[i][j])){
                cout << "   ";//Print spaties voor geen waardes in beschrijving
                flag--;
                continue;
            }
            if(nonogram::kolommen2[i][j] > 9){//Print geen spatie na waarde voor getallen boven 9; breekt bij > 99 :C
                cout << nonogram::kolommen2[i][j] << " ";
            }else{
                cout << " " << nonogram::kolommen2[i][j] << " ";
            }
        }
        cout << endl;
    }
}
//Bereken seed randomgetal
unsigned int randomgetal ( ) {
    static unsigned int getal = 42;
    getal = ( 221 * getal + 1 ) % 1000;
    return getal;
}
//Vul de nonogram pseudorandom in op basis van randomgetal seed
void nonogram::vulRandom(){
    bool flag;
    unsigned int randomGrens = nonogram::rpercent*10;

    for (unsigned int i = 0; i < this->grootte[0]; i++ ){
        for (unsigned int j = 0; j < this->grootte[1]; j++ ) {
            if (randomgetal() < randomGrens) {
                flag = true;
            }
            else {
                flag = false;
            }
            this->nonoArray[i][j] = flag;
        }
    }
}
//Schrijf huidige beschrijving naar uitvoerbestand
void nonogram::schrijfFile(){
    string uitvoernaam = uitvoerf();
    ofstream uitvoer;
    uitvoer.open(uitvoernaam,ios::out);
    for(unsigned int i = 0; i < 2; i++){
        uitvoer << nonogram::grootte[i] << ' ';
    }
    for(unsigned int i = 0; i < nonogram::grootte[0]; i++){
        unsigned int j = 0;
        uitvoer << '\n';
        //Schrijf de eerste waardes > 0 naar file
        while(nonogram::rijen[i][j]){
            uitvoer << nonogram::rijen[i][j] << ' ';
            j++;
            continue;
        }
        uitvoer << 0;
    }

    for(unsigned int i = 0; i < nonogram::grootte[1]; i++){
        unsigned int j = 0;
        uitvoer << '\n';
        //Schrijf de eerste waardes > 0 naar file
        while(nonogram::kolommen[i][j]){
            uitvoer << nonogram::kolommen[i][j] << ' ';
            j++;
            continue;
        }
        uitvoer << 0;
    }
    uitvoer.close();
    cin.ignore();
}
//Lees bestand en neem beschrijving over
void nonogram::leesFile(){
    //Reset
    nonogram::maakSchoon();
    nonogram::updateBeschrijvingen(1);

    ifstream invoer;
    string invoernaam = invoerf(invoer);
    invoer.open(invoernaam, ios::in);
    char letter;
    unsigned int sum;

    this->grootte[0] = leesGetal(sum,letter,invoer);
    this->grootte[1] = leesGetal(sum,letter,invoer);
    invoer.ignore();//Negeer eerste 0

    //Lees alle karakters per rij in en sla op in beschrijvingen array
    for(unsigned int i = 0; i < grootte[0];i++){
        unsigned int j = 0;
        while(!invoer.eof()){
            nonogram::rijen2[i][j] = leesGetal(sum,letter,invoer);
            j++;
            if(letter == '\n'){
                break;
            }
        }
    }
    for(unsigned int i = 0; i < grootte[1];i++){
        unsigned int j = 0;
        while(!invoer.eof()){
            nonogram::kolommen2[i][j] = leesGetal(sum,letter,invoer);
            j++;
            if(letter == '\n'){
                break;
            }
        }
    }
    cin.ignore();
    this->cursorX = this->cursorY = 0;// Reset cursor positie
}
//leesGetal voor files; geen input sanitization :C
int leesGetal(unsigned int& sum, char& letter, ifstream& invoer){
    sum = 0;
    letter = invoer.get();
    while(isdigit(letter)){
        if(letter == '\n'){
            break;
        }
        //Indien meer iteraties, vermenigvuldig met 10
        sum *= sum > 0 ? 10 : sum;
        sum += (letter - '0');
        letter = invoer.get();
    }
    return sum;
}
//leesGetal voor iostream
int leesGetal(unsigned int& sum, char& letter){
    bool flag = false;
    sum = 0;
    letter = cin.get();
    while(true){
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
    }
    //Check of er een getal ingevuld is
    if(flag){
        return sum;
    }else{
        return -1;
    }
}

//Valideer iostream input maximaal 1 karakter per enter
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
//Bepaal invoernaam
string invoerf(ifstream& invoer){
    string invoernaam;

    while(true){
        cout << "Wat is de naam van het invoerbestand?" << endl;
        cin >> invoernaam;
        //Open invoerbestand
        invoer.open(invoernaam,ios::in);
        //Error handling
        if(invoer.fail()){
            cerr << "Het openen van " << invoernaam << " is mislukt. "
            "Probeer het nogmaals" << endl;
        }
        else{
            //Sluit invoerbestand 
            invoer.close();
            break;
        }
    }
    return invoernaam;
}
//Bepaal uitvoernaam
string uitvoerf(){
    string uitvoernaam;

    cout << "Wat is de naam van het uitvoerbestand?" << endl;
    cin >> uitvoernaam;

    return uitvoernaam;
}

//Beweeg de cursor van de nonogram met behulp van iostream input
int nonogram::beweegCursor(char keuze){
    switch(keuze){
    case('W'):
    case('w'):
        this->cursorY > 0 ? this->cursorY-- : 0;
        break;
    case('A'):
    case('a'):
        this->cursorX > 0 ? this->cursorX-- : 0;
        break;
    case('S'):
    case('s'):
        this->cursorY < (this->grootte[0]-1)? this->cursorY++ : 0; // Limiet hoogte
        break;
    case('D'):
    case('d'):
        this->cursorX < (this->grootte[1]-1)? this->cursorX++ : 0; // Limiet breedte
    default:
        cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
        return 0;
    }
    return 1;
}
//Keer waarde om in nonoArray 
void nonogram::toggle(){
    this->nonoArray[this->cursorY][this->cursorX] = this->nonoArray[this->cursorY][this->cursorX] ? 0 : 1;
    return;
}
//Print checkmark
void nonogram::printCheck(unsigned int j, unsigned int& i){
    cout << "[";
    if(nonogram::checkArray[j][i]){
        cout << "V";
    }else{
        cout << " ";
    }
    cout << "]";
    return;
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
    cout << "* Dit programma maakt een nonogram        *" << endl;
    for(unsigned int i = 1; i<44;i++){
        cout << '*';
    }
    cout << endl;
}
//Update beschrijvingen van huidige nonogram en vergelijk met de huidige geprinte beschrijvingen
void nonogram::updateBeschrijvingenf(unsigned int dimensie, unsigned int flag, unsigned int beschrijvingenArray[][50], unsigned int beschrijvingenArray2[][50]){ //dimensie 0 = hoogte, 1 = breedte;
    for(unsigned int i = 0; i < this->grootte[dimensie];i++){
        unsigned int sum = 0;
        unsigned int index = 0;
        bool flag2 = true;
        for(unsigned int j = 0; j <= this->grootte[!dimensie]; j++){
            //Reset
            beschrijvingenArray[i][j] = 0;
            if(flag){
                    beschrijvingenArray2[i][j] = 0;
                }
            //Bereken opeenvolgende positieve waarden
            if(dimensie && this->nonoArray[j][i]){//Kolommen
                sum++;
                continue;
            }else if(!dimensie && this->nonoArray[i][j]){//Rijen
                sum++;
                continue;
            }else{
                //Vul in beschrijvingen array alleen waardes > 0
                if(sum){
                    //Update beschrijvingen
                    if(flag){
                        beschrijvingenArray2[i][index] = sum;
                    }
                    beschrijvingenArray[i][index] = sum;
                    //Update checkArray waarde als nonogram niet overeenkomt met beschrijving
                    if(!(beschrijvingenArray[i][index] ==  beschrijvingenArray2[i][index])){
                        flag2 = false;
                    }
                    index++;
                    sum = 0;
                }
            }
        }
        //Evalueer nog een keer na laatste waarde van nonogram::kolommen
        if(beschrijvingenArray[i][index] !=  beschrijvingenArray2[i][index]){
            flag2 = false;
        }
        checkArray[dimensie][i] = flag2;
    }
}
//Implementeer functie voor rijen en kolommen
void nonogram::updateBeschrijvingen(unsigned int flag){
    nonogram::updateBeschrijvingenf(0,flag,rijen,rijen2);//Update beschrijvingen rijen
    nonogram::updateBeschrijvingenf(1,flag,kolommen, kolommen2);//Update beschrijvingen kolommen
    return;
}
