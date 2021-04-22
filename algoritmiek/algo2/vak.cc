#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "vak.h"

int Vak::nrVakCount = 0;

Vak::Vak(std::string vakNaam, int nrDocent, int nrTracks)
{
  this->vakNaam   = vakNaam; 
  this->nrDocent  = nrDocent;
  this->nrTracks  = nrTracks;
  this->nrVak     = Vak::nrVakCount;
  Vak::nrVakCount++;
}
Vak::~Vak()
{

}
void Vak::voegTrackToe(int track)
{
  this->tracks.push_back(track);
}

std::vector<int> Vak::krijgTracks()
{
  return this->tracks;
}

std::string Vak::krijgVakNaam()
{
  return this->vakNaam;
}

int Vak::krijgNrDocent()
{
  return this->nrDocent;
}

int Vak::krijgNrVak()
{
  return this->nrVak;
}