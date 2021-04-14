#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "vak.h"

Vak::Vak(std::string vakNaam, int nrDocent, int nrTracks)
{
  this->vakNaam   = vakNaam; 
  this->nrDocent  = nrDocent;
  this-> nrTracks = nrTracks;
}

void Vak::voegTrackToe(int track)
{
  this->tracks.push_back(track);
}

std::vector<int> Vak::krijgTracks()
{
  return this->tracks;
}
