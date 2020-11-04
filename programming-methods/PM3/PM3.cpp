#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;


class nonogram{
    public:
        nonogram(){
        };

        static void stelParameterMenu();
        static void stelGrootte();
        static void stelSymbool();
        static void stelPercentage();
        static void stelCursor();
        static void beweegCursor(char);
        static void toggle();

        static void hoofdMenu();

        static void maakSchoon();
        static void drukAf();
        static void vulRandom();
        static void leesFile();

    private:
    
        static const int MAX = 50;
        static int grootte[2];//hoogte,breedte
        static char symbool[2];//wit,zwart
        static int cursor;
        static int rpercent;

        static unsigned int cursorX;
        static unsigned int cursorY;

        static bool nonoArray[MAX][MAX];
        static int rijen[MAX][MAX];
        static int kolommen[MAX][MAX];

        static int rijenCheck[MAX][MAX];
        static int kolommenCheck[MAX][MAX];
};
//Forward declarations
string invoerf(ifstream&);
//Polymorfisme
int leesGetal(int&, char&);
int leesGetal(int&, char&, ifstream&);

char leesKarakter();
//static initialisation
int nonogram::grootte[2] = {10,10};
char nonogram::symbool[2] = {'*',' '};
int nonogram::cursor = 0;
int nonogram::rpercent = 50;

unsigned int nonogram::cursorX = 0;
unsigned int nonogram::cursorY = 0;

bool nonogram::nonoArray[nonogram::MAX][nonogram::MAX] = {};
int nonogram::rijen[nonogram::MAX][nonogram::MAX] = {};
int nonogram::kolommen[nonogram::MAX][nonogram::MAX] = {};


int main(){
    nonogram::vulRandom();
    nonogram::hoofdMenu();
    system("pause");
    return 0;
}
void nonogram::stelParameterMenu(){
    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(true){
        nonogram::drukAf();
        cout << "Kies uit 1 van de volgende opties: (W,A,S,D), t(O)ggle, (G)rootte, s(Y)mbool, (C)ursor, r(P)ercentage, (T)erug" << endl;
        char keuze = leesKarakter();
        switch(keuze){
            case('G'):
            case('g'):
                nonogram::stelGrootte();
                continue;
            case('Y'):
            case('y'):
                nonogram::stelSymbool();
                continue;
            case('C'):
            case('c'):
                nonogram::stelCursor();
                continue;
            case('P'):
            case('p'):
                nonogram::stelPercentage();
                continue;
            case('O'):
            case('o'):
                nonogram::toggle();
                continue;
            case('T'):
            case('t'):
                return;
            default:
                nonogram::beweegCursor(keuze);
                continue;
        }
    }
}
void nonogram::stelGrootte(){
    char letter;
    int sum = 0;
    cout << "De huidige dimensies zijn:"<< endl;
    cout << "hoogte: " << nonogram::grootte[0] << ", breedte:" << nonogram::grootte[1] <<endl; 

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
        nonogram::grootte[0] = sum;
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
        nonogram::grootte[1] = sum;
        break;
    }
    
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}

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
void nonogram::stelCursor(){

    switch(nonogram::cursor){
        case(0):
            cout << "De huidige keuze is brackets[](0)" <<endl; 
            break;
        case(1):
            cout << "De huidige keuze is braces{}(1)" <<endl; 
            break;
        case(2):
            cout << "De huidige keuze is parentheses()(2)" <<endl; 
            break;
    }
    char keuze;

    while(true){
        cout << "Welke symbolen moeten er naast het punt staan bij het verplaatsen van de cursor?" << endl;
        cout << "Brackets[](0), Braces{}(1), Parentheses(2)" << endl;
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

void nonogram::stelPercentage(){
    char letter;
    int sum = 0;
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

void nonogram::hoofdMenu(){

    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(true){
        nonogram::drukAf();
        cout << "Kies uit 1 van de volgende opties: (W,A,S,D), t(O)ggle, (F)ile, s(C)hoon, (R)andom, (P)arameters, s(T)oppen" << endl;
        char keuze = leesKarakter();

        switch(keuze){
            case('F'):
            case('f'):
                nonogram::leesFile();
                continue;
            case('C'):
            case('c'):
                nonogram::maakSchoon();
                continue;
            case('R'):
            case('r'):
                nonogram::vulRandom();
                continue;
            case('P'):
            case('p'):
                nonogram::stelParameterMenu();
                continue;
            case('O'):
            case('o'):
                nonogram::toggle();
                continue;
            case('T'):
            case('t'):
                return;
            default:
                nonogram::beweegCursor(keuze);
                continue;
        }
    }
}

void nonogram::maakSchoon(){
    for (int i = 0; i < nonogram::grootte[0]; i++ ) {
        for (int j = 0; j < nonogram::grootte[1]; j++ ) {
            nonogram::nonoArray[i][j] = false;
        }//for j
    }//for i
} 

void nonogram::drukAf(){
    for (int i = 0; i < nonogram::grootte[0]; i++ ) {
        int j = 0;
        cout << " ";
        for (; j < nonogram::grootte[1]; j++ ) {
            if(i == nonogram::cursorY && j == nonogram::cursorX){
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
            if (nonogram::nonoArray[i][j]){
                cout << nonogram::symbool[0];
            }else{
                cout << nonogram::symbool[1];
            }
            if(i == nonogram::cursorY && j == nonogram::cursorX){
                switch(nonogram::cursor){
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
        }//for j
        //Check of waarde niet 0 is -> einde array
        j = 0;
        while(nonogram::rijen[i][j]){
            cout << nonogram::rijen[i][j]  << " ";
            j++;
        }
        cout << endl;
    }//for i
    int flag = nonogram::grootte[1];
    for(int j = 0; (flag > 0); j++){
        flag = nonogram::grootte[1];
        for(int i = 0;i < nonogram::grootte[1];i++){
            if(!(nonogram::kolommen[i][j])){
                cout << "  ";
                flag--;
                continue;
            }
            cout << nonogram::kolommen[i][j] << " ";
        }
        cout << endl;
    }
}//nonogram::drukaf

int randomgetal ( ) {
    static int getal = 42;
    getal = ( 221 * getal + 1 ) % 1000;
    return getal;
}//randomgetal

void nonogram::vulRandom(){
    bool flag;
    int randomGrens = nonogram::rpercent*10;

    for (int i = 0; i < nonogram::grootte[0]; i++ ){
        for (int j = 0; j < nonogram::grootte[1]; j++ ) {
            if (randomgetal() < randomGrens) {
                flag = true;
            }
            else {
                flag = false;
            }
            nonogram::nonoArray[i][j] = flag; //each element has bla random percentage chance of being 1
        }//for j
    }//for i
    //nonogram::drukAf()
}

void nonogram::leesFile(){
    ifstream invoer;
    string invoernaam = invoerf(invoer);
    invoer.open(invoernaam, ios::in);
    char letter;
    int sum;
    while(true){
        nonogram::grootte[0] = leesGetal(sum,letter,invoer);
        break;
    }
    while(true){
        nonogram::grootte[1] = leesGetal(sum,letter,invoer);
        break;
    }
    //cout << nonogram::grootte[0] << endl << nonogram::grootte[1] << endl;

    for(int i = 0; i < grootte[0];i++){
        int j = 0;
        while(!invoer.eof()){
            nonogram::rijen[i][j] = leesGetal(sum,letter,invoer);
            //cout << nonogram::rijen[i][j] << " ";
            j++;
            if(letter == '\n'){
                break;
            }
        }
        //cout << endl;
    }
    for(int i = 0; i < grootte[1];i++){
        int j = 0;
        while(!invoer.eof()){
            nonogram::kolommen[i][j] = leesGetal(sum,letter,invoer);
            //cout << nonogram::kolommen[i][j] << " ";
            j++;
            if(letter == '\n'){
                break;
            }
        }
        //cout << endl;
    }
}
//leesGetal voor files
int leesGetal(int& sum, char& letter, ifstream& invoer){
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
//leesGetal voor io
int leesGetal(int& sum, char& letter){
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

string invoerf(ifstream& invoer){
    string invoernaam;

    while(true){
        cout << "Wat is de naam van het invoerbestand?" << endl;
        cin >> invoernaam;
        //Open uitvoerbestand
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

void nonogram::beweegCursor(char keuze){
    switch(keuze){
        case('W'):
        case('w'):
            nonogram::cursorY > 0 ? nonogram::cursorY-- : 0;
            break;
        case('A'):
        case('a'):
            nonogram::cursorX > 0 ? nonogram::cursorX-- : 0;
            break;
        case('S'):
        case('s'):
            nonogram::cursorY < (nonogram::grootte[1] - 1)? nonogram::cursorY++ : 0;
            break;
        case('D'):
        case('d'):
            nonogram::cursorX < (nonogram::grootte[0] - 1)? nonogram::cursorX++ : 0;
            break;
        default:
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
    }
}

void nonogram::toggle(){
    nonoArray[cursorY][cursorX] = nonoArray[cursorY][cursorX] ? 0 : 1;
    return;
}
