#include <string>
#include <fstream>
#include <iostream>
#include <climits>
using namespace std;
 
//Programma van Yvo Hu en Wietske ...
//Laatste update 25-9-2020
//Gebruikte compiler GNU g++

//Function prototypes
void splitGetal(int, ofstream&);
void encode(ifstream&, ofstream&, string, string);
void decode(ifstream&, ofstream&, string, string);
int pow(int, int);
void lychrel(ifstream&, string);
long long keerGetal(long long);


int main()
{
    ifstream invoer;
    ofstream uitvoer;
    string invoernaam;
    string uitvoernaam;
    char keuze;
    bool keuze2;
    int flag_invoer = 1;
    int flag_uitvoer = 1;
    int flag_keuze = 1;
    cout << "Wil je coderen(c) of decoderen?(d)" << endl;
    do{
        keuze = cin.get();
        if(keuze == 'C' || keuze == 'c'){
            flag_keuze = 0;
            keuze2 = true;
        }
        else if(keuze == 'D' || keuze == 'd'){
            flag_keuze = 0;
            keuze2 = false;
        }
        else{
            cout << "Ongeldige invoer, probeer het opnieuw" << endl;
        }
    } while(flag_keuze);

    do{
        cout << "Wat is de naam van het invoerbestand?" << endl;
        cin >> invoernaam;
        invoer.open(invoernaam,ios::in);
        if(invoer.fail()){
            cerr << "Het openen van " << invoernaam << " is mislukt. Probeer het nogmaals" << endl;
        }
        else{
            flag_invoer = 0;
            invoer.close();
        }
    } while(flag_invoer);

    //
    cout << "Wat is de naam van het uitvoerbestand?" << endl;
    cin >> uitvoernaam;
    keuze2 ? encode(invoer, uitvoer, invoernaam, uitvoernaam) : decode(invoer, uitvoer, invoernaam, uitvoernaam);

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
void encode(ifstream& invoer, ofstream& uitvoer, string invoernaam, string uitvoernaam)
{
    invoer.open(invoernaam, ios::in);
    uitvoer.open(uitvoernaam, ios::out);
    char letter = invoer.get();
    char vorige_letter = letter;
    while(!invoer.eof()){
        int count = 0;
        while(letter == vorige_letter){
            count++;
            letter = invoer.get();
        }
        if(vorige_letter == '\\'){
            uitvoer.put('\\');
            uitvoer.put('\\');
        }
        else if(isdigit(vorige_letter)){
            uitvoer.put('\\');
            uitvoer.put(vorige_letter);
        }
        else if(vorige_letter == '\n'){
            for(int i = 0; i < count; i++){
                uitvoer.put(vorige_letter);
            }
        }
        else{
            uitvoer.put(vorige_letter);
        }
        if(count > 1 && vorige_letter != '\n'){
            splitGetal(count,uitvoer);
        }
        vorige_letter = letter;
    }
    invoer.close();
    uitvoer.close();
    lychrel(invoer,invoernaam);
}
//Decodeer functie
void decode(ifstream& invoer, ofstream& uitvoer, string invoernaam, string uitvoernaam)
{
    invoer.open(invoernaam, ios::in);
    uitvoer.open(uitvoernaam, ios::out);
    char letter = invoer.get();
    char vorige_letter = letter;
    char vorige_letter2 = vorige_letter;
    int getal;
    int sum = 0;
    int digitCount = 0;
    while(!invoer.eof()){
        if(letter == '\\'){
            letter = invoer.get();   
            if(letter == '\\'){
            }
            else if(isdigit(letter)){
                getal = letter - '0';
                cout << getal << endl;
                //Check of het getal meerdere keren moet worden herhaald
                do{
                    letter = invoer.get();
                    if(isdigit(letter)){
                        //Indien meer iteraties, vermenigvuldig met 10
                        sum *= sum > 0 ? 10 : 1;
                        sum += (letter - '0');
                        digitCount++;
                    }
                }while(isdigit(letter));
                //
                for(int i = 0; i < sum; i++){
                    cout << getal;
                    uitvoer.put(getal + '0');
                }
            }
        }
        letter = invoer.get();
    }
    invoer.close();
    uitvoer.close();
}

//Machtsfunctie
int pow(int x, int y){
    int result = 1;
    for(int i=1; i<y; i++)
    result *= x;
    return result;
}

//Lees het bestand per karakter en sla getallenreeksen op om vervolgens te checken of het lychrelgetallen zijn
void lychrel(ifstream& invoer, string invoernaam){
    invoer.open(invoernaam,ios::in);
    char letter = invoer.get();
    while(!invoer.eof()){
        int count = 0;
        int getal[10];;
        int result= 0;
        if(isdigit(letter)){
            while(isdigit(letter)){
                getal[count] = letter - '0';
                count++;
                letter = invoer.get();
            }
            for(int i = 0; i < count;i++){
                result += getal[i]*pow(10,count-i);
            }
            cout << result;

            while(true){
            const int limit = INT_MAX;
            long long temp = result;
            long long rev;
            int count = 0;
            while(temp < INT_MAX){
                rev = keerGetal(temp);
                if(temp == keerGetal(temp)){
                    cout << " iteraties:" << count;
                    break;
                }
                temp = temp + rev;
                count++;
            }
            if(temp >= INT_MAX){
                cout << " iteraties:" << count << " tot het getal groter is dan INT_MAX";
            }
            break;
            }
            cout << endl;
        }
        //reset
        letter = invoer.get();
    }
    invoer.close();
}

long long keerGetal(long long getal){
    long long result = 0;
    while (getal> 0){
        result = (result * 10) + (getal % 10);
        getal = getal / 10;
    }
    return result;
}