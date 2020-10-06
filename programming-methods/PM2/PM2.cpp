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

    //Keuze encode of decode
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
void encode(ifstream& invoer, 
	    ofstream& uitvoer, 
	    string invoernaam,
            string uitvoernaam)
{
    //Open invoer- en uitvoerbestand
    invoer.open(invoernaam, ios::in);
    uitvoer.open(uitvoernaam, ios::out);
    //Get het eerste karakter
    char letter = invoer.get();
    char vorige_letter = letter;
    int count;
    //Als het EOF symbool nog niet is bereikt voer uit
    while(!invoer.eof()){
        //Reset
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
    //Sluit invoer- en uitvoerbestannd
    invoer.close();
    uitvoer.close();
    //Check of de getallen Lychrel zijn
    lychrel(invoer,invoernaam);
}
//Decodeer functie
void decode(ifstream& invoer, 
	    ofstream& uitvoer, 
	    string invoernaam,
	    string uitvoernaam)
{
    //Open invoer- en uitvoerbestand
    invoer.open(invoernaam, ios::in);
    uitvoer.open(uitvoernaam, ios::out);
    //Get het eerste karakter
    char letter = invoer.get();
    int temp;
    int sum = 0;

    //Als het EOF symbool nog niet is bereikt
    while(!invoer.eof()){
            //'\\' overslaan
        if(letter == '\\'){
            letter = invoer.get();
    }
        //Print letter o.b.v. sum
        temp = letter;
        letter = invoer.get();
        countf(sum, letter, invoer);
        for(int i = 0; i < sum; i++){
            uitvoer.put(char(temp));
        }
        //Reset
        sum = 0;
    }
    //Sluit invoer- en uitvoerbestand
    invoer.close();
    uitvoer.close();
}

//Lees het bestand per karakter en sla getallenreeksen op om
//vervolgens te checken of het lychrelgetallen zijn
void lychrel(ifstream& invoer, string invoernaam){
    //Open invoerbestand
    invoer.open(invoernaam,ios::in);
    //Get het eerste karakter
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
    //Sluit invoerbestand
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
        //Herhaal indien geen geldig karakter
        else{
            cerr << "Ongeldige invoer, probeer het opnieuw" << endl;
        }
    }

    return (keuze2);
}
 //Check of het getal meerdere keren moet worden herhaald
void countf(int& sum, char& letter, ifstream& invoer){
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
        long long temp = sum;
        long long rev;
        int count = 0;
        //Verwerk het getal tot INT_MAX
        while(temp < INT_MAX){
            //Omgekeerde van getal
            rev = keerGetal(temp);
            //Als het getal niet lychrel is
            if(temp == keerGetal(temp)){
                cout << " iteraties:" << count;
                break;
            }
            //Nog een iteratie
            temp = temp + rev;
            count++;
        }
        //Uitvoer aantal iteraties tot het getal groter is dan INT_MAX
        if(temp >= INT_MAX){
            cout << " iteraties:" << count << " tot het getal groter "
            "is dan INT_MAX";
        }
        break;
    }
}

void infoblok(){
    for(int i = 1; i<44;i++){
        cout << '*';
    } 
    cout << endl;
    cout << "* Yvo Hu studentnummer: 2962802           *" << endl;
    cout << "* Wietske Holwerda studentnummer: 2838192 *" << endl;
	cout << "* Studiejaar: 2020                        *" << endl;
	cout << "* Studierichting: Informatica             *" << endl;
    for(int i = 1; i<44;i++){
        cout << '*';
    }
    cout << endl;
}
