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

        static const int MAX = 50; //-----------------added W
        static void maakSchoon();
        static void drukAf();
        static void vulRandom();

    private:
        static int grootte[2];
        static char symbool[2];
        static int cursor;
        static int rpercent;

         //---------------added W
        static bool nonoArray[MAX][MAX];
        static int rijen[MAX][MAX];
        static int hoogte, breedte;

};
int main(){
    nonogram::hoofdMenu();
    system("pause");
    return 0;
}
void nonogram::stelParameterMenu(){
    char keuze; //submenu keuze
    string trash = "foo";
    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(1){
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
                continue;
            case('S'):
            case('s'):
                continue;
            case('C'):
            case('c'):/*
                cout << "Welke kleur moet het nieuwe punt zijn bij het verplaatsen van de cursor?" << endl;
                cout << "Onveranderd(0), Wit(1), Zwart(2)" << endl;
                cout << "Uw wijziging is opgeslagen" << endl;*/
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

}
void nonogram::stelSymbool(){
    cout << "De huidige keuze is Wit:" << nonogram::symbool[0] << ", " << "Zwart:" << nonogram::symbool[1] <<endl; 
    string trash = "";
    char keuze;
    char keuze2;

    while(1){
        cout << "Welk symbool moeten de witte pixels voorstellen?" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();

        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }else if(1){
            /* code */
        }
        
    }
    while(1){

    }
    
    cout << "Uw wijziging is opgeslagen" << endl;
    return;
}
void nonogram::stelCursor(){

    cout << "De huidige keuze is " << nonogram::cursor <<endl; 
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

        }else
        {
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

        if(!isdigit(letter) && letter != '\n' || !(sum >= 0 && sum <= 100)){
            cout << "Ongeldige invoer, typ een getal tussen de 0 en 100 in voor uw keuze." << endl;
            //cin.clear();
            if(sum >=0 && sum <= 100){
                getline(cin,trash);
            }
            sum = 0;
            continue;
        }
        else{
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
        cout << "Kies uit 1 van de volgende opties: s(C)hoon, (R)andom, (P)arameters, (S)toppen" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();
        if(!trash.empty()){
            cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
            continue;
        }
        switch(keuze){
            case('C'):
            case('c'):
                continue;
            case('R'):
            case('r'):

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
    int i, j;
    for ( i = 0; i < hoogte; i++ ) {
        for ( j = 0; j < breedte; j++ ) {
            nonoArray[i][j] = false;
        }//for j
    }//for i
} //end maakNonogram

void nonogram::drukAf(){
    int i, j;
    for ( i = 0; i < hoogte; i++ ) {
        for ( j = 0; j < breedte; j++ ) {
            if ( nonoArray[i][j] )
                cout << " X";
            else
                cout << " O";
        }//for j
        cout << endl;
    }//for i
}//nonogram::drukaf

int randomgetal ( ) {
    static int getal = 42; // (*)
    getal = ( 221 * getal + 1 ) % 1000; // niet aan knoeien
    return getal;
}//randomgetal

void nonogram::vulRandom(){
    int vullTrue;
    int percentRandomGrens = (rpercent/100)*1000;
    if (randomgetal() < percentRandomGrens) {
        vullTrue = true;
    }
    else {
        vullTrue = false;
    }
    int i, j;
    for ( i = 0; i < hoogte; i++ ) {
        for ( j = 0; j < breedte; j++ ) {
            nonoArray[i][j] = vullTrue; //each element has bla random percentage chance of being 1
        }//for j
    }//for i
}


int nonogram::grootte[2];
char nonogram::symbool[2] = {0,0};
int nonogram::cursor;
int nonogram::rpercent = 50;
bool nonogram::nonoArray[nonogram::MAX][nonogram::MAX];
int nonogram::hoogte,nonogram::breedte;
