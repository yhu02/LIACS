// Definitie van klasse AapjeOmino

#ifndef AapjeOminoHVar  // voorkom dat dit bestand meerdere keren
#define AapjeOminoHVar  // ge-include wordt

#include <vector>
#include "zet.h"
using namespace std;

const int SteenZijden = 4;
const int MaxDimensie = 10;  // maximaal aantal rijen en maximaal aantal
                             // kolommen in een spel

class AapjeOmino
{ public:
    // Default constructor.
    AapjeOmino ();

    bool leesIn (const char* invoernaam);

    bool eindstand ();

    // Druk de hele stand (bord, stenen in de twee handen en de pot,
    // speler aan beurt) af op het scherm.
    void drukAf ();

    vector<Zet> bepaalMogelijkeZetten ();

    int haalSteenUitPot ();

    void wisselSpeler ();

    bool doeZet (Zet zet);

    bool undoeZet(Zet zet);

    vector<Zet> bepaalGoedeZetten ();

    // Bepaal met behulp van brute force de eindscore voor de speler die in
    // de huidige stand (= de stand van de huidige recursieve aanroep)
    // aan de beurt is, wanneer beide spelers vanaf dit punt optimaal verder
    // spelen.
    // De score is het aantal resterende stenen van de andere speler min
    // het aantal resterende stenen van de huidige speler (hoe hoger hoe
    // beter).
    // Retourneer: de berekende eindscore
    // Post:
    // * als de huidige speler in deze beurt daadwerkelijk een steen kan
    //   opleggen (eventueel na een steen uit de pot gehaald te hebben),
    //   bevat parameter besteZet een zet voor de huidige speler aan beurt
    //   die tot de beste score kan leiden
    // * anders bevat parameter besteZet een passende default waarde
    // * aantalStanden is gelijk aan het aantal standen dat we hebben
    //   bekeken bij het bepalen van de beste eindscore
    // * de stand in het spel is nog onveranderd
    int besteScore (Zet &besteZet, long long &aantalStanden);

    bool genereerRandomSpel (int hoogte0, int breedte0,
           int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
           int minGetal, int maxGetal);
    int leesGetal(char& letter, ifstream& invoer);
    vector<vector<int>> stenen, speler1Stenen, speler2Stenen, potStenen;
    vector<vector<int>>* huidigStenen = &speler1Stenen;
    vector<Zet> zetten;
    vector<Zet> oudeZetten;
    void drukAfStenen(vector<vector<int>> stenen);
    int winnend(int stand);
    
  private:
    // TODO: uw eigen memberfuncties en -variabelen
    void bepaalMogelijkeZettenf(int rij, int kolom, int nr, int zijde);
      // alvast enkele membervariabelen
    vector<int> schuif(vector<int> vec, int schuif);
    pair<int,int> bord[MaxDimensie][MaxDimensie];
              // in een pair kunnen we een steennummer en een rotatie opslaan
    int hoogte, breedte,  // van het bord
        nrStenen,     // totaal aantal stenen in het spel
        aanBeurt = 1,     // speler die aan de beurt is
        beginStenen,  // aantal beginstenen per speler
        rij, kolom;   // positie steen 0

    int uitPotGehaald;


};

#endif

