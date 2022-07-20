// Hoofdprogramma voor oplossing voor programmeeropdracht 3 van Algoritmiek,
// voorjaar 2022: Containerhaven.
//
// Biedt de gebruiker een menustructuur om
// * een instantie (haven, containers, kranen) in te lezen, en vervolgens:
//   - rechtstreeks recursief een goedkoopste plaatsing van containers door
//     kranen te bepalen
//   - met top-down dynamisch programmeren een goedkoopste plaatsing van
//     containers door kranen te bepalen
//   - met bottom-up dynamisch programmeren een goedkoopste plaatsing van
//     containers door kranen te bepalen
// * experimenten te doen
//
// 25 mei 2022
// Yvo Hu            s2962802
// Lohrenz Amatkasan s3118533


#include <iostream>
#include <vector>
#include <ctime>  // voor clock() en clock_t
#include "standaard.h"
#include "haven.h"
using namespace std;
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

//*************************************************************************

// Schrijf het menu voor een toernooi op het scherm,
// en vraag een keuze van de gebruiker.
// Parameter activiteit geeft aan waarmee je bezig bent.
// Dat wordt vermeld bij optie 4 (Stoppen).
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu (const char *activiteit)
{ int keuze;

  cout << endl;
  cout << "1. Minimale kosten bepalen (rechtstreeks recursief)"
       << endl;
  cout << "2. Minimale kosten bepalen (top-down DP)"
       << endl;
  cout << "3. Minimale kosten met bijbehorende plaatsing containers bepalen (bottom-up DP)"
       << endl;
  cout << "4. Stoppen met " << activiteit << endl;
  cout << endl;
  cout << "Maak een keuze: ";
  cin >> keuze;

  return keuze;

}  // keuzeUitMenu

//*************************************************************************

// Bied de gebruiker een menu om voor instantie h1 op drie verschillende
// manieren de minimale kosten te bepalen om de containers door de kranen
// in rijen in de haven te plaatsen:
// * rechtstreeks recursief
// * top-down dynamisch programmeren
// * bottom-up dynamisch programmeren
// Roep vervolgens h1 aan voor de gekozen manier.
// Herhaal dit totdat de gebruiker aangeeft te willen stoppen.
void menuVoorInstantie (Haven *h1)
{ int keuze;
  clock_t t1, t2;
  double kosten;
  vector <pair <int,int> > plaatsing;

  keuze = 0;
  while (keuze!=4)
  {
    keuze = keuzeUitMenu ("deze instantie");

    t1 = clock ();
    cout << endl;
    switch (keuze)
    { case 1: kosten = h1 -> bepaalMinKostenRec ();
              cout << "Minimale kosten zijn: " << kosten << endl;
              break;
      case 2: kosten = h1 -> bepaalMinKostenTD ();
              cout << "Minimale kosten zijn: " << kosten << endl;
              break;
      case 3: kosten = h1 -> bepaalMinKostenBU (plaatsing);
              cout << "Minimale kosten zijn: " << kosten << endl;
              h1 -> drukAfPlaatsing (plaatsing);
              break;
      case 4: break;
      default: cout << "Voer een goede keuze in!" << endl;
    }  // switch
    t2 = clock ();

    if (integerInBereik (keuze, 1, 3))
    { cout << endl;
      cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
        << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;
    }
  }  // while

}  // menuVoorInstantie

//*************************************************************************

// Voer de experimenten uit, zoals beschreven in de opdracht.
void doeExperimenten ()
{ int keuze;
  clock_t t1, t2;
  double kosten;
  vector <pair <int,int> > plaatsing;
 
  int N[9] = {4, 6, 8, 10, 12, 14, 16, 18, 20};
  int Nsize = sizeof(N)/ sizeof(N[0]);

  keuze = 0;
  while (keuze!=4)
  {
    Haven* h1 = new Haven ();

    cout << endl;
    cout << "Waarmee wilt u experimenten doen?" << endl;
    keuze = keuzeUitMenu ("de experimenten");

    for(int n = 0; n < Nsize; n++)
    {
      h1->genereerInstantie (100, N[n], 10, 50, 2, 1.0, N[n]/2, 1.0, 10.0);

      t1 = clock ();
      cout << endl;

      switch (keuze)
      { case 1: kosten = h1 -> bepaalMinKostenRec ();
                cout << "Minimale kosten zijn: " << kosten << endl;
                break;
        case 2: kosten = h1 -> bepaalMinKostenTD ();
                cout << "Minimale kosten zijn: " << kosten << endl;
                break;
        case 3: kosten = h1 -> bepaalMinKostenBU (plaatsing);
                cout << "Minimale kosten zijn: " << kosten << endl;
                h1 -> drukAfPlaatsing (plaatsing);
                break;
        case 4: break;
        default: cout << "Voer een goede keuze in!" << endl;
      }  // switch

      t2 = clock ();

      if (integerInBereik (keuze, 1, 3))
      { cout << endl;
        cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
        << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;
      }
 
    }
    delete h1;
  }  // while

}  // doeExperimenten

//*************************************************************************

void hoofdmenu ()
{ Haven *h1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
              // en weer weg te gooien
  int keuze;
  char invoernaam[MaxBestandsNaamLengte+1];

  do
  {
    cout << endl;
    cout << "1. Een nieuwe instantie inlezen" << endl;
    cout << "2. Experimenten uitvoeren" << endl;
    cout << "3. Stoppen" << endl;
    cout << endl;
    cout << "Maak een keuze: ";
    cin >> keuze;
    switch (keuze)
    { case 1: h1 = new Haven ();
              cout << "Geef de naam van het tekstbestand met de invoer: ";
              cin >> invoernaam;
              if (h1 -> leesInInstantie (invoernaam))
              { h1 -> drukAfInstantie ();
                menuVoorInstantie (h1);
              }
              delete h1;  // netjes opruimen
              break;
      case 2: doeExperimenten ();
              break;
      case 3: break;
      default: cout << endl;
               cout << "Voer een goede keuze in!" << endl;
    }

  } while (keuze!=3);

}  // hoofdmenu

//*************************************************************************
  
int main ()
{
  hoofdmenu ();

  return 0;

}
