// Hoofdprogramma voor oplossing voor tweede programmeeropdracht Algoritmiek,
// voorjaar 2021: Rooster.
//
// Biedt de gebruiker een menustructuur om
// * een instantie van Rooster in te lezen, en daarvoor roosters te maken.
//   (een geldig rooster, een zo kort mogelijk rooster, een gretig rooster)
//
// Naam student 1, naam student 2

#include <iostream>
#include <ctime>  // voor clock() en clock_t
#include "constantes.h"
#include "standaard.h"
#include "rooster.h"

const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

//*************************************************************************

// Schrijf het menu voor een instantie op het scherm,
// en vraag een keuze van de gebruiker.
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu ()
{ int keuze;

  std::cout << std::endl;
  std::cout << "1. Een rooster bepalen (backtracking)" << std::endl;
  std::cout << "2. Een zo kort mogelijk rooster bepalen (backtracking)" << std::endl;
  std::cout << "3. Een rooster bepalen (gretig)" << std::endl;
  std::cout << "4. Stoppen met deze instantie" << std::endl;
  std::cout << std::endl;
  std::cout << "Maak een keuze: ";
  std::cin >> keuze;

  return keuze;

}  // keuzeUitMenu

//*************************************************************************

// Bied de gebruiker een menu om roosters te bepalen voor instantie r1:
// een rooster (met backtracking), een zo kort mogelijk rooster (met
// backtracking), en een rooster (met een gretig algoritme).
// Roep vervolgens r1 aan voor het gekozen soort rooster.
// Herhaal dit totdat de gebruiker aangeeft te willen stoppen.
void menuVoorInstantie (Rooster *r1)
{ int keuze,
      rooster[MaxNrTijdsloten][MaxNrZalen];
  long long aantalDeelroosters;  // aantal deelroosters dat we hebben
                                 // opgebouwd
  clock_t t1, t2;
  bool br;  // lukt het om een (minimaal) rooster te bepalen
            // met backtracking?

  keuze = 0;
  while (keuze!=4)
  {
    keuze = keuzeUitMenu ();

    switch (keuze)
    { case 1:
      case 2: t1 = clock ();
              std::cout << std::endl;
              if (keuze==1)
              { std::cout << "bepaalRooster" << std::endl;
                br = r1->bepaalRooster (rooster, aantalDeelroosters);
              }
              else
              { std::cout << "bepaalMinRooster" << std::endl;
                br = r1->bepaalMinRooster (rooster, aantalDeelroosters);
              }

              if (br)
                r1 -> drukAfRooster (rooster);
              else
              { std::cout << std::endl;
                std::cout << "Helaas, we vonden geen geldig rooster";
              }
              t2 = clock ();
              std::cout << std::endl;
              std::cout << "We hebben hiervoor " << aantalDeelroosters
                   << " deelroosters opgebouwd." << std::endl;
              std::cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
               << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << std::endl;
              break;
      case 3: std::cout << std::endl;
              std::cout << "bepaalRoosterGretig" << std::endl;
              r1 -> bepaalRoosterGretig (rooster);
              r1 -> drukAfRooster (rooster);
              break;
      case 4: break;
      default: std::cout << std::endl;
               std::cout << "Voer een goede keuze in!" << std::endl;
    }  // switch

  }  // while

}  // menuVoorInstantie

//*************************************************************************

void hoofdmenu ()
{ Rooster *r1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
                // en weer weg te gooien
  int keuze;
  char invoernaam[MaxBestandsNaamLengte+1];

  do
  {
    std::cout << std::endl;
    std::cout << "1. Een instantie inlezen en verwerken" << std::endl;
    std::cout << "2. Stoppen" << std::endl;
    std::cout << std::endl;
    std::cout << "Maak een keuze: ";
    std::cin >> keuze;
    switch (keuze)
    { case 1: r1 = new Rooster ();
              std::cout << "Geef de naam van het tekstbestand met de invoer: ";
              std::cin >> invoernaam;
              if (r1 -> leesIn (invoernaam))
              { r1 -> drukAf ();
                menuVoorInstantie (r1);
              }
              delete r1;  // netjes opruimen
              break;
      case 2: break;
      default: std::cout << std::endl;
               std::cout << "Voer een goede keuze in!" << std::endl;
    }

  } while (keuze!=2);

}  // hoofdmenu

//*************************************************************************
  
int main ()
{
  hoofdmenu ();

  return 0;

}
