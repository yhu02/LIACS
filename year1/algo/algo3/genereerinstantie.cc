// Hulpprogramma bij `Beurs'
// programmeeropdracht 3 van Algoritmiek, voorjaar 2021.
//
// Genereert random beurskoersen en rentestanden tussen bepaalde,
// door de gebruiker op te geven grenzen.
// De gebruiker geeft ook waarden op voor
// * tw: het aantal dagen waarna de belegger de balans wil opmaken
// * n:  het aantal bedrijven waarvoor koersen worden gegenereerd
// * p:  het provisie percentage
// * b0: het geldbedrag waarmee de belegger start
// De opgegeven en gegenereerde waardes worden naar het scherm geschreven.
//
//   Rudy van Vliet, 2 mei 2021

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "standaard.h"
using namespace std;

const int MaxTw = 100;
const int MaxN = 8;
const double MinProvisie = 0.00;
const double MaxProvisie = 50.0; // maximaal 50%
const double MinB0 = 0.00;
const double MinKoers = 0.00;   // koers moet GROTER zijn dan MinKoers
const double MaxKoers = 100.00;
const double MinRente = -10.00;  // -10% rente dus, kan tegenwoordig
const double MaxRente = 100.00;  // 100% rente dus

//****************************************************************************

bool invoerOK (int tw, int n, double p, double b0, double mijnMinKoers,
               double mijnMaxKoers, double mijnMinRente, double mijnMaxRente)
// Controleer of de invoer van de gebruiker OK is.
{ bool OK;

  OK = true;

  if (!integerInBereik ("tw", tw, 1, MaxTw))
    OK = false;

  if (!integerInBereik ("n", n, 1, MaxN))
    OK = false;

  if ((p<MinProvisie) || (p>MaxProvisie))
  { cout << "Er moet gelden: " << MinProvisie << " <= p <= " << MaxProvisie
         << endl;
    OK = false;
  }

  if (b0 < MinB0)
  { cout << "Er moet gelden: b0 >= " << MinB0 << endl;
    OK = false;
  }
  
  if ((mijnMinKoers<=MinKoers) || (mijnMaxKoers<mijnMinKoers) ||
      (mijnMaxKoers>MaxKoers))
  { cout << "Er moet gelden: " << MinKoers
         << " < minimale koers <= maximale koers <= " << MaxKoers << endl;
    OK = false;
  }

  if ((mijnMinRente<MinRente) || (mijnMaxRente<mijnMinRente) ||
      (mijnMaxRente>MaxRente))
  { cout << "Er moet gelden: " << MinRente
         << " <= minimale rente <= maximale rente <= " << MaxRente << endl;
    OK = false;
  }

  return OK;

}  // invoerOK

//****************************************************************************

void genereerWaardes (int tw, int n, double p, double b0, double mijnMinKoers,
               double mijnMaxKoers, double mijnMinRente, double mijnMaxRente)
// Genereer waardes tussen de opgegeven grenzen, en schrijf alles naar
// het scherm.
{ int t, i;

  cout << tw << ' ' << n << endl;
  schrijfDouble (p, 2, true);
  schrijfDouble (b0, 2, true);

  for (t=0;t<=tw;t++)
    for (i=1;i<=n;i++)
    { schrijfDouble (randomDouble (mijnMinKoers, mijnMaxKoers), 2, false);
      if (i<n)
        cout << ' ';
      else
        cout << endl;
    }

  for (t=0;t<tw;t++)
    schrijfDouble (randomDouble (mijnMinRente, mijnMaxRente), 2, true);

}  // genereerWaardes

//****************************************************************************

int main ()
{ int tw, n;
  double p,
        b0,
        mijnMinKoers, mijnMaxKoers,
        mijnMinRente, mijnMaxRente;

  cout << endl;
  cout << "Geef een waarde voor tw: ";
  cin >> tw;
  cout << "Geef een waarde voor n: ";
  cin >> n;
  cout << "Geef een waarde voor p: ";
  cin >> p;
  cout << "Geef een waarde voor b0: ";
  cin >> b0;
  cout << "Geef een ondergrens voor de koersen: ";
  cin >> mijnMinKoers;
  cout << "Geef een bovengrens voor de koersen: ";
  cin >> mijnMaxKoers;
  cout << "Geef een ondergrens voor de rentepercentages: ";
  cin >> mijnMinRente;
  cout << "Geef een bovengrens voor de rentepercentages: ";
  cin >> mijnMaxRente;

  if (invoerOK (tw, n, p, b0,
                mijnMinKoers, mijnMaxKoers, mijnMinRente, mijnMaxRente))
    genereerWaardes (tw, n, p, b0,
                     mijnMinKoers, mijnMaxKoers, mijnMinRente, mijnMaxRente);

  return 0;

}
