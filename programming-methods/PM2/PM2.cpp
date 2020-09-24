#include <string>
#include <fstream>
#include <iostream>
#include <climits>
using namespace std;
 
//Programma van Yvo Hu en Wietske ...
//Laatste update 22-9-2020
//Gebruikte compiler GNU g++
void split_getal(int,ofstream&);
void encode(ifstream&, ofstream&);

void is_lychrel_getal(string invoernaam){
    ifstream invoer(invoernaam,ios::in);
    char letter = invoer.get();
    while(!invoer.eof()){
        int count = 0;
        int getal[10] = {0,0,0,0,0,0,0,0,0,0};
        if(isdigit(letter)){
            while(isdigit(letter)){
                getal[count] = letter - '0';
                std::cout << getal[count];
                count++;
                letter = invoer.get();
            }
            cout << endl;
        }

        //process result
        //reset
        letter = invoer.get();
    }
    invoer.close();
}

/*
long long int numReverse(long long int number)
{
    long long int rem = 0;
    while (number > 0)
    {
    rem = (rem * 10) + (number % 10);
        number = number / 10;
    }
    return rem;
}
bool is_Palindrome(long long int num)
{
    return (num == numReverse(num));
}
bool isLychrel(int num, const int iterCount = 500)
{
    long long int temp = num;
    long long int rev;
    for (int i = 0; i < iterCount; i++)
    {
        rev = numReverse(temp);
        if (is_Palindrome(rev + temp))
            return false;
        temp = temp + rev;
    }
    return true;
}
*/

int main()
{
    ifstream invoer;
    ofstream uitvoer;
    string invoernaam;
    string uitvoernaam;
    char keuze;
    int flag_invoer = 1;
    int flag_uitvoer = 1;
    int flag_keuze = 1;
    cout << "Wil je coderen(c) of decoderen?(d)" << endl;
    do{
        keuze = cin.get();
        if(keuze == 'C' || keuze == 'c'){
            flag_keuze = 0;
        }
        else if(keuze == 'D' || keuze == 'd'){
            flag_keuze = 0;
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
        }
    } while(flag_invoer);

    //
    cout << "Wat is de naam van het uitvoerbestand?" << endl;
    cin >> uitvoernaam;
    uitvoer.open(uitvoernaam,ios::out);

    encode(invoer,uitvoer);
    invoer.close();
    uitvoer.close();

    is_lychrel_getal(invoernaam);

    system("pause");
    return 0;
}

void split_getal(int i,ofstream& uitvoer)
{
    i > 9 ? split_getal(i / 10,uitvoer) : void();

    int getal = i % 10;
    uitvoer.put('0' + getal);
}

void encode(ifstream& invoer,ofstream& uitvoer)
{
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
            split_getal(count,uitvoer);
        }
        vorige_letter = letter;
    }
    uitvoer.close();
}
