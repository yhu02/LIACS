// Defintie van standaard functies.

#ifndef StandaardHVar  // om te voorkomen dat dit .h bestand meerdere keren
#define StandaardHVar  // wordt ge-include 

// Controleer of variabele met naam `variabele' een waarde `waarde' heeft
// die tussen (inclusief) minWaarde en maxWaarde in ligt.
// Zo nee, geef een passende foutmelding.
//
// Voorbeeld van aanroep:
//   if (integerInBereik ("teller", teller, 0, 1000))
//     ...
bool integerInBereik (const char *variabele, int waarde,
                      int minWaarde, int maxWaarde);

// Genereer een random geheel getal r waarvoor  min <= r <= max.
// Pre: min <= max;
int randomGetal (int min, int max);

// Genereer een random double getal fr waarvoor  min <= fr <= max.
// Pre: min <= max;
//   (max-min) <= INT_MAX / 100
double randomDouble (double min, double max);

// Schrijf een double met een bepaalde precisie naar het scherm
// (al dan niet met endln erachter).
void schrijfDouble (double getal, int precisie, bool metEndln);

#endif

