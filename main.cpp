#include "grman/grman.h"
#include <iostream>

#include "graph.h"
#include <allegro.h>
#include <png.h>
#include <loadpng.h>
#include <jpgalleg.h>

int menu ()
{
    int menu=0;
    BITMAP *Menu;
    BITMAP *Buffer;
    BITMAP *Souris;

    Buffer=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(Buffer);

    Menu=load_bitmap("pics/Menu2.bmp", NULL);
    Souris=load_bitmap("pics/ecolo.bmp", NULL);

    if (Menu==NULL){
    allegro_message("Erreur de chargement de l'image");
    return 0;
    }

    while ( menu!=1 && menu!=2 && menu!=3 && menu!=4 && menu!=5 ){

        blit(Menu, Buffer, 0,0,0,0, SCREEN_W, SCREEN_H);

        if (mouse_b & 1 && mouse_x>401 && mouse_x<600 && mouse_y<441 && mouse_y>354){   /// GRAPHE 1
        menu=1;
        }

        if (mouse_b & 1 && mouse_x>402 && mouse_x<602 && mouse_y<566 && mouse_y>475){       /// GRAPHE 2
        menu=2;
        }

        if (mouse_b & 1 && mouse_x>403 && mouse_x<602 && mouse_y<690 && mouse_y>603){  /// GRAPHE 3
        menu=3;
        }

        if (mouse_b & 1 && mouse_x>46 && mouse_x<245 && mouse_y<128 && mouse_y>42){   /// INSTRUCTIONS
        menu=4;
        }

        if (mouse_b & 1 && mouse_x>769 && mouse_x<969 && mouse_y<121 && mouse_y>33){   /// QUITTER
        menu=5;
        }

        //textprintf_ex(Buffer,font,mouse_x,mouse_y,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);

        masked_blit(Souris, Buffer, 0, 0, mouse_x, mouse_y, Souris->w, Souris->h);
        blit(Buffer, screen, 0,0,0,0,SCREEN_W,SCREEN_H);
        clear(Buffer);
        //show_mouse(screen);
    }

return menu;
}

int instructions ()
{
    int instructions= 1;

    BITMAP *Infos;
    BITMAP *Buffer;
    BITMAP *Souris;

    Buffer=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(Buffer);

    Infos=load_bitmap("pics/ocean2.bmp", NULL);
    Souris=load_bitmap("pics/ecolo.bmp", NULL);

    while(instructions != 0){
        blit(Infos, Buffer, 0,0,0,0, SCREEN_W, SCREEN_H);

        if (mouse_b & 1 && mouse_x>388 && mouse_x<624 && mouse_y<724 && mouse_y>639){
            instructions=0;    /// Retour
        }

        //textprintf_ex(Buffer,font,mouse_x,mouse_y,makecol(0,255,0),makecol(0,0,0),"%4d %4d",mouse_x,mouse_y);

        masked_blit(Souris, Buffer, 0, 0, mouse_x, mouse_y, Souris->w, Souris->h);
        blit(Buffer, screen, 0,0,0,0,SCREEN_W,SCREEN_H);
        clear(Buffer);
        //show_mouse(screen);
    }

return instructions;
}

int main()
{
    int value = 1;
    grman::init();
    value = menu();
    menu();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    while (value != 5){     /// menu == 5 --> QUITTER

        if (value == 0){   /// MENU
            menu();
        }

        if (value == 1){    /// GRAPHE 1

            Graph g;

            g.ChargerGrapheBase("Graphe1.txt");

            while ( !key[KEY_ESC] ){
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g.Interaction();
                g.update();

                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            value = menu();
        }

        if (value == 2){   /// GRAPHE 2

            Graph g;

            g.ChargerGrapheBase("Graphe2.txt");

            while ( !key[KEY_ESC] ){
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g.Interaction();
                g.update();

                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            value = menu();
        }

        if (value == 3){   /// GRAPHE 3

            Graph g;

            g.ChargerGrapheBase("Graphe3.txt");

            while ( !key[KEY_ESC] ){
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g.Interaction();
                g.update();

                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            value = menu();
        }

        if (value == 4){   /// INSTRUCTIONS
            instructions();
            value = instructions();
            value = menu();
        }
    }
return 0;
}
END_OF_MAIN();


