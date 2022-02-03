// Definitie van klasse Vak

#ifndef VakHVar  // voorkom dat dit bestand meerdere keren
#define VakHVar  // ge-include wordt

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Vak
{
  public:  
    Vak(std::string vakNaam, int nrDocent, int nrTracks);
    ~Vak();
    void voegTrackToe(int track);
    std::vector<int> krijgTracks();
    std::string krijgVakNaam();
    int krijgNrDocent();
    int krijgNrVak();
    static int nrVakCount;


  private:
    std::string vakNaam;
    int nrVak;
    int nrDocent;
    int nrTracks;
    std::vector<int> tracks;
};
#endif
