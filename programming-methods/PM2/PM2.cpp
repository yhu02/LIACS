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
string invoerf(ifstream&);
string uitvoerf();
bool keuzef();

int main()
{
    ifstream invoer;
    ofstream uitvoer;
    bool keuze2 = keuzef();
    string invoernaam = invoerf(invoer);
    string uitvoernaam = uitvoerf();

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
    int temp;
    int sum = 0;
    int digitCount = 0;
    while(!invoer.eof()){
        if(letter == '\\'){
            letter = invoer.get();   
            if(letter == '\\'){
                temp = letter;
                letter = invoer.get();
                while(isdigit(letter)){
                    //Indien meer iteraties, vermenigvuldig met 10
                    sum *= sum ? 10 : 1;
                    sum += (letter - '0');
                    digitCount++;
                    letter = invoer.get();
                }       
                //Indien geen meerdere getallen, wijs de waarde 1 toe aan sum
                sum = sum ? sum : 1;
                for(int i = 0; i < sum; i++){
                    uitvoer.put(char(temp));
                }
                sum = 0;
            }
            else if(isdigit(letter)){
                temp = letter - '0';
                //Check of het getal meerdere keren moet worden herhaald
                letter = invoer.get();
                while(isdigit(letter)){
                    //Indien meer iteraties, vermenigvuldig met 10
                    sum *= sum > 0 ? 10 : 1;
                    sum += (letter - '0');
                    digitCount++;
                    letter = invoer.get();
                }

                sum = sum ? sum : 1;
                for(int i = 0; i < sum; i++){
                    uitvoer.put(temp + '0');
                }
                sum = 0;
            }
        }
        else{
            temp = letter;
            letter = invoer.get();
            while(isdigit(letter)){
                //Indien meer iteraties, vermenigvuldig met 10
                sum *= sum ? 10 : 1;
                sum += (letter - '0');
                digitCount++;
                letter = invoer.get();
            }       
            //Indien geen meerdere getallen, wijs de waarde 1 toe aan sum
            sum = sum ? sum : 1;
            for(int i = 0; i < sum; i++){
                uitvoer.put(char(temp));
            }
        
            sum = 0;
        }
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

string invoerf(ifstream& invoer){
    string invoernaam;
    bool flag_invoer = true;
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
    return invoernaam;

}

string uitvoerf(){
    string uitvoernaam;
    cout << "Wat is de naam van het uitvoerbestand?" << endl;
    cin >> uitvoernaam;

    return uitvoernaam;
}

bool keuzef(){
    cout << "Wil je coderen(c) of decoderen?(d)" << endl;
    char keuze;
    bool keuze2;
    bool flag_keuze = true;
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
    return (keuze2);
}
