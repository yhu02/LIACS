// Definitie van klasse Beurs

#ifndef BeursHVar  // voorkom dat dit bestand meerdere keren
#define BeursHVar  // ge-include wordt

#include <vector>
#include "constantes.h"
using namespace std;

class Beurs
{ public:

    // Default constructor
    Beurs ();

    // Lees de invoerparameters en -waardes in uit tekstbestand invoernaam.
    // Het formaat van het bestand is als volgt:
    // * een regel met twee integers tw en n: respectievelijk het aantal dagen
    //   waarna je de balans opmaakt, en het aantal bedrijven
    // * een regel met een double p:  het provisiepercentage
    // * een regel met een double b0: het geldbedrag waarmee je start
    // * (tw+1) regels met elk n doubles: voor elke dag een regel met
    //   de koersen van de aandelen van de n bedrijven
    // * tw regels met een double: voor elke dag (TOT de laatste) het dan
    //   geldende rentepercentage.
    //
    // Retourneer:
    // * true als het lukt om het tekstbestand te openen, en de eerste
    //   twee getallen binnen de grenzen van de opdracht liggen
    // * false als het niet lukt om het tekstbestand te openen, of de eerste
    //   twee getallen niet beide binnen de grenzen van de opdracht liggen.
    // Pre:
    // * als het lukt om het tekstbestand te openen, en de eerste twee
    //   getallen binnen de grenzen van de opdracht liggen, liggen alle andere
    //   getallen in het bestand ook binnen de grenzen van de opdracht
    // Post:
    // * Als de returnwaarde true is, zijn alle ingelezen getallen
    //   opgeslagen in membervariabelen.
    bool leesIn (const char* invoernaam);

    // Druk, ter controle, de complete invoer af op het scherm.
    void drukAfInvoer ();

    // Bepaal met behulp van bottom-up dynamisch programmeren het maximale
    // bedrag dat de belegger aan het eind van dag tw in kas kan hebben
    // (op dat moment heeft hij geen aandelen meer).
    // De tabel met bedragen wordt iteratief ingevuld.
    // Pre:
    // * Er is een geldige instantie ingelezen.
    // Post:
    // * Parameter transacties bevat de transacties die nodig zijn om tot
    //   het maximale bedrag te komen.
    //   In het bijzonder: transacties[t] is een vector met de transacties
    //   die nodig zijn op dag t (de size van transacties is dus tw+1). Elke
    //   transactie is een pair bestaande uit een boolean en een integer i.
    //   De boolean is true als aandeel i gekocht moet worden, en false als
    //   aandeel i verkocht moet worden.
    // Retourneer:
    // * Het maximale bedrag op dag tw.
    double bepaalMaxBedragBU
             (vector <vector <pair <bool,int> > > &transacties);

    // Bepaal recursief het maximale bedrag dat de belegger aan het eind
    // van dag tw in kas kan hebben (op dat moment heeft hij geen aandelen
    // meer). Als parameter memo = true worden berekende deelresultaten
    // opgeslagen in tabel bedrag en hergebruikt. Als memo = false, worden
    // deelresultaten steeds opnieuw berekend.
    // De benodigde transacties om het maximale bedrag te behalen, hoeven
    // niet bepaald te worden.
    // Pre:
    // * Er is een geldige instantie ingelezen.
    // Retourneer:
    // * Het maximale bedrag op dag tw.
    double bepaalMaxBedragRec (bool memo);

    // Druk de transacties in parameter transacties (koop en verkoop
    // van aandelen) dag voor dag af op het scherm.
    // Bereken ook, uitgaande van startbedrag b0, het resulterende
    // eindbedrag, en druk dat af op het scherm.
    // Voor de betekenis van parameter transacties, zie functie
    // bepaalMaxBedragBU.
    // Nuttig als controle voor het antwoord van dynamisch programmeren.
    // Pre:
    // * Er is een geldige instantie ingelezen.
    // * De transacties in parameter transacties zijn geldige transacties
    //   bij deze instantie.
    void drukAfTransacties (vector <vector <pair <bool,int> > > transacties);

  private:
    // TODO: uw eigen memberfuncties en -variabelen

    int tw,    // tijdstip_wereldreis
        n;     // het aantal verschillende aandelen
    double provisie,  // provisie op transacties, in procenten.
           b0;  // bedrag op tijdstip 0

};

#endif

