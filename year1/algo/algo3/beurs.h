// Definitie van klasse Beurs

#ifndef BeursHVar  // voorkom dat dit bestand meerdere keren
#define BeursHVar  // ge-include wordt

#include <vector>
#include "constantes.h"


class Beurs
{ public:

    // Default constructor
    Beurs ();
    ~Beurs ();


    bool leesIn (const char* invoernaam);

    void drukAfInvoer ();

    double bepaalMaxBedragBU
             (std::vector <std::vector <std::pair <bool,int> > > &transacties);

    // Bepaal recursief het maximale bedrag dat de belegger aan het eind
    // van dag tw in kas kan hebben (op dat moment heeft hij geen aandelen
    // meer). Als parameter memo = true worden berekende deelresultaten
    // opgeslagen in tabel bedrag en hergebruikt. Als memo = false, worden
    // deelresultaten steeds opnieuw berekend.
    // De benodigde transacties om het maximale bedrag te behalen, hoeven
    // niet bepaald te worden.
    // Pre:
    // * Er is een geldige instantie ingelezen.
    // Retourneer:
    // * Het maximale bedrag op dag tw.
    double bepaalMaxBedragRec (bool memo);
    void hulpRecFunctie (int t);

    //Bereken de beste combinatie aandelen om te kopen en het bijbehorende bedrag in kas.
    void berekenMaxb0(int t,  int &maxa, double &maxb0);

    //Vul de bedrag tabel
    void vulTabel(int t, std::vector<double> &dagb0);

    //Druk alle transacties af van alle dagen
    void drukAfTransacties (std::vector <std::vector <std::pair <bool,int> > > &transacties);

    //Voeg de transacties van een bepaalde dag toe aan de vector transacties
    void voegToeDagTransacties (int maxa, std::vector <std::vector <std::pair <bool,int> > > &transacties);  


  private:
    int tw,    // tijdstip_wereldreis
        n;     // het aantal verschillende aandelen
    double p,  // provisie op transacties, in procenten.
           b0; // bedrag op tijdstip t
    
    std::vector<std::vector<double>> bedrag;
  
    std::vector<std::vector<double>> koers; //Pointer naar een pointer naar een array met de prijzen van elk aandeel op dag t
    std::vector<double> rentes; //Pointer naar een array met de actuele rente op dag t
    int a;          //Bitstring aandelenbezit lopend van rechts naar links op tijdstip t
    

};

#endif

