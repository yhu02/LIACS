#include <string>
#include <fstream>
#include <iostream>
#include <climits>
using namespace std;

//Programma van Yvo Hu en Wietske Holwerda
//Laatste update 06-10-2020
//Gebruikte compiler GNU g++

//Function prototypes
void infoblok();
void lychrelf(int&);
void countf(int&, char&, ifstream&);
void splitGetal(int, ofstream&);
void decode(ifstream&, ofstream&, string, string);
void encode(ifstream&, ofstream&, string, string);
void lychrel(ifstream&, string);
string uitvoerf();
string invoerf(ifstream&);
bool keuzef();
long long keerGetal(long long);

int main()
{
    ifstream invoer;
    ofstream uitvoer;
    infoblok();
    bool keuze2 = keuzef();
    string invoernaam = invoerf(invoer);
    string uitvoernaam = uitvoerf();

    //kies encode of decode
    keuze2 ? encode(invoer, uitvoer, invoernaam, uitvoernaam)
    : decode(invoer, uitvoer, invoernaam, uitvoernaam);
    system("pause");
    return 0;
}

void splitGetal(int i, ofstream& uitvoer)
{
    //Recursie voor getallen groter dan 9
    i > 9 ? splitGetal(i / 10,uitvoer) : void();

    int getal = i % 10;
    uitvoer.put('0' + getal);
}

//Codeer functie
void encode(ifstream& invoer, ofstream& uitvoer, string invoernaam,
            string uitvoernaam)
{
    //input en output files openen
    invoer.open(invoernaam, ios::in);
    uitvoer.open(uitvoernaam, ios::out);
    //get eerste karakter
    char letter = invoer.get();
    char vorige_letter = letter;
    int count;
    //Als het EOF symbool nog niet is bereikt voer uit
    while(!invoer.eof()){
        //reset
        count = 0;
        vorige_letter = letter;
        //Als de huidige letter overeenkomt met de vorige letter ga
        //verder en verhoog count met 1
        while(letter == vorige_letter){
            count++;
            letter = invoer.get();
        }
        //Check of de vorige letter een \, een getal of een whitespace
        // is met aparte verwerking voor iedere situatie
        if(vorige_letter == '\\'){
            uitvoer.put(vorige_letter);
            uitvoer.put(vorige_letter);
        }
        else if(isdigit(vorige_letter)){
            uitvoer.put('\\');
            uitvoer.put(vorige_letter);
        }
        else if(isspace(vorige_letter)){
            for(int i = 0; i < count; i++){
                uitvoer.put(vorige_letter);
            }
        }
        else{
            uitvoer.put(vorige_letter);
        }
        if(count > 1 && !isspace(vorige_letter)){
            splitGetal(count,uitvoer);
        }
    }
    //input en output files sluiten
    invoer.close();
    uitvoer.close();
    //getallen checken voor lychrel getallen
    lychrel(invoer,invoernaam);
}
//Decodeer functie
void decode(ifstream& invoer, ofstream& uitvoer, string invoernaam,
            string uitvoernaam)
{
    //input en output files openen
    invoer.open(invoernaam, ios::in);
    uitvoer.open(uitvoernaam, ios::out);
    //get eerste karakter
    char letter = invoer.get();
    int temp;
    int sum = 0;

    //Als het EOF symbool nog niet is bereikt
    while(!invoer.eof()){
            //'\\' overslaan
        if(letter == '\\'){
            letter = invoer.get();

        //medere keer zelfde letter
        temp = letter;
        letter = invoer.get();
        countf(sum, letter, invoer);
        for(int i = 0; i < sum; i++){
            uitvoer.put(char(temp));
        }
        //Reset
        sum = 0;
    }
    //input en output files sluiten
    invoer.close();
    uitvoer.close();
}

//Lees het bestand per karakter en sla getallenreeksen op om
//vervolgens te checken of het lychrelgetallen zijn
void lychrel(ifstream& invoer, string invoernaam){
    //input file openen
    invoer.open(invoernaam,ios::in);
    //get eerste karakter
    char letter = invoer.get();
    int sum;

    //Als het EOF symbool nog niet is bereikt
    while(!invoer.eof()){
        if(isdigit(letter)){
            countf(sum, letter, invoer);
            cout << sum;
            lychrelf(sum);
            cout << endl;
        }
        //Reset
        sum = 0;
        letter = invoer.get();
    }
    //input file sluiten
    invoer.close();
}
//Voer het getal uit van rechts naar links
//Gebruikt long long om ook grote getallen te kunnen verwerken
long long keerGetal(long long getal){
    long long result = 0;

    while (getal> 0){
        result = (result * 10) + (getal % 10);
        getal = getal / 10;
    }

    return result;
}
//Bepaal invoernaam
string invoerf(ifstream& invoer){
    string invoernaam;

    while(true){
        cout << "Wat is de naam van het invoerbestand?" << endl;
        cin >> invoernaam;
        //input file openen om te testen
        invoer.open(invoernaam,ios::in);
        //Error handling
        if(invoer.fail()){
            cerr << "Het openen van " << invoernaam << " is mislukt. "
            "Probeer het nogmaals" << endl;
        }
        else{
                //input file sluiten na testing
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

//Bepaal codeer of decodeer keuze
bool keuzef(){
    char keuze;
    bool keuze2;

    cout << "Wil je coderen(c) of decoderen?(d)" << endl;
    while(true){
        keuze = cin.get();
        if(keuze == 'C' || keuze == 'c'){
            keuze2 = true;
            break;
        }
        else if(keuze == 'D' || keuze == 'd'){
            keuze2 = false;
            break;
        }
        //Error handling, vragen tot een geldige antwoord
        else{
            cerr << "Ongeldige invoer, probeer het opnieuw" << endl;
        }
    }

    return (keuze2);
}
 //Check of het getal meerdere keren moet worden herhaald
void countf(int& sum, char& letter, ifstream& invoer){
    //als getal is
    while(isdigit(letter)){
        //Indien meer iteraties, vermenigvuldig met 10
        sum *= sum > 0 ? 10 : sum;
        sum += (letter - '0');
        letter = invoer.get();
    }
    //Wijs de waarde 1 toe aan sum als er geen getal achter het
    //karakter staat
    sum = sum ? sum : 1;
}
//Bereken of het getal een lychrel getal is en voer het resultaat uit
void lychrelf(int& sum){
    while(true){
        // set limiet als INT_MAX
        const int limit = INT_MAX;
        long long temp = sum;
        long long rev;
        int count = 0;
        while(temp < INT_MAX){
            //reverse van getal
            rev = keerGetal(temp);
            //als getal geen lychrel getal is
            if(temp == keerGetal(temp)){
                cout << " iteraties:" << count;
                break;
            }
            //nog een iteratie
            temp = temp + rev;
            count++;
        }
        //getal te hoog, kan niet verder uitrekenen
        if(temp >= INT_MAX){
            cout << " iteraties:" << count << " tot het getal groter "
            "is dan INT_MAX";
        }
        break;
    }
}

void infoblok(){

}
