// Definitie van klasse Rooster

#ifndef RoosterHVar  // voorkom dat dit bestand meerdere keren
#define RoosterHVar  // ge-include wordt

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "constantes.h"
#include "vak.h"

class Rooster
{ 
  public:
    // Default constructor
    Rooster ();

    // Lees een instantie in uit invoernaam.
    // Controleer daarbij of het bestand wel te openen is.
    // Pre:
    // * het aantal dagen, het aantal uren per dag, het aantal zalen,
    //   het aantal docenten, het aantal tijdsloten per docent,
    //   die tijdsloten zelf, het aantal vakken, de nummers van de docent
    //   van een vak, het aantal tracks en de tracks zelf die je inleest
    //   vallen binnen de gegeven grenzen (dat hoef je dus niet meer
    //   te controleren).
    // Retourneer:
    // * true, als het bestand te openen was.
    //   In dat geval is alle ingelezen informatie in het object opgeslagen.
    // * false, anders
    bool leesIn (const char* invoerNaam); //////////////////////////////////////////////////////////////

    // Druk de informatie van de instantie af op het scherm.
    void drukAf (); //////////////////////////////////////////////////////////////////////////////

    // Bepaal zo mogelijk een rooster voor de verschillende tracks,
    // rekening houdend met de beschikbaarheid van de docenten,
    // en de eisen aan de tracks.
    // Retourneer:
    // * true, als het lukt om een rooster te bepalen
    // * false, als het niet lukt om een rooster te bepalen
    // Post:
    // * als het lukt om een rooster te bepalen, bevat parameter `rooster'
    //   zo'n rooster. Dan geldt: rooster[s][z] =
    //   - het nummer van het vak dat op tijdslot s (in de week) en zaal z
    //     is ingeroosterd
    //   - -1 als er geen vak op tijdslot s en zaal z is ingeroosterd
    // * aantalDeelroosters is gelijk aan het aantal deelroosters dat we
    //   hebben gezien bij het bepalen van een rooster
    bool bepaalRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                            long long &aantalDeelroosters);

    // Bepaal zo mogelijk een rooster voor de verschillende tracks,
    // rekening houdend met de beschikbaarheid van de docenten,
    // en de eisen aan de tracks. Als er een rooster te vinden is,
    // bepaal dan een zo kort mogelijk rooster (een rooster dat zo vroeg
    // mogelijk (qua tijdslot in de week) klaar is).
    // Retourneer:
    // * true, als het lukt om een rooster te bepalen
    // * false, als het niet lukt om een rooster te bepalen
    // Post:
    // * als het lukt om een rooster te bepalen, bevat parameter `rooster'
    //   een zo kort mogelijk rooster. Dan geldt: rooster[s][z] =
    //   - het nummer van het vak dat op tijdslot s (in de week) en zaal z
    //     is ingeroosterd
    //   - -1 als er geen vak op tijdslot s en zaal z is ingeroosterd
    // * aantalDeelroosters is gelijk aan het aantal deelroosters dat we
    //   hebben gezien bij het bepalen van een rooster
    bool bepaalMinRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                            long long &aantalDeelroosters);

    // Druk parameter rooster overzichtelijk af op het scherm
    // (dag, tijd, zaal, naam van vak, nummer van docent).
    // Pre:
    // * er is al een instantie ingelezen, en parameter rooster
    //   bevat een rooster voor die instantie
    void drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen]);

    // Bepaal op een gretige manier een rooster voor de ingelezen vakken,
    // docenten en tracks. Als het niet lukt om aan alle eisen voor een
    // geldig rooster te voldoen, probeer er dan `zo goed mogelijk' aan
    // te voldoen.
    // Pre:
    // * het aantal vakken is <= het aantal combinaties (tijdslot,zaal),
    //   zodat daadwerkelijk alle vakken te geven zijn (los van onze andere,
    //   logische eisen aan een rooster)
    // Post:
    // * rooster bevat een rooster voor alle ingelezen vakken.
    void bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen]);

  private:
    //Conditie checks
    bool checkCondities(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z, std::vector<Vak> vakken_temp);
    int checkCondities2(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z, std::vector<Vak> vakken_temp);   //Voor gebruik in bepaalRoosterGretig
    bool conditie2(int rooster[MaxNrTijdsloten][MaxNrZalen], int s);
    bool conditie3(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z);
    bool conditie4(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z);
    bool conditie5(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z, std::vector<Vak> vakken_temp);
    bool conditie6(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z);

    //Uitzonderingen van conditie5
    bool uitzondering1(int t);
    bool uitzondering2();

    //Hulpfuncties van de uitzonderingen
    void uitzondering1Functie();

    //Hulpfunctie bepaalMinRooster
    bool bepaalMinRoosterRecursief(int rooster[MaxNrTijdsloten][MaxNrZalen],  
                  int minRooster[MaxNrTijdsloten][MaxNrZalen], int &minTijd, 
                  long long &aantalDeelroosters, std::vector<Vak> &vakken);  
                                    
    void bepaalMinRoosterFunctie(   int rooster[MaxNrTijdsloten][MaxNrZalen],
                                    int minRooster[MaxNrTijdsloten][MaxNrZalen],
                                    int &minTijd);

    //Hulpfunctie bepaalRooster
    bool bepaalRoosterRecursief(int rooster[MaxNrTijdsloten][MaxNrZalen],   
                  long long &aantalDeelroosters, std::vector<Vak> &vakken);            

    //Veranderd een nummer naar een string met de dag van de week
    std::string weekDag(int nrDag); 

    //Check of er zalen symmetrie is opgetreden
    //bool zalenSymmetrie(int s, int rooster[MaxNrTijdsloten][MaxNrZalen]);
    //void leegRoosterVakCombinaties();

    //Leest een getal in
    int leesGetal(std::ifstream& invoer);

    int nrDagen,          // Aantal dagen in het rooster
        nrUrenPerDag,     // Aantal uren per dag
        nrZalen,          // Aantal beschikbare zalen
        nrDocenten,       // Aantal docenten
        nrVakken,         // Aantal vakken
        maxKarakters;     // Aantal karakters van het vak met de langste naam

    std::vector<std::vector<std::vector<int>>> roosterVakCombinaties;    //Alle vakken combinaties op een gegeven tijdslot in het rooster die al gezien zijn

    std::vector<std::vector<int>> tracks;             // Aantal tracks met bijbehorende vakken
    std::vector<std::vector<int>> docenten;           // Aantal Docenten met bijbehorende beschikbare tijdsloten
    std::vector<Vak> vakken;                          // Aantal vakken met bijbehorende informatie
    std::vector<int> uitzondering1Tracks;             // Vector met de tracks die voldoen aan uitzondering1
};

#endif

