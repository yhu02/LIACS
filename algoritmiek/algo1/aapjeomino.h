// Definitie van klasse AapjeOmino

#ifndef AapjeOminoHVar  // voorkom dat dit bestand meerdere keren
#define AapjeOminoHVar  // ge-include wordt

#include <vector>
#include "zet.h"
using namespace std;

const int SteenZijden = 4;   //Aantal zijdes van een steen 
const int MaxDimensie = 10;  //Maximaal aantal rijen en maximaal aantal kolommen in een spel

class AapjeOmino
{ public:
    AapjeOmino ();                              //Default constructor.                         

    bool leesIn (const char* invoernaam);       //Lees bestand in

    bool eindstand ();                          //Check of eindstand is bereikt

    void drukAf ();                             //Druk de hele stand (bord, stenen in de twee handen en de pot,
                                                //speler aan beurt) af op het scherm.

    vector<Zet> bepaalMogelijkeZetten ();       //Bepaal alle mogelijke zetten op basis van de huidige stenen voor de huidige speler

    int haalSteenUitPot ();                     //Haal steen uit pot

    void wisselSpeler ();                       //Wissel van beurt

    bool doeZet (Zet zet);                      //Doe een zet in zetten

    vector<Zet> bepaalGoedeZetten ();           //Bepaal de beste zetten op basis vana de meeste bezette buurvakken

    int besteScore (Zet &besteZet, long long &aantalStanden);   //Bepaal best mogelijke score op basis van de huidige stand

    bool genereerRandomSpel (int hoogte0, int breedte0,          //Genereer random spel
           int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
           int minGetal, int maxGetal);
  private:
    int leesGetal(char& letter, ifstream& invoer);              //Lees getal uit bestand

    void drukAfStenen(vector<vector<int>> stenen);              //Druk alle stenen af in de vector stenen

    void bepaalMogelijkeZettenf(int rij, int kolom, int nr, int zijde);

    vector<int> schuif(vector<int> vec, int schuif);            //Verschuif de elementen in vec met schuif naar rechts en weer terug naar begin

    pair<int,int> bord[MaxDimensie][MaxDimensie];               // in een pair kunnen we een steennummer en een rotatie opslaan

    vector<vector<int>> stenen, speler1Stenen, speler2Stenen, potStenen;    //Totale aantal stenen, in hand van spelers, en de pot

    vector<vector<int>>* huidigStenen;                                      //Pointer naar de hand van de huidige speler 

    vector<Zet> zetten;                                                     //Vector met alle huidige zetten van de huidige speler

    vector<Zet> oudeZetten;                                                 //Vector met alle zetten die gedaan zijn door beide spelers
    
    bool doeZet2 (Zet zet);                                                 //Doe een zet in zetten
    
    bool undoeZet(Zet zet);                                                 //Verwijder steen van bord

    int huidigStand;                                                        //Counter met de huidige stand

    int hoogte, breedte,  // van het bord
        nrStenen,         // totaal aantal stenen in het spel
        aanBeurt,         // speler die aan de beurt is
        beginStenen,      // aantal beginstenen per speler
        rij, kolom,       // positie steen 0
        score;            // score initialiseren

};

#endif

