// Hoofdprogramma voor testen van eerste programmeeropdracht Algoritmiek,
// voorjaar 2021: Aapje Omino.
//
// Dit programma verwacht drie command line argumenten:
// * een integer keuze die aangeeft wat voor soort test moet worden
//   uitgevoerd:
//   1. leesIn
//   2. bepaalMogelijkeZetten
//   3. besteScore
// * de naam van een tekstbestand met de instantie waarvoor de test moet
//   worden uitgevoerd
// * een integer die het juiste antwoord op de test voorstelt
// Voorbeeld van aanroep:
// ./AapjeOmino 3 ao1.txt 1
//
// De command line argumenten worden verwerkt, en de bijpassende test wordt
// uitgevoerd. Het programma wordt verlaten met een exitvalue 0 als de test
// goed ging, of een exitvalue 1 of hoger als er iets misging.
//
// Rudy van Vliet, 17 maart 2021


#include <iostream>
#include <ctime>  // voor clock() en clock_t
#include <cstring>
#include "zet.h"
#include "aapjeomino.h"
using namespace std;
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

//*************************************************************************

// Voer een test `keuze' uit voor het bestand `invoernaam',
// waarbij `juistAntwoord' het juiste antwoord zou moeten zijn.
// Retourneer:
// * 0, als de test uit te voeren is en de test goed uitpakt
//    (het antwoord op de test is inderdaad `juistAntwoord')
// * 1 of hoger (afhankelijk van hoeveel tests er feitelijk worden
//   uitgevoerd), anders
int hoofdmenu (int keuze, char *invoernaam, int juistAntwoord)
{ clock_t t1, t2;
  AapjeOmino *ao1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
                    // en weer weg te gooien
  int antwoord,      // antwoord dat we krijgen bij de test
      returnWaarde,  // 0 als test goed uitpakte
                     // 1 of hoger anders
      sc1;           // returnwaarde van besteScore
  vector<Zet> zetten;
  Zet besteZet;
  long long aantalStanden;

  t1 = clock ();

  ao1 = new AapjeOmino ();

  returnWaarde = 1;
   // als keuze geen bruikbare waarde heeft, retourneren we 1

  switch (keuze)
  { case 1: if (ao1 -> leesIn (invoernaam))
              antwoord = 7;  // komt overeen met true
            else
              antwoord = 0;  // komt overeen met false
            if (antwoord==juistAntwoord)
              returnWaarde = 0;
            else
              returnWaarde = 6;
            break;
    case 2: if (ao1 -> leesIn (invoernaam))
            { zetten = ao1 -> bepaalMogelijkeZetten ();
              if (((int)zetten.size()) == juistAntwoord)
                returnWaarde = 0;
              else
              { cout << endl;
                cout << "Aantal gevonden zetten is " << zetten.size()
                     << " maar moet zijn " << juistAntwoord
                     << endl;
                returnWaarde = 5;
              }
            }
            else  // gebeurt normaal gesproken niet
              returnWaarde = 4;
            break;
    case 3: if (ao1 -> leesIn (invoernaam))
            { sc1 = ao1 -> besteScore (besteZet, aantalStanden);
              if (sc1 == juistAntwoord)
                returnWaarde = 0;
              else
              { cout << endl;
                cout << "Berekende besteScore is " << sc1
                     << " maar moet zijn " << juistAntwoord
                     << endl;
                returnWaarde = 3;
              }
            }
            else  // gebeurt normaal gesproken niet
              returnWaarde = 2;
            break;
    default: cout << endl;
             cout << "Geen goede keuze!" << endl;
  }

  delete ao1;  // netjes opruimen

  t2 = clock ();
  cout << endl;
  cout << "Deze test kostte " << (t2-t1) << " clock ticks, ofwel "
       << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;

  return returnWaarde;

}  // hoofdmenu

//*************************************************************************
  
// Controleer of kar een cijfer is.
bool cijfer (char kar)
{
  return ((kar>='0') && (kar<='9'));

}  // cijfer

//*************************************************************************
  
// Zet een string str om in het bijbehorende gehele getal.
// Return:
// * het bijbehorende gehele getal (0 als str leeg is of alleen een - bevat)
// Pre:
// * argument str bestaat uit alleen cijfers, eventueel voorafgegaan
//   door een -.
int atoi (char *str)
{ int len,
      i,
      factor,  // +1 of -1 voor positieve / negatieve getallen
      getal;

  len = strlen (str);
  getal = 0;
  factor = 1;
  for (i=0;i<len;i++)
  { if (i==0 && str[i]=='-')
      factor = -1;
    else
    { if (cijfer(str[i]))
        getal = 10*getal + (str[i]-'0');
      else
      { cout << endl;
        cout << "str[" << i << "] = " << str[i] << ", wat geen cijfer is."
             << endl;
      }
    }
  }  // for

  return factor * getal;

}  // atoi

//*************************************************************************
  
int main (int argc, char *argv[])
{ int keuze,     // wat voor soort test gaan we uitvoeren
      juistAntwoord,  // wat is het juiste antwoord voor de test
      waarde;    // uitkomst van de test: 0 als het goed ging, 1 anders
  bool genoeg;   // genoeg parameters opgegeven

  genoeg = true;
  juistAntwoord = 0;  // default waarde

  if (argc>=3)
  { keuze = atoi (argv[1]);
    cout << "keuze = " << keuze << endl;
    switch (keuze)
    { case 1:  // leesIn
      case 2:  // bepaalMogelijkeZetten
      case 3:  // besteScore
              if (argc>=4)
                juistAntwoord = atoi (argv[3]);
              else
                genoeg = false;
              break;
      default: ;
    }

  }
  else
    genoeg = false;

  if (genoeg)
    waarde = hoofdmenu (keuze, argv[2], juistAntwoord);
  else
  { cout << endl;
    cout << "Niet genoeg argumenten opgegeven." << endl;
    waarde = 1;
  }

  if (waarde==0)
  { cout << endl;
    cout << "OK" << endl;
  }
  else
  { cout << endl;
    cout << "niet OK" << endl;
  }
  
  return waarde;

}