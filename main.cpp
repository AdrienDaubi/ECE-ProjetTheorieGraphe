#include "grman/grman.h"
#include <iostream>

#include "graph.h"


int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    Graph g;
    int choix;
    std::cout << "Quels graphe voulez vous ouvrir ? : " << std::endl;
    std::cin >> choix;

    while(choix<1 || choix>4)
        {
            std::cout<<"vous avez saisi une action non valide, veuillez recommencer SVP :";
            std::cin>>choix;
        }
    switch(choix)
        {
            case 1 :g.make_example1();
                    break;

            case 2 :g.make_example2();
                    //break;

           // case 3 :j.achat_carte(toutes_cartes_dispo);
                   // break;
        }
    //g.make_example();

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


