// Definitie van klasse Rooster

#ifndef RoosterHVar  // voorkom dat dit bestand meerdere keren
#define RoosterHVar  // ge-include wordt

#include "constantes.h"

class Rooster
{ public:

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
    bool leesIn (const char* invoerNaam);

    // Druk de informatie van de instantie af op het scherm.
    void drukAf ();

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
    // TODO: uw eigen memberfuncties en -variabelen

    int nrDagen,       // aantal dagen in het rooster
        nrUrenPerDag,  // aantal uren per dag
        nrZalen,       // aantal beschikbare zalen
        nrDocenten,
        nrVakken;

      // Een datastructuur voor de docenten en hun beschikbaarheden.
      // En een datastructuur voor de vakken, met hun docenten en tracks.
};

#endif

