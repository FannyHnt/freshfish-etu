#include "partie.hpp"
#include "melangeur.hpp"

#include <iostream>
#include <cstdlib>
#include <sstream>

void plateau2(){

  std::cout << std::endl << "============================= PLATEAU 2 ==============================" << std::endl; 

  Plateau p ;
  p.ajouter({0,0}, {9,9}) ;


  p.amenager({3,2}, Amenagement::ARBRE, 0) ;
  p.amenager({3,6}, Amenagement::ARBRE, 0) ;
  p.amenager({3,7}, Amenagement::ARBRE, 0) ;
  p.amenager({4,1}, Amenagement::ARBRE, 0) ;
  p.amenager({5,3}, Amenagement::ARBRE, 0) ;
  p.amenager({5,5}, Amenagement::ARBRE, 0) ;
  p.amenager({6,5}, Amenagement::ARBRE, 0) ;
  p.amenager({8,6}, Amenagement::ARBRE, 0) ;
  p.amenager({9,5}, Amenagement::ARBRE, 0) ;
  p.amenager({9,6}, Amenagement::ARBRE, 0) ;

  p.amenager({2,8}, Amenagement::PRODUCTEUR_AVOCAT, 0) ;
  p.amenager({8,9}, Amenagement::PRODUCTEUR_BROCOLI, 0) ;
  p.amenager({4,2}, Amenagement::PRODUCTEUR_CAROTTE, 0) ;
  p.amenager({2,2}, Amenagement::PRODUCTEUR_DATTE, 0) ;
  p.amenager({9,4}, Amenagement::PRODUCTEUR_ECHALOTE, 0) ;

  p.amenager({2,1}, Amenagement::BOUTIQUE_CAROTTE, 2) ;
  p.amenager({2,4}, Amenagement::BOUTIQUE_CAROTTE, 4) ;
  p.amenager({2,5}, Amenagement::BOUTIQUE_BROCOLI, 0) ;
  p.amenager({2,7}, Amenagement::BOUTIQUE_CAROTTE, 1) ;

  p.amenager({3,4}, Amenagement::BOUTIQUE_ECHALOTE, 3) ;
  p.amenager({3,5}, Amenagement::BOUTIQUE_ECHALOTE, 4) ;
  p.amenager({3,8}, Amenagement::BOUTIQUE_AVOCAT, 1) ;

  p.amenager({4,3}, Amenagement::BOUTIQUE_DATTE, 1) ;

  p.amenager({5,1}, Amenagement::BOUTIQUE_BROCOLI, 3) ;

  p.amenager({8,2}, Amenagement::BOUTIQUE_CAROTTE, 0) ;
  p.amenager({8,8}, Amenagement::BOUTIQUE_DATTE, 0) ;

  p.amenager({9,2}, Amenagement::BOUTIQUE_AVOCAT, 4) ;
  p.amenager({9,9}, Amenagement::BOUTIQUE_DATTE, 2) ;
  
      std::cout << p << std::endl;
}

int main() {

  Plateau p ;
  p.ajouter({0,0}, {9,9}) ;

  Melangeur<Position> candidates ;
  for(auto& t : p.tuiles) {
    candidates.inserer(t.first) ;
  }

#ifndef NO_GRAPHICS
  int i = 1 ;
  p.dessiner("output/plateau_0.png") ;
#endif 

/*  while(candidates.taille() > 0) {
    Position pos = candidates.retirer() ;
    if(p.tuiles[pos].amenagement == Amenagement::VIDE) {
      p.reserver(pos, 0) ;
      p.amenager(pos, Amenagement::ARBRE, 0) ;
      std::cout << "============================================" << std::endl ;
      std::cout << p << std::endl ;

    }
    }*/

    std::vector<Position> positions = {
        {1, 3}, {7, 4}, {4, 4}, {5, 1}, {2, 1}, {0, 4},
        {6, 3}, {9, 3}, {8, 5}, {3, 4}, {4, 6}, {7, 3},
        {0, 3}, {9, 4}, {4, 5}, {5, 7}, {0, 0}, {3, 1},
        {0, 5}, {3, 3}, {5, 4}, {8, 9}, {7, 2}, {4, 0},
        {0, 6}, {4, 8}, {3, 0}, {5, 0}, {6, 4}, {5, 3},
        {1, 8}, {8, 7}, {9, 5}, {8, 8}, {5, 8}, {9, 0},
        {4, 7}, {9, 9}, {1, 6}, {7, 1}, {5, 6}, {1, 7},
        {9, 2}, {7, 5}, {0, 1}, {3, 5}, {7, 8}, {2, 7},
        {7, 7}, {9, 1}, {2, 8}
    };

    std::cout << "============================= PLATEAU 1 ==============================" << std::endl; 

    for(const auto& pos : positions){
      p.reserver(pos,0);
      p.amenager(pos,Amenagement::ARBRE,0);
#ifndef NO_GRAPHICS
      std::stringstream ss ;
      ss << "output/plateau_" << i << ".png" ;
      p.dessiner(ss.str()) ;
      ++i ;
#endif
      std::cout << "============================================" << std::endl;
      std::cout << p << std::endl;
    }
 

  plateau2();

  return 0 ;

} 



