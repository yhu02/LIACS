#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;


class nonogram{
    public:
        nonogram(){
        };
        //static int maakSchoonMenu();
        //static void maakSchoon();
        //static int vulRandomMenu();
        //static void vulRandom();
        //static int drukAfMenu();
        //static void drukAf();
        
        static int stelParameterMenu();
        static void stelGrootte();
        static void stelSymbool();
        static void stelCursor();
        static void stelPercentage();

        static void hoofdMenu();
    private:
        static int grootte[2];
        static char symbool[2];
        static int cursor;
        static int rpercent;
};
int main(){
    nonogram::hoofdMenu();
    system("pause");
    return 0;
}
int nonogram::stelParameterMenu(){
    char keuze; //submenu keuze
    bool valid = false;
    string trash = "foo";
    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(!trash.empty() || !valid){
        valid = true;
        cout << "Kies uit 1 van de volgende opties: (G)rootte, (S)ymbool, (C)ursor, r(P)ercentage, (T)erug" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();

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
                hoofdMenu();  
                return 0;
            default:
                valid = false;
                cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
        }
    }
    return 1;
}
void nonogram::stelGrootte(){

}
void nonogram::stelSymbool(){

}
void nonogram::stelCursor(){

}

void nonogram::stelPercentage(){
    cout << "Het huidige randompercentage is " << nonogram::rpercent << " procent" <<endl; 
    string trash = "bar";
    char letter;
    int getal;
    int sum = 0;
    bool valid = false;
    while(!valid){
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
            cin.clear();
            getline(cin,trash);
            sum = 0;
            continue;
        }
        else{
            rpercent = sum;
            cout << "Uw wijziging is opgeslagen" << endl << sum << endl;
            valid = nonogram::stelParameterMenu();
        }
        sum = 0;
    }       
}

void nonogram::hoofdMenu(){
    char keuze;
    bool valid = false;
    string trash = "foo";
    //Check of de gebruiker geen extra karakters heeft gebruikt
    while(!trash.empty() || !valid){
        valid = true;
        cout << "Kies uit 1 van de volgende opties: s(C)hoon, (R)andom, (P)arameters, (S)toppen" << endl;
        keuze = cin.get();
        getline(cin,trash);
        cin.clear();

        switch(keuze){
            case('C'):
            case('c'):

                break;
            case('R'):
            case('r'):

                break;
            case('P'):
            case('p'):
                valid = nonogram::stelParameterMenu();
                continue;
            case('S'):
            case('s'):
                
                break;
            default:
                valid = true;
                cout << "Ongeldige invoer, typ een geldig karakter in voor uw keuze." << endl;
        }
    }
}
/*
int countf(){
    while(isdigit(letter)){
        //Indien meer iteraties, vermenigvuldig met 10
        sum *= sum > 0 ? 10 : sum;
        sum += (letter - '0');
        letter = invoer.get();
    }
    //Wijs de waarde 1 toe aan sum als er geen getal achter het
    //karakter staat
    return (sum = sum ? sum : 1);
}*/
int nonogram::rpercent = 50;
