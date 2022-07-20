// Definitie van klasse Haven

#ifndef HavenHVar // voorkom dat dit bestand meerdere keren
#define HavenHVar // ge-include wordt

#include "constantes.h"
#include <vector>
#include <utility> // lijkt soms nodig voor pair
using namespace std;

class Haven
{
public:
  // Default constructor
  Haven();
  ~Haven();

  bool leesInInstantie(const char *invoerNaam);

  void genereerInstantie(int b, int N, int ogLengte, int bgLengte,
                         int s, double c, int K, double ogOpKosten, double bgOpKosten);

  void drukAfInstantie();

  double bepaalMinKostenRec();

  double bepaalMinKostenTD();

  double bepaalMinKostenBU(vector<pair<int, int>> &plaatsing);

  void drukAfPlaatsing(vector<pair<int, int>> &plaatsing);

  void initTable();
  void deleteTable();

  std::vector<std::vector<std::vector<int>>> initRijenBU();
  std::vector<std::vector<int>> initKranenCombinatiesBU(int rijenSize, std::vector<std::pair<int, std::vector<std::vector<int>>>> kranenCombinatiesN);
  void berekenPlaatsingBU(std::vector<std::pair<int, int>> &plaatsing, std::vector<std::vector<int>> besteRijCombinatie, std::vector<int> besteKraanCombinatie);

  double getKosten(int k, int i, int j);

  double getRijKosten(int i, int j);
  double getKraanKosten(int k, int i, int j);
  double totaalKosten(int k, int j, std::vector<std::vector<int>> &rijen, int itk, int itj, double oudeKosten);

private:
  int b;
  int N;
  std::vector<int> l;
  int s;
  double c;
  int K;
  std::vector<std::vector<double>> o;
  double ***table;
  int mode;
  bool geldig;
};

#endif
