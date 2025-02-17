#ifndef FRESHFISH_PLATEAU_HPP
#define FRESHFISH_PLATEAU_HPP

#include "constantes.hpp"
#include "position.hpp"

#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

struct Tuile {
  Amenagement amenagement ;
  int joueur ;
} ;

struct Plateau {

  //ajout de cases une par une
  void ajouter(const Position& pos) ;

  //ajout d'un rectangle d'un coup
  void ajouter(const Position& pos_min, const Position& pos_max) ;

  //réservation d'un emplacemen pour un joueur
  void reserver(const Position& pos, int joueur) ;

  //placement d'un aménagement
  void amenager(const Position& pos, Amenagement amenagement, int joueur) ;

  //dessin en svg ou en png, nécessite librsvg-2.0
#ifndef NO_GRAPHICS
  void dessiner(const std::string& cible, int unit = 64, float margin = 0., const std::string& style="style.svg") ;
#endif
  
  //stockage des tuiles
  std::map<Position, Tuile> tuiles ;

  //stockage des aménagements éliminés (pour la fin du projet)
  std::vector<Tuile> eliminees ;
} ;

/**
  * @brief Verifie si il y a un arc entre une case et sa voisine
  * Une case vide ou une route peut accéder aux quatre cases voisines. 
  * Une case aménagée ne peut accéder à aucune autre case.
  * @param pos position de la case
  * @param v voisine vers laquelle on vérifie si il y a un arc 
  * @param p plateau
  */
bool arc_existe(const Position& pos,int v,std::map<Position,Tuile> & tuiles);
std::pair<int, int> dimensions(Plateau& p);



//affichage du plateau en console
std::ostream& operator<<(std::ostream& out, const Plateau& p) ;

#endif
