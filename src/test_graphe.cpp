#include "partie.hpp"
#include "melangeur.hpp"

#include <iostream>
#include <cstdlib>
#include <sstream>

int main() {

  //n'hésitez pas à ajouter vos propres tests

  //test de la fonction voisine
  std::cout << "=== positions voisines de {0,3} ===" << std::endl ;

  std::cout << "attendu : {-1,3}" << std::endl ;
  std::cout << voisine({0,3}, 0) << std::endl ;
  std::cout << "attendu : {0,4}" << std::endl ;
  std::cout << voisine({0,3}, 1) << std::endl ;
  std::cout << "attendu : {1,3}" << std::endl ;
  std::cout << voisine({0,3}, 2) << std::endl ;
  std::cout << "attendu : {0,2}" << std::endl ;
  std::cout << voisine({0,3}, 3) << std::endl ;

  //création d'un plateau
  Plateau p ;
  //spécification de la dimension du plateau
  p.ajouter({0,0}, {9,9}) ;

  //placement d'un arbre
  //le dernier paramètre est le numéro du joueur (inutile pour un arbre)
  p.amenager({1,1}, Amenagement::ARBRE, 0) ;

  //placement d'un producteur
  p.amenager({3,7}, Amenagement::PRODUCTEUR_AVOCAT, 0) ;

  //placement d'une boutique
  p.amenager({8,2}, Amenagement::BOUTIQUE_ECHALOTE, 1) ;
  //dessin du plateau dans la console
  std::cout << p << std::endl ;

  if(p.tuiles.find(voisine({1,1},0)) == p.tuiles.end()){
    std::cout << "pas endore amménagé" << std::endl;
  } else {
    std::cout << "la voisine de haut de {1,1}, attendu : {0,1}" << std::endl ;
    std::cout << p.tuiles.find(voisine({1,1},0))->first << std::endl << std::endl;
    std::cout << "la voisine de droite de {1,1}, attendu : {1,2}" << std::endl ;
    std::cout << p.tuiles.find(voisine({1,1},1))->first << std::endl << std::endl;
    std::cout << "la voisine du bas de {1,1}, attendu : {2,1}" << std::endl ;
    std::cout << p.tuiles.find(voisine({1,1},2))->first << std::endl << std::endl;
    std::cout << "la voisine de gauche de {1,1}, attendu : {1,0}" << std::endl ;
    std::cout << p.tuiles.find(voisine({1,1},3))->first << std::endl << std::endl;
    std::cout << "l'amménagement à la case {0,1}, attendu : VIDE" << std::endl ;
    std::cout << nom_amenagement(p.tuiles.find(voisine({1,1},0))->second.amenagement) << std::endl <<std::endl;
    std::cout << "la valeur de joueur pour la tuile à la case {0,1}, attendu : -1" << std::endl ;
    std::cout << p.tuiles.find(voisine({1,1},0))->second.joueur << std::endl << std::endl;
  }

  std::cout << "Un arc existe-t-il entre {1,1} et sa voisine du haut {0,1} ?" << std::endl ; 
  std::cout << "{1,1} est aménagé donc attendu : 0 (false)" << std::endl ;
  std::cout << arc_existe({1,1},0,p.tuiles) << std::endl << std::endl;
  std::cout << "Un arc existe-t-il entre {2,1} et son voisin du haut {1,1} ?" << std::endl ;
  std::cout << "{2,1} n'est pas aménagé donc attendu : 1 (true)" << std::endl ;
  std::cout << arc_existe({2,1},0,p.tuiles) << std::endl << std::endl;

  //dessin du plateau dans une image
  //il est également possible d'écrire un fichier png
  //pour que ça fonctionne, il faut avoir librsvg-2.0
#ifndef NO_GRAPHICS
  p.dessiner("plateau.svg", 100) ;
#endif
  
  return 0 ;
}
