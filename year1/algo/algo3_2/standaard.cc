// Implementatie van standaard functies.

#include <iostream>
#include <cstdlib>  // voor rand
#include "standaard.h"
using namespace std;

//*************************************************************************

bool integerInBereik (const char *variabele, int waarde,
                      int minWaarde, int maxWaarde)
{
  if (waarde>=minWaarde && waarde<=maxWaarde)
    return true;
  else
  { cout << variabele << "=" << waarde << ", maar moet in [" << minWaarde
         << "," << maxWaarde << "] liggen." << endl;
    return false;
  }

}  // integerInBereik

//*************************************************************************

bool integerInBereik (int waarde, int minWaarde, int maxWaarde)
{
  if (waarde>=minWaarde && waarde<=maxWaarde)
    return true;
  else
    return false;

}  // integerInBereik

//*************************************************************************

int randomGetal (int min, int max)
{ int bereik,
      r;

  bereik = max - min + 1;

  r = ((rand())%bereik) + min;
  return r;

}  // randomGetal

//****************************************************************************

double randomDouble (double min, double max)
{ int bereik,
      r;
  double fr;

  bereik = (int)(100*(max - min) + 1);
  
  r = (rand())%bereik;  // 0 <= r <= 100*(max-min)
  fr = min + 0.01*r;
  
  return fr;

}  // randomDouble

//*************************************************************************

// Bereken kwadraat van x
double square (double x)
{
  return x*x;

}  // intKwadraat

//*************************************************************************

int intSquare (int x)
{
  return x*x;

}  // intSquare

void errorMsg(const char * s)
{
  std::cout << s << std::endl;
  exit(1);
}
