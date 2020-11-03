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
        static void stelCursor();
        static void stelPercentage();

        static void hoofdMenu();

        static const int MAX = 50;
        static void maakSchoon();
        static void drukAf();
        static void vulRandom();
        static void vulFile();
        
        static string invoerf(ifstream&);

    private:
        static int grootte[2];//hoogte,breedte
        static char symbool[2];//wit,zwart
        static int cursor;
        static int rpercent;

        static bool nonoArray[MAX][MAX];
        static int rijen[MAX][MAX];
        static int kolommen[MAX][MAX];
};

int nonogram::grootte[2] = {10,10};
char nonogram::symbool[2] = {'*',' '};
int nonogram::cursor = 0;
int nonogram::rpercent = 50;
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
    char keuze; //submenu keuze
    string trash = "foo";
    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(1){
        nonogram::drukAf();
        cout << "Kies uit 1 van de volgende opties: (G)rootte, (S)ymbool, (C)ursor, r(P)ercentage, (T)erug" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();

        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }
        switch(keuze){
            case('G'):
            case('g'):
                nonogram::stelGrootte();
                continue;
            case('S'):
            case('s'):
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
            case('T'):
            case('t'):
                return;
        }
    }
}
void nonogram::stelGrootte(){
    cout << "De huidige dimensies zijn:"<< endl;
    cout << "hoogte: " << nonogram::grootte[0] << ", breedte:" << nonogram::grootte[1] <<endl; 
    string trash = "";
    char letter;
    int getal;
    int sum = 0;

    while(1){
        cout << "Wat moet de hoogte van de puzzel zijn?" << endl;
        letter = cin.get();
        while(isdigit(letter)){
            //Indien meer iteraties, vermenigvuldig met 10
            getal = letter - '0';
            sum *= sum > 0 ? 10 : sum;
            sum += (getal);
            letter = cin.get();
        }

        if((!isdigit(letter) && letter != '\n') || !(sum >= 1 && sum <= 50)){
            cout << "Ongeldige invoer, typ een getal tussen de 1 en 50 in voor uw keuze." << endl;
            //cin.clear();
            if(sum >=1 && sum <= 50){
                getline(cin,trash);
            }
            sum = 0;
            continue;
        }else{
            nonogram::grootte[0] = sum;
            break;
        }
        sum = 0;
    }
    while(1){
        cout << "Wat moet de breedte van de puzzel zijn?" << endl;
        letter = cin.get();
        while(isdigit(letter)){
            //Indien meer iteraties, vermenigvuldig met 10
            getal = letter - '0';
            sum *= sum > 0 ? 10 : sum;
            sum += (getal);
            letter = cin.get();
        }

        if((!isdigit(letter) && letter != '\n') || !(sum >= 1 && sum <= 50)){
            cout << "Ongeldige invoer, typ een getal tussen de 1 en 50 in voor uw keuze." << endl;
            //cin.clear();
            if(sum >=1 && sum <= 50){
                getline(cin,trash);
            }
            sum = 0;
            continue;
        }else{
            nonogram::grootte[1] = sum;
            break;
        }
        sum = 0;
    }
    
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}

void nonogram::stelSymbool(){
    cout << "De huidige keuze is Wit:" << nonogram::symbool[0] << ", " << "Zwart:" << nonogram::symbool[1] <<endl; 
    string trash = "";
    char keuze;

    while(1){
        cout << "Welk symbool moeten de witte pixels voorstellen?" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();

        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }else{
            nonogram::symbool[0] = keuze;
            break;
        }
    }
    while(1){
        cout << "Welk symbool moeten de zwarte pixels voorstellen?" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();

        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }else if(nonogram::symbool[0]==keuze){
            cout << "De zwarte en witte pixels hebben hetzelfde symbool" << endl;
            cout <<  "Typ een nieuw karakter in voor de zwarte pixels" << endl;
            continue;
        }
        else{
            nonogram::symbool[1] = keuze;
            break;
        }
    }
    
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}
void nonogram::stelCursor(){

    switch(nonogram::cursor){
        case(0):
            cout << "De huidige keuze is onveranderd(0)" <<endl; 
            break;
        case(1):
            cout << "De huidige keuze is wit(1)" <<endl; 
            break;
        case(2):
            cout << "De huidige keuze is zwart(2)" <<endl; 
            break;
    }
    string trash = "";
    char keuze;
    int getal;

    while(1){
        cout << "Welke kleur moet het nieuwe punt zijn bij het verplaatsen van de cursor?" << endl;
        cout << "Onveranderd(0), Wit(1), Zwart(2)" << endl;
        keuze = cin.get();
        getal = keuze - '0';
        getline(cin,trash);
        cin.clear();

        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }else if(getal >=0 && getal <=2){
            nonogram::cursor = getal;
            break;

        }else{
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }
    }
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}

void nonogram::stelPercentage(){
    cout << "Het huidige randompercentage is " << nonogram::rpercent << " procent" <<endl; 
    string trash = "";
    char letter;
    int getal;
    int sum = 0;
    while(1){
        cout << "Welk random percentage(0-100) wilt u instellen?" << endl;
        letter = cin.get();
        while(isdigit(letter)){
            //Indien meer iteraties, vermenigvuldig met 10
            getal = letter - '0';
            sum *= sum > 0 ? 10 : sum;
            sum += (getal);
            letter = cin.get();
        }

        if((!isdigit(letter) && letter != '\n') || !(sum >= 0 && sum <= 100)){
            cout << "Ongeldige invoer, typ een getal tussen de 0 en 100 in voor uw keuze." << endl;
            //cin.clear();
            if(sum >=0 && sum <= 100){
                getline(cin,trash);
            }
            sum = 0;
            continue;
        }else{
            rpercent = sum;
            cout << "Uw wijziging is opgeslagen" << endl << sum << endl;
            return;
        }
        sum = 0;
    }       
}

void nonogram::hoofdMenu(){
    char keuze;
    string trash = "";

    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(1){
        nonogram::drukAf();
        cout << "Kies uit 1 van de volgende opties: (F)ile, s(C)hoon, (R)andom, (P)arameters, (S)toppen" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();
        nonogram nono;
        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }
        switch(keuze){
            case('F'):
            case('f'):
                nonogram::vulFile();
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
            case('S'):
            case('s'):
                return;
            default:
                cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
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
        for (int j = 0; j < nonogram::grootte[1]; j++ ) {
            if (nonogram::nonoArray[i][j] )
                cout << nonogram::symbool[0];
            else
                cout << nonogram::symbool[1];
            cout << ' ';
        }//for j
        cout << endl;
    }//for i
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

void nonogram::vulFile(){
    ifstream invoer;
    string invoernaam = invoerf(invoer);
    invoer.open(invoernaam, ios::in);

    char letter;
    int getal;
    int sum = 0;
    while(1){
        letter = invoer.get();
        while(isdigit(letter)){
            //Indien meer iteraties, vermenigvuldig met 10
            getal = letter - '0';
            sum *= sum > 0 ? 10 : sum;
            sum += (getal);
            letter = invoer.get();
        }
        nonogram::grootte[0] = sum;
        sum = 0;
        break;
    }
    while(1){
        letter = invoer.get();
        while(isdigit(letter)){
            //Indien meer iteraties, vermenigvuldig met 10
            getal = letter - '0';
            sum *= sum > 0 ? 10 : sum;
            sum += (getal);
            letter = invoer.get();
        }
        nonogram::grootte[1] = sum;
        sum = 0;
        break;
    }
    cout << nonogram::grootte[0] << endl << nonogram::grootte[1];
/*
    while(!invoer.eof()){
        for(int i = 0; i < rijen; i++){
            char letter = invoer.get();
            if(letter == '0'){
                cout << endl;
                continue;
            }
            cout << letter;
        }
    }
    for(int i = 0; i < kolommen; i++){
    }*/
}

string nonogram::invoerf(ifstream& invoer){
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
