#include "constantes.hpp"
#include "plateau.hpp"
#include "console_pad.hpp"

#include <stdexcept>
#include <sstream>

#include<vector>
#include<utility>
#include <iomanip>

///////_____________________ALGORITHME NAIF ______________________________________________//////////////////////////

//fonction qui effectue un parcours en profondeur 

void explorer(const Position & pos,std::map<Position,Tuile> & tuiles, std::vector<std::vector<bool>>& matrice_visite ){
  //matrice_visite[pos.first][pos.second]=true ;

  //la liste des voisins du noeud actuel 
  std::vector<Position> voisins = {
    {voisine(pos,0)},
    {voisine(pos,1)},
    {voisine(pos,2)},
    {voisine(pos,3)}
  };
  //  for(auto& t : plateau.tuiles) { pour iteret sur les elem du vecteur  => foreach
  for (std::size_t i = 0; i < voisins.size(); ++i){
    if(arc_existe(pos, (int) i, tuiles) && matrice_visite[voisins[i].first][voisins[i].second]==false){
      matrice_visite[voisins[i].first][voisins[i].second]=true;
      //si le voisin existe et n'a pas été deja visité on lance un parcours en profondeur depuis ce voisin
      explorer(voisins[i],tuiles,matrice_visite);
    }
  }
}

//lancer le parcour a partir d'une case dans le platreau

void parcours_en_profondeur(const Position& pos, std::map<Position, Tuile>& tuiles, std::vector<std::vector<bool>>& visite) {
    // Initialiser un ensemble pour garder une trace des cases visitées
    //std::vector<std::vector<bool>> visite(tuiles.size(), std::vector<bool>(tuiles.size(), false));  
    for (std::size_t i = 0; i < visite.size(); ++i) {
        for (std::size_t j = 0; j < visite[i].size(); ++j) {
          visite[i][j] = false;
        }
    } 
    visite[pos.first][pos.second]=true; 
    // Lancer le parcours en profondeur DFS depuis la position donnée
    explorer(pos, tuiles, visite);
}

//pour verifier si la case est bien
bool case_accessible(Plateau& p, std::vector<std::vector<bool>>& visited) {
    for (int l = 0; static_cast<std::size_t> (l) < p.tuiles.size(); l++) {
        for (int c = 0; static_cast<std::size_t>(c) < p.tuiles.size(); c++) {
            Position pos = {static_cast<int>(l), static_cast<int>(c)};
            if (type(p.tuiles.find(pos)->second.amenagement) != Amenagement::VIDE) {
                // Si une case aménagée n'est pas accessible, retourner faux
                if (!visited[l][c]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// fonction qui retourne les dimension d'un plateau
std::pair<int, int> dimensions(Plateau& p){
  if (p.tuiles.empty()) {
      return {0, 0}; // Retourner les dimensions nulles si le plateau est vide
  }

  // on initialise les dimensions avec les premières coordonnées de tuile
  int min_x = p.tuiles.begin()->first.first;
  int max_x = p.tuiles.begin()->first.first;
  int min_y = p.tuiles.begin()->first.second;
  int max_y = p.tuiles.begin()->first.second;

  // on parcourt toutes les positions des tuiles pour trouver les dimensions
  for (const auto& [pos, _] : p.tuiles) {
      min_x = std::min(min_x, pos.first);
      max_x = std::max(max_x, pos.first);
      min_y = std::min(min_y, pos.second);
      max_y = std::max(max_y, pos.second);
  }

  // on calcule la largeur et la hauteur du plateau
  int largeur = max_x - min_x + 1;
  int hauteur = max_y - min_y + 1;

  return {largeur, hauteur};
}



static void placer_routes_naive(Plateau& p) {

  std::pair<int, int> dim = dimensions(p);

   for (auto& tuile : p.tuiles) {
        // Si la tuile est vide
        if (tuile.second.amenagement == Amenagement::VIDE) {
            // placer un arbre sur cette tuile
            tuile.second.amenagement = Amenagement::ARBRE;

            // au moment où l'arbre est aménagé, cherchez une autre case non aménagée ;
            bool accessible = true;
            for (auto& tuile_vide: p.tuiles) {
                if (tuile_vide.second.amenagement == Amenagement::VIDE) {
                    // Lancer un parcours en profondeur depuis cette  tuile trouvée
                    std::vector<std::vector<bool>> visite(dim.first, std::vector<bool>(dim.second, false)); 
                    parcours_en_profondeur(tuile_vide.first, p.tuiles, visite);

                    // Vérifier si toutes les cases sont accessibles
                    // si une case n'a pas été visité alors elle n'est pas accéssible
                    for (const auto& ligne : visite) {
                        for (bool valeur : ligne) {
                            if(valeur==false){
                              accessible=false;
                              break;
                            }
                        }
                    } 

                    break;
                }
            }

            // Retirer l'arbre  placé sur cette tuile
            tuile.second.amenagement = Amenagement::VIDE;

            //  placer une route permanente si obligé 
            if (!accessible) {
                tuile.second.amenagement = Amenagement::ROUTE;
            }
        }
    }
}

///////_____________________ALGORITHME ELABORE ______________________________________________//////////////////////////

// fonction qui calcule la remontée d'une tuile se trouvant à la position pos 
// et modifie la case correspondante dans le tableau remontee
void calcul_remontee(const Position & pos, std::map<Position,Tuile> & tuiles, std::vector<std::vector<int>>& profondeur, std::vector<std::vector<int>>& remontee){
  int x=pos.first;
  int y=pos.second;
  remontee[x][y]=profondeur[x][y];

  std::vector<Position> voisins = {
    {voisine(pos,0)},
    {voisine(pos,1)},
    {voisine(pos,2)},
    {voisine(pos,3)}
  };

  int profondeur_tuile=profondeur[x][y];

    for (std::size_t i = 0; i < voisins.size(); ++i){
      // on vérifie l'arc vers chaque voisin de la tuile et on modifie sa remontée selon le cas
      if(arc_existe(pos,(int) i, tuiles) && !(construit(tuiles.find(voisins[i])->second.amenagement))){

        if(profondeur[voisins[i].first][voisins[i].second]==profondeur_tuile+1){
          //si l'arc est utilisé 
          remontee[x][y]=std::min(remontee[x][y],remontee[voisins[i].first][voisins[i].second]);
        } else {
          //si l'arc est ignorée
          remontee[x][y]=std::min(remontee[x][y],profondeur[voisins[i].first][voisins[i].second]);
        }  

          //si l'arc est une impasse on ne fait rien  
      }     
    }  
}


// matrice_visite pour déterminer si la case voisine a déja était visitée, l'arc est donc ignorée
// remontee[i][j] est utilisée pour stocker la valeur de la remontée de la tuile à la position (i,j)
// profondeur[i][j] est utilisée pour stocker la valeur de la profondeur de la tuile à la position (i,j)
// utilisee[i][j] stocke le nombre d'arc utilisée dont l'origine est la tuile à la position (i,j)
// compteur_amenagee compte pour chaque case aménagée le nombre de cases pouvant l'atteindre
void explorer_elabore(const Position & pos,std::map<Position,Tuile> & tuiles, std::vector<std::vector<bool>>& matrice_visite, std::vector<std::vector<int>>& remontee, std::vector<std::vector<int>>& profondeur , std::vector<std::vector<int>>& utilisee, std::vector<std::vector<int>>& compteur_amenage ){
  std::vector<Position> voisins = {
    {voisine(pos,0)},
    {voisine(pos,1)},
    {voisine(pos,2)},
    {voisine(pos,3)}
  };
  for (std::size_t i = 0; i < voisins.size(); ++i){

    if(arc_existe(pos, (int) i, tuiles) && matrice_visite[voisins[i].first][voisins[i].second]==false && !(construit(tuiles.find(voisins[i])->second.amenagement))){

      //l'arc est utilisée
      matrice_visite[voisins[i].first][voisins[i].second]=true;
      profondeur[voisins[i].first][voisins[i].second]=(profondeur[pos.first][pos.second])+1;
      utilisee[pos.first][pos.second]++;
      explorer_elabore(voisins[i],tuiles,matrice_visite,remontee,profondeur,utilisee,compteur_amenage);

    } else if(arc_existe(pos, (int) i, tuiles) && matrice_visite[voisins[i].first][voisins[i].second]==true){
      //l'arc est ignorée

    } 
    if(arc_existe(pos, (int) i, tuiles) && construit(tuiles.find(voisins[i])->second.amenagement)){
      //si l'arc est une impasse
      compteur_amenage[voisins[i].first][voisins[i].second]++;
      matrice_visite[voisins[i].first][voisins[i].second]=true;
    }
  }

  calcul_remontee(pos,tuiles,profondeur,remontee);
}

void parcours_en_profondeur_elabore(const Position& pos, std::map<Position, Tuile>& tuiles, std::vector<std::vector<bool>>& visite,std::vector<std::vector<int>>& remontee, std::vector<std::vector<int>>& profondeur ,std::vector<std::vector<int>>& utilisee, std::vector<std::vector<int>>& compteur_amenage) {
  for (std::size_t j = 0; j < profondeur.size();j++){
    for (std::size_t k = 0; k < profondeur[j].size();k++){
      profondeur[j][k]=0;
      remontee[j][k]=0;
    }
  }
   
    for (std::size_t i = 0; i < visite.size(); ++i) {
        for (std::size_t j = 0; j < visite[i].size(); ++j) {
          visite[i][j] = false;
        }
    } 
    visite[pos.first][pos.second]=true; 

    // Lancer le parcours en profondeur depuis la position donnée
    explorer_elabore(pos, tuiles, visite,remontee,profondeur,utilisee,compteur_amenage);
}

//fonction d'affichage de tableau pour vérifier le calcul des remontées et des profondeurs
void afficherTableau(const std::vector<std::vector<int>>& tableau) {
    // on détermine la largeur maximale pour chaque colonne pour un affichage avec des colonnes alignées
    std::vector<int> largeurs(tableau[0].size(), 0);
    for (const auto& ligne : tableau) {
        for (size_t i = 0; i < ligne.size(); ++i) {
            int largeur_nombre = std::to_string(ligne[i]).size();
            if (largeur_nombre > largeurs[i]) {
                largeurs[i] = largeur_nombre;
            }
        }
    }

    // on affiche le tableau 
    for (const auto& ligne : tableau) {
        std::cout << "+";
        for (size_t i = 0; i < ligne.size(); ++i) {
            std::cout << std::string(largeurs[i] + 1, '-');
            std::cout << "+";
        }
        std::cout << "\n|";
        for (size_t i = 0; i < ligne.size(); ++i) {
            std::cout << std::setw(largeurs[i]) << std::right << ligne[i] << " |";
        }
        std::cout << "\n";
    }

    // on affiche la ligne inférieure de la grille
    std::cout << "+";
    for (size_t i = 0; i < tableau[0].size(); ++i) {
        std::cout << std::string(largeurs[i] + 1, '-');
        std::cout << "+";
    }
    std::cout << "\n";
}

void placer_routes(Plateau  & p ){

  std::pair<int, int> dim = dimensions(p);

  // On initialise des matrices pour stocker les informations dont on aura besoin pour déterminer les cases devenant des routes 
  std::vector<std::vector<bool>> visite(dim.first, std::vector<bool>(dim.second, false));
  std::vector<std::vector<int>> profondeur(dim.first, std::vector<int>(dim.second, 0));
  std::vector<std::vector<int>> remontee(dim.first, std::vector<int>(dim.second, 0));
  std::vector<std::vector<int>> utilisee(dim.first, std::vector<int>(dim.second, 0));
  std::vector<std::vector<int>> compteur_amenage(dim.first, std::vector<int>(dim.second, 0));

  Position depart;
  for (auto& tuile : p.tuiles) {
      // Chercher une case vide pour démarrer le parcours la tuile est vide
      if (tuile.second.amenagement == Amenagement::VIDE) {
        depart=tuile.first;
        break;
      }
  }

  // on lance le parcours qui calcule en meme temps les profondeurs des cases et les remontées
  parcours_en_profondeur_elabore(depart,p.tuiles,visite,remontee,profondeur,utilisee,compteur_amenage);


  // on parcours les tuiles pour déterminer les cases devenant des routes
  for (auto& tuile : p.tuiles) {
        std::vector<Position> voisins = {
          {voisine(tuile.first,0)},
          {voisine(tuile.first,1)},
          {voisine(tuile.first,2)},
          {voisine(tuile.first,3)}
        };

      bool route=false;

      if (tuile.first.first!=depart.first || tuile.first.second!=depart.second) {
        // la case n'est pas la case de départ
        for(std::size_t i = 0; i < voisins.size(); i++){

          if(arc_existe(tuile.first,(int) i, p.tuiles) && construit(p.tuiles.find(voisins[i])->second.amenagement)){
            // CAS 1 
            // si la case est la seule à pouvoir accéder à une case aménagée
            if(compteur_amenage[voisins[i].first][voisins[i].second]==1){
              route=true;
            }

          } else if((arc_existe(tuile.first,(int) i, p.tuiles)) && profondeur[voisins[i].first][voisins[i].second]==(profondeur[tuile.first.first][tuile.first.second]+1) && remontee[voisins[i].first][voisins[i].second]>=profondeur[tuile.first.first][tuile.first.second]){
            // CAS 2
            // elle n'est pas la case de départ et elle est à l'origine d'un arc u -> v utilisé tel que R(v)>=P(u)
            route=true;
          }
        }          

      }

      // CAS 3
      // la case est la case de départ et elle est à l'origine de plusieurs arcs utilisés.
      if ((tuile.first.first==depart.first && tuile.first.second==depart.second) && utilisee[tuile.first.first][tuile.first.second]>1) {
        route=true;
      }

      if(route){
        tuile.second.amenagement = Amenagement::ROUTE;
      }

  }

// Pour vérifier les valeurs des remontées et des profondeurs:

  /* 
    // Afficher le contenu du tableau profondeur
    std::cout << "profondeur" << std::endl;
    afficherTableau(profondeur);
    std::cout << std::endl;
    std::cout << std::endl;

    // Afficher le contenu du tableau remontee
    std::cout << "remontee" << std::endl;
    afficherTableau(remontee);
    std::cout << std::endl;
    std::cout << std::endl;
  */
  
}

void Plateau::ajouter(const Position& pos) {
  if(tuiles.find(pos) != tuiles.end()) {
    throw std::domain_error(
        "une tuile existe déjà à cet endroit"
        ) ;
  }

  Tuile& nouveau = tuiles[pos] ;
  nouveau.amenagement = Amenagement::VIDE ;
  nouveau.joueur = -1 ;
}

void Plateau::ajouter(const Position& pos_min, const Position& pos_max) {
  for(int i = pos_min.first; i <= pos_max.first; ++i) {
    for(int j = pos_min.second; j <= pos_max.second; ++j) {
      try {
        ajouter({i,j}) ;
      } catch(std::exception& e) {
        //erreur ignorée pour pouvoir simplement créer un plateau par blocs
      }
    }
  }
}

void Plateau::reserver(const Position& pos, int joueur) {
  Tuile& t = tuiles.at(pos) ;

  if(t.joueur >= 0) {
    throw std::invalid_argument(
        "cette tuile est déjà réservée"
        ) ;
  }

  t.joueur = joueur ;
  t.amenagement = Amenagement::RESERVEE ;
}

void Plateau::amenager(const Position& pos, Amenagement amenagement, int joueur) {
  Tuile& t = tuiles.at(pos) ;

/*
  if(t.amenagement != Amenagement::RESERVEE) {
    throw std::invalid_argument(
        "cette tuile n'a pas été réservée"
        ) ;
  }

  if(t.joueur != joueur) {
    throw std::invalid_argument(
        "cette tuile n'est pas réservée pour le joueur"
        ) ;
  }
*/

  t.joueur = joueur ;
  t.amenagement = amenagement ;
  placer_routes(*this) ;
}

bool arc_existe(const Position& pos,int v,std::map<Position,Tuile> & tuiles){
  bool arc=false;
  if (tuiles.find(voisine(pos,v))!=tuiles.end() && !construit(tuiles.find(pos)->second.amenagement)){
    arc=true;
  } 
  return arc; 
}

std::ostream& operator<<(std::ostream& out, const Plateau& plateau) {
  //pas de tuile pas d'affichage
  if(plateau.tuiles.size() == 0) return out ;

  //dimensions du plateau
  int lmin, lmax, cmin, cmax ;
  lmin = lmax = plateau.tuiles.begin()->first.first ;
  cmin = cmax = plateau.tuiles.begin()->first.second ;
  for(auto& t : plateau.tuiles) {
    lmin = std::min(lmin, t.first.first) ;
    lmax = std::max(lmax, t.first.first) ;
    cmin = std::min(cmin, t.first.second) ;
    cmax = std::max(cmax, t.first.second) ;
  }

  //creation d'un buffer de suffisamment de lignes
  int eliminees = plateau.eliminees.size() > 0 ? 4 : 0 ;
  ConsolePad pad((lmax - lmin + 1) * 2 + 1 + eliminees) ;

  //dessin d'une tuile
  auto dessiner = [&] (const Position& pos, const Tuile& tuile) {
    //ligne et colonne dans le pad
    unsigned int l = 2*(pos.first - lmin) ;
    unsigned int c = 4*(pos.second - cmin) ;

    //tuile vide
    pad.moveto(l, c) ;
    pad 
      << "+   +" << std::endl
      << "     " << std::endl
      << "+   +" ;

    //murs
    static constexpr const int positions_murs[] = {
      0, 1,
      1, 4,
      2, 1,
      1, 0
    } ;
    for(int i = 0; i < 4; ++i) {
      Position v = voisine(pos, i) ;
      if(plateau.tuiles.find(v) == plateau.tuiles.end()) {
        pad.moveto(l+positions_murs[2*i], c+positions_murs[2*i+1]) ;
        if(i%2 == 1) {
          pad << "|" ;
        } else {
          pad << "---" ;
        }
      }
    }

    //contenu possible des tuiles
    static constexpr const char* txt_amenagements[] = {
      "+ +", "+A+", "+B+", "+C+", "+D+", "+E+",
      "$ $", "$A$", "$B$", "$C$", "$D$", "$E$",
      "{:}",
      " # ",
      " @ ",
      "   "
    } ;
    static constexpr const char* couleurs_joueurs[] = {
#ifndef NO_COLOR
      "\x1B[38;5;38m", //bleu
      "\x1B[38;5;208m", //orange
      "\x1B[38;5;135m", //violet
      "\x1B[38;5;11m", //jaune
      "\x1B[38;5;206m" //rose
      //"\x1B[38;5;124m" //rouge
#else
      ""
#endif
    } ;
    static constexpr const int nb_couleurs = sizeof(couleurs_joueurs) / sizeof(char*) ;
  
    static constexpr const char* couleur_defaut = 
#ifndef NO_COLOR
      "\x1B[39m"
#else
      ""
#endif
    ;

    pad.moveto(l+1, c+1) ;
    Amenagement base = type(tuile.amenagement) ;
    if(base == Amenagement::BOUTIQUE || base == Amenagement::RESERVEE) {
      pad << couleurs_joueurs[tuile.joueur % nb_couleurs] ;
    }
    pad << txt_amenagements[(int) tuile.amenagement] ;
    if(base == Amenagement::BOUTIQUE || base == Amenagement::RESERVEE) {
      pad << couleur_defaut ;
    }
  } ;

  //dessin
  for(auto& t : plateau.tuiles) {
    dessiner(t.first, t.second) ;
  }

  //tuiles eliminees
  int nb_eliminees = plateau.eliminees.size() ;
  for(int i = 0; i < nb_eliminees; ++i) {
    dessiner({lmax + 2, cmin + i}, plateau.eliminees[i]) ;
  }


  out << pad.lines() ;
  return out ;
}


#ifndef NO_GRAPHICS
  #include <librsvg-2.0/librsvg/rsvg.h>
  #include <cairo/cairo.h>
  #include <cairo/cairo-svg.h>

void Plateau::dessiner(const std::string& cible, int unit, float margin, const std::string& style) {
  //pas de tuile pas d'affichage
  if(tuiles.size() == 0) return ;

  //dimensions du plateau
  int lmin, lmax, cmin, cmax ;
  lmin = lmax = tuiles.begin()->first.first ;
  cmin = cmax = tuiles.begin()->first.second ;
  for(auto& t : tuiles) {
    lmin = std::min(lmin, t.first.first) ;
    lmax = std::max(lmax, t.first.first) ;
    cmin = std::min(cmin, t.first.second) ;
    cmax = std::max(cmax, t.first.second) ;
  }

  //chargement du fichier de style svg
#ifndef FF_ASSETS
#define FF_ASSETS "../assets/"
#endif
  std::stringstream style_path ;
  style_path << FF_ASSETS << style ;
  GError* svg_error = nullptr ;
  RsvgHandle* style_handle = rsvg_handle_new_from_file(style_path.str().c_str(), &svg_error) ;
  if(svg_error) {
    throw std::invalid_argument("fichier de style introuvable") ;
  }

  //creation d'une surface cairo pour enregistrer le dessin
  cairo_surface_t* record = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, nullptr) ;

  //contexte de dessin
  cairo_t* cr = cairo_create(record) ;

  //dessin d'site
  auto dessiner = [&] (const Position& pos, const Tuile& tuile) {
    //fenetre a dessiner
    double x = pos.second ;
    double y = pos.first ;
    RsvgRectangle rect({x, y, 1, 1}) ;

    //id dans le fichier de style svg
    static constexpr const char* ids[] {
      "producteur",
      "producteur_avocat",
      "producteur_brocoli",
      "producteur_carotte",
      "producteur_datte",
      "producteur_echalote",
      "boutique",
      "boutique_avocat",
      "boutique_brocoli",
      "boutique_carotte",
      "boutique_datte",
      "boutique_echalote",
      "inutile",
      "route",
      "reservee",
      "vide"
    } ;

    std::stringstream svg_id ;
    svg_id << "#" << ids[(int) tuile.amenagement] ;
    Amenagement base = type(tuile.amenagement) ;
    if(base == Amenagement::BOUTIQUE || base == Amenagement::RESERVEE) {
      svg_id << "_" << tuile.joueur ;
    }

    //dessin
    GError* error = nullptr ;
    rsvg_handle_render_layer(style_handle, cr, svg_id.str().c_str(), &rect, &error) ;
    if(error) {
      std::cout << "got an error : " << svg_id.str() << " " << x << " " << y << std::endl ;
    }
  } ;

  //dessin
  for(auto& t : tuiles) {
    dessiner(t.first, t.second) ;
  }

  //tuiles eliminees
  int nb_eliminees = eliminees.size() ;
  for(int i = 0; i < nb_eliminees; ++i) {
    dessiner({lmax + 2, cmin + i}, eliminees[i]) ;
  }

  cairo_surface_flush(record) ;


  //finalisation du dessin
  cairo_destroy(cr) ;
  g_object_unref(style_handle) ;

  //export
  double x0, y0, largeur, hauteur ;
  cairo_recording_surface_ink_extents(record, &x0, &y0, &largeur, &hauteur) ;

  //il semble qu'il y ait un bug dans le calcul des largeur, hauteur, et origine
  largeur -= 2 ;
  hauteur -= 2 ;
  x0 += 1 ;
  y0 += 1 ;

  //ouvrir une surface pour l'export
  int umargin = margin * unit ;
  cairo_surface_t* img ;
  const std::string& extension = cible.substr(cible.find_last_of(".") + 1) ;
  if(extension == "svg") {
     img = cairo_svg_surface_create(
         cible.c_str(), unit*largeur + 2*umargin, unit*hauteur + 2*umargin
         ) ;
  } else if(extension == "png") {
    img = cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, unit*largeur + 2*umargin, unit*hauteur + 2*umargin
        ) ;
  } else {
    throw std::invalid_argument("seul l'export svg ou png est possible") ;
  }

  //creer un contexte pour l'export
  cr = cairo_create(img) ;
  cairo_translate(cr, umargin, umargin) ;
  cairo_scale(cr, unit, unit) ;

  //rejouer l'enregistrement
  cairo_set_source_surface(cr, record, -x0, -y0) ;
  cairo_paint(cr) ;
  cairo_show_page(cr) ;

  //finalisation de l'image
  cairo_surface_flush(img) ;

  //ecriture du png si besoin
  if(extension == "png") {
    cairo_surface_write_to_png(img, cible.c_str()) ;
  }

  //destruction du contexte
  cairo_destroy(cr) ;

  //destruction de la surface d'export
  cairo_surface_finish(img) ;
  cairo_surface_destroy(img) ;
  cairo_surface_finish(record) ;
  cairo_surface_destroy(record) ;
}

#endif
