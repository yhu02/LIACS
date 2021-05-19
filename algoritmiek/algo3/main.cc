// Hoofdprogramma voor oplossing voor derde programmeeropdracht Algoritmiek,
// voorjaar 2021: Beurs
//
// Bij deze opdracht gaat het erom om bij gegeven beurskoersen van
// een aantal aandelen en gegeven rentepercentages te berekenen
// wanneer je het best welke aandelen had kunnen kopen of verkopen
// om een zo hoog mogelijk eindbedrag te krijgen. Beperkingen zijn
// * dat je nooit een negatief bedrag in kas mag hebben,
// * en dat je op elk moment van elk bedrijf hoogstens 1 aandeel wilt
//   hebben; dit betekent dat je je aandelenbezit op elk moment kunt
//   coderen in een bitstring.
//
// Dit programma biedt de gebruiker een menustructuur om
// * een instantie van Beurs in te lezen, en daarvoor op drie verschillende
//   manieren een optimale oplossing te bepalen: het maximale bedrag dat
//   de belegger aan het eind in kas kan hebben. De drie manieren zijn:
//   - bottom-up dynamisch programmeren
//   - top-down dynamisch programmeren
//   - rechtstreeks recursief
//   Bij bottom-up dynamisch programmeren worden ook de voor het maximale
//   bedrag benodigde transacties bepaald.
//
// Naam student 1, naam student 2

#include <iostream>
#include <ctime>  // voor clock() en clock_t
#include "standaard.h"
#include "beurs.h"
using namespace std;
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

//*************************************************************************

// Schrijf het menu voor een instantie op het scherm,
// en vraag een keuze van de gebruiker.
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu ()
{ int keuze;

  cout << endl;
  cout << "1. Optimale oplossing bepalen (bottom-up DP)" << endl;
  cout << "2. Optimale oplossing bepalen (top-down DP)" << endl;
  cout << "3. Optimale oplossing bepalen (rechtstreeks recursief)" << endl;
  cout << "4. Stoppen met deze instantie" << endl;
  cout << endl;
  cout << "Maak een keuze: ";
  cin >> keuze;

  return keuze;

}  // keuzeUitMenu

//*************************************************************************

// Bied de gebruiker een menu om een optimale oplossing te berekenen
// voor instantie b1: het maximale bedrag dat de belegger aan het eind in kas
// kan hebben, met de daarvoor benodigde transacties.
// Roep vervolgens b1 aan voor deze optie.
// Pre: b1 bevat een geldige instantie
void menuVoorInstantie (Beurs *b1)
{ int keuze;
  clock_t t1, t2;
  vector <vector <pair <bool,int> > > transacties;
  double maxBedrag;

  keuze = 0;
  while (keuze!=4)
  {
    keuze = keuzeUitMenu ();

    t1 = clock ();
    switch (keuze)
    { case 1: maxBedrag = b1->bepaalMaxBedragBU (transacties);
              break;
      case 2: maxBedrag = b1->bepaalMaxBedragRec (true);
              break;
      case 3: maxBedrag = b1->bepaalMaxBedragRec (false);
              break;
      case 4: break;
      default: cout << endl;
               cout << "Voer een goede keuze in!" << endl;
    }  // switch
    t2 = clock ();

    if (keuze>=1 && keuze<=3)
    { cout << endl;
      cout << "maxBedrag = ";
      schrijfDouble (maxBedrag, 2, true);

      if (keuze==1)
        b1 -> drukAfTransacties (transacties);

      cout << endl;
      cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
           << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;
    }

  }  // while

}  // menuVoorInstantie

//*************************************************************************

void hoofdmenu ()
{ Beurs *b1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
              // en weer weg te gooien
  int keuze;
  char invoernaam[MaxBestandsNaamLengte+1];

  do
  {
    cout << endl;
    cout << "1. Een instantie inlezen en verwerken" << endl;
    cout << "2. Stoppen" << endl;
    cout << endl;
    cout << "Maak een keuze: ";
    cin >> keuze;
    switch (keuze)
    { case 1: b1 = new Beurs ();
              cout << endl;
              cout << "Geef de naam van het tekstbestand met de invoer: ";
              cin >> invoernaam;
              if (b1 -> leesIn (invoernaam))
              { b1 -> drukAfInvoer ();
                menuVoorInstantie (b1);
              }
              delete b1;  // netjes opruimen
              break;
      case 2: break;
      default: cout << endl;
               cout << "Voer een goede keuze in!" << endl;
    }

  } while (keuze!=2);

}  // hoofdmenu

//*************************************************************************
  
int main ()
{
  hoofdmenu ();

  return 0;

}
