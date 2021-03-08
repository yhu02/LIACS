// Definitie van klasse AapjeOmino

#ifndef AapjeOminoHVar  // voorkom dat dit bestand meerdere keren
#define AapjeOminoHVar  // ge-include wordt

#include <vector>
#include "zet.h"
using namespace std;

const int MaxDimensie = 10;  // maximaal aantal rijen en maximaal aantal
                             // kolommen in een spel

class AapjeOmino
{ public:
    // Default constructor.
    AapjeOmino ();

    // Lees een spel in vanuit tekstbestand invoernaam.
    // Controleer daarbij
    // * of het bestand wel te openen is,
    // * of hoogte en breedte binnen de grenzen vallen,
    // * of er genoeg stenen zijn om te verdelen over de twee spelers en om
    //   het spel met een steen te starten,
    // * en of positie van startsteen daadwerkelijk op het bord ligt.
    // Retourneer:
    // * true, als aan alle voorwaarden is voldaan
    // * false, als niet aan alle voorwaarden is voldaan
    // Post:
    // * als aan alle voorwaarden is voldaan, zijn de stenen verdeeld over
    //   het bord, de twee handen en de pot
    bool leesIn (const char* invoernaam);

    // Controleer of we een eindstand hebben bereikt:
    // * een van de twee spelers heeft geen stenen meer
    // * of er is geen mogelijke zet voor de speler die aan de beurt is,
    //   terwijl de pot leeg is
    bool eindstand ();

    // Druk de hele stand (bord, stenen in de twee handen en de pot,
    // speler aan beurt) af op het scherm.
    void drukAf ();

    // Bepaal alle mogelijke zetten voor de speler die aan de beurt is:
    // combinaties (steen, rotatie, rij, kolom) die aansluiten bij de stenen
    // op het bord.
    // Retourneer: een vector met al deze zetten
    vector<Zet> bepaalMogelijkeZetten ();

    // Haal voor de speler die op dit moment aan de beurt is,
    // een steen uit de pot.
    // Controleer eerst
    // * of de pot niet leeg is
    // * of de speler deze beurt al geen steen uit de pot gehaald heeft
    // * of de speler al geen bruikbare stenen in de hand heeft
    // Retourneer:
    // * het nummer van de uit de pot gehaalde steen, als het kan en mag
    //   om een steen uit de pot te halen
    // * -1, als het niet kan of mag
    // Post:
    // * als het kan en mag om een steen uit de pot te halen,
    //   is deze steen verhuisd van de pot naar de hand van de speler
    //   die aan de beurt is
    // * de speler aan beurt is nog niet veranderd
    int haalSteenUitPot ();

    // Wissel de speler die aan de beurt is.
    // Pre:
    // * de speler aan beurt is klaar met zijn beurt
    // Post:
    // * de speler aan beurt is de andere speler geworden
    void wisselSpeler ();

    // Doe een zet voor de speler die aan de beurt is:
    // steen i met rotatie r op vakje (rij,kolom), samen in parameter zet.
    // Controleer eerst of het wel een geldige zet is:
    // * i en r zijn geldige nummers voor de steen en de rotatie
    // * (rij,kolom) is een geldig, vrij vakje, grenzend aan de stenen die
    //   al op het bord liggen
    // * steen i is in de hand van de speler die aan de beurt is
    // * steen i past met rotatie r op vakje (rij,kolom)
    // Retourneer:
    // * true, als dit een geldige zet is
    // * false, als dit geen geldige zet is.
    // Post:
    // * als het een geldige zet is, is de zet uitgevoerd:
    //   - de steen ligt op het bord, en is uit de hand van de speler die
    //     aan de beurt was,
    //   - de speler aan beurt is gewisseld,
    // * als het geen geldige zet is, is de stand niet veranderd.
    bool doeZet (Zet zet);

    // Bepaal alle goede zetten voor de speler die aan de beurt is:
    // mogelijke zetten met zoveel mogelijk buurvakjes.
    // Retourneer:
    // * een vector met al deze zetten
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

    // Genereer een spel met bepaalde parameters en random getallen tussen
    // minGetal en maxGetal (inclusief) op de stenen.
    // Controleer eerst
    // * of hoogte en breedte binnen de grenzen vallen,
    // * of er genoeg stenen zijn om te verdelen over de twee spelers en om
    //   het spel met een steen te starten,
    // * en of positie (rij0,kolom0) van startsteen daadwerkelijk op het bord
    //   ligt.
    // Retourneer:
    // * true, als alle parameters OK zijn
    // * false, als minstens 1 parameter niet OK is
    // Post:
    // * als alle parameters OK zijn
    //   - is een eventueel al in het object aanwezige stand overschreven
    //   - zijn de nieuwe stenen gegenereerd en verdeeld over het bord,
    //     de twee handen en de pot
    // * als niet alle parameters OK zijn, is een eventueel in het object
    //   aanwezige stand niet veranderd
    bool genereerRandomSpel (int hoogte0, int breedte0,
           int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
           int minGetal, int maxGetal);

  private:
    // TODO: uw eigen memberfuncties en -variabelen

      // alvast enkele membervariabelen
    pair<int,int> bord[MaxDimensie][MaxDimensie];
              // in een pair kunnen we een steennummer en een rotatie opslaan
    int hoogte, breedte,  // van het bord
        nrStenen,     // totaal aantal stenen in het spel
        aanBeurt;     // speler die aan de beurt is


};

#endif

