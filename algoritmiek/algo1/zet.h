// Definitie van klasse Zet

#ifndef ZetHVar  // voorkom dat dit bestand meerdere keren
#define ZetHVar  // ge-include wordt

class Zet
{ public:

    // Default constructor
    Zet ();

    // Geef de vier velden waardes die in combinatie niet voor kunnen
    // komen, als default voor `geen echte zet'.
    void setDefaultWaardes ();
    
    // Geef alle vier velden een waarde.
    // Controleer nog wel of het zinnige waardes zijn.
    void setWaardes (int i0, int r0, int rij0, int kolom0, int som0);

    // Vier getters:
    int getI ();
    int getR ();
    int getRij ();
    int getKolom ();
    int getSom ();

    // Druk de vier waardes van de zet af op het scherm
    void drukAf ();

  private:
    int i,  // nummer van de steen
        r,  // rotatie
        rij, kolom, // vakje op het bord
        som; //Aantal buurvakjes

};

#endif

