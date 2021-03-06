#include "graph.h"
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1024,768);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(1024,768);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(834,768);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCBLEU);


    //on ajoute en bas la boite � boutons
    m_top_box.add_child(m_ButtonBox);
    m_ButtonBox.set_dim(190,768);
    m_ButtonBox.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_ButtonBox.set_bg_color(BLEUCLAIR);
    m_ButtonBox.set_moveable();

    ///Premier bouton
    m_ButtonBox.add_child(m_Button1);
    m_Button1.set_frame(20,65,140,50);
    m_Button1.set_bg_color(VERT);

    m_Button1.add_child(m_sauvgarder);
    m_sauvgarder.set_message("Sauvegarder");

    ///Deuxieme bouton
    m_ButtonBox.add_child(m_Button3);
    m_Button3.set_frame(20,165,140,50);
    m_Button3.set_bg_color(ORANGE);

    m_Button3.add_child(m_recuperer);
    m_recuperer.set_message("Charger");

    ///Troisieme Bouton
    m_ButtonBox.add_child(m_Button4);
    m_Button4.set_frame(20,265,140,50);
    m_Button4.set_bg_color(GRISCLAIR);

    m_Button4.add_child(m_AddVertex);
    m_AddVertex.set_message("Ajouter un sommet");

    ///Cinqui�me bouton
    m_ButtonBox.add_child(m_Button5);
    m_Button5.set_frame(20,365,140,50);
    m_Button5.set_bg_color(GRISCLAIR);

    m_Button5.add_child(m_AddEdge);
    m_AddEdge.set_message("Ajouter un arc");

    ///Quatrieme bouton
    m_ButtonBox.add_child(m_Button6);
    m_Button6.set_frame(20,465,140,50);
    m_Button6.set_bg_color(ROUGE);

    m_Button6.add_child(m_effacer_sommet);
    m_effacer_sommet.set_message("Effacer Sommet");

    ///Sixieme bouton
    m_ButtonBox.add_child(m_Button2);
    m_Button2.set_frame(20,565,140,50);
    m_Button2.set_bg_color(ROUGE);

    m_Button2.add_child(m_effacer_arc);
    m_effacer_arc.set_message("Effacer Arc");

}

/*
/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example1()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 100, 400, 100, "homme.jpg");// num de sommet , poid, | , --
    add_interfaced_vertex(1,  10, 200, 300, "viande.png");
    add_interfaced_vertex(2,  50, 400, 300, "fruits.png");
    add_interfaced_vertex(3,   0, 600, 300, "poisson.png");
    add_interfaced_vertex(4, 100, 200, 500, "cereales.jpg");
    add_interfaced_vertex(5,   0, 400, 500, "pesticides.png");
    add_interfaced_vertex(6,   0, 600, 500, "plancton.png");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 2, 0, 50.0);// indice de l'ar�te, de 1 � 2 , poids 50
    add_interfaced_edge(1, 1, 0, 50.0);
    add_interfaced_edge(2, 3, 0, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 5, 1, 25.0);
    add_interfaced_edge(5, 5, 2, 25.0);
    add_interfaced_edge(6, 6, 3, 0.0);
}


void Graph::make_example2()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 500, 400,   0, "sun.png");// num de sommet , poid, | , --
    add_interfaced_vertex(1, 300, 200, 200, "eau.png");
    add_interfaced_vertex(2, 150, 600, 200, "arbre.png");
    add_interfaced_vertex(3,  60, 400, 400, "sol.png");
    add_interfaced_vertex(4, 100,  25, 400, "Potable.png");
    add_interfaced_vertex(5, 100, 700, 400, "O2.png");
    add_interfaced_vertex(6,  80, 200, 600, "cereales.jpg");
    add_interfaced_vertex(7,  80, 600, 600, "animaux.png");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 0, 50.0);// indice de l'ar�te, de 1 � 2 , poids 50
    add_interfaced_edge(1, 2, 0, 50.0);
    add_interfaced_edge(2, 2, 1, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 5, 2, 25.0);
    add_interfaced_edge(5, 3, 1, 25.0);
    add_interfaced_edge(6, 3, 2, 0.0);
    add_interfaced_edge(7, 6, 3, 100.0);
    add_interfaced_edge(8, 7, 6, 20.0);
}

void Graph::make_example3()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    //add_interfaced_vertex(0, 500, 200,   0, "pic.png");// num de sommet , poid, | , --
    add_interfaced_vertex(0, 300, 325, 0, "fox.png");
    add_interfaced_vertex(1, 150, 25, 200, "chevreuil.png");
    add_interfaced_vertex(2,  60, 225, 200, "mesange.jpg");
    add_interfaced_vertex(3, 100, 425, 200, "ecureuil.png");
    add_interfaced_vertex(4, 100, 25, 400, "chene.jpg");
    add_interfaced_vertex(5,  80, 225, 400, "noisettes.png");
    add_interfaced_vertex(6,  80, 425, 400, "orchis.png");
    add_interfaced_vertex(7,  80, 25, 600, "sun.png");
    add_interfaced_vertex(8,  80, 225, 600, "CO2.png");
    add_interfaced_vertex(9,  80, 425, 600, "eau.png");
    add_interfaced_vertex(10,  80, 625, 600, "mineraux.png");
    add_interfaced_vertex(11,  80, 625, 100, "ver.png");
    add_interfaced_vertex(12,  80, 625, 300, "Champignons.png");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 0, 50.0);// indice de l'ar�te, de 1 � 2 , poids 50
    add_interfaced_edge(1, 2, 0, 50.0);
    add_interfaced_edge(2, 3, 0, 75.0);
    add_interfaced_edge(3, 0,11, 25.0);
    add_interfaced_edge(4, 4, 1, 25.0);
    add_interfaced_edge(5, 6, 1, 25.0);
    add_interfaced_edge(6, 4, 2, 0.0);
    add_interfaced_edge(7, 5, 3, 100.0);
    add_interfaced_edge(8, 6, 2, 20.0);
    add_interfaced_edge(9, 7, 4, 50.0);
    add_interfaced_edge(10, 7, 5, 50.0);
    add_interfaced_edge(11, 7, 6, 50.0);
    add_interfaced_edge(12, 8, 4, 50.0);
    add_interfaced_edge(13, 8, 5, 50.0);
    add_interfaced_edge(14, 8, 6, 50.0);
    add_interfaced_edge(15, 9, 4, 50.0);
    add_interfaced_edge(16, 9, 5, 50.0);
    add_interfaced_edge(17, 9, 6, 50.0);
    add_interfaced_edge(18, 10, 4, 50.0);
    add_interfaced_edge(19, 10, 5, 50.0);
    add_interfaced_edge(20, 10, 6, 50.0);
    add_interfaced_edge(21, 11, 12, 50.0);
    add_interfaced_edge(22, 12, 10, 50.0);
    add_interfaced_edge(23, 10, 9, 50.0);
    add_interfaced_edge(24, 3, 11, 50.0);
    add_interfaced_edge(25, 6, 11, 50.0);
}
*/


/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    m_interface->m_top_box.update();

    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    ///Utilisation des bouton pour d�clencher un �v�nement

    if (m_interface->m_Button1.clicked())
    {
        Sauvegarde();
    }

    if (m_interface->m_Button3.clicked())
    {
        ChargerGraphe();
    }

    if(m_interface->m_Button2.clicked())
    {
        std::cout << std::endl << "  Quel arc voulez-vous supprimer ?" << std::endl;

        do{
        std::cout << "--Quel est l'indice de l'arc a supprimer ?--" <<  std::endl;
        std::cin >> num;
        }while(num > m_edges.size() - 1);
        test_remove_edge(num);

    }

    if (m_interface->m_Button6.clicked())
    {
        std::cout << std::endl << "  Quel sommet voulez-vous supprimer ? " << std::endl;

        do{
        std::cout << "--Quel est l'indice du sommet a supprimer ?--" <<  std::endl;
        std::cin >> num;
        }while(num > m_vertices.size()- 1);

        for (auto& elem : m_edges)
        {
            if((elem.second.m_to==num)||(elem.second.m_from==num))
            {
                test_remove_edge(elem.first);
            }
        }
        test_remove_vertex(num);

    }

    if (m_interface->m_Button4.clicked())
    {
        std::cout <<std::endl << "--------Creation d'un nouveau sommet :--------" <<std::endl;
        std::cout<<"Prendre un indice qui n'existe pas."<<std::endl<<std::endl;
        do
        {
        std::cout << "--Que sera l'indice de votre sommet ?--" <<  std::endl;
        std::cin >> indice;
        }while(indice < m_vertices.size() );

        do
        {
        std::cout << "--Que sera la valeur du sommet ?--" << std::endl;
        std::cin >> valeur;
        }while (valeur > 100 || valeur <0);

        char Choix;
        std::cout <<  "--Quel sommet aimerez-vous ajoutez ?--"<<std::endl;
         do
        {
        std::cout << "0. Homme" <<std::endl << "1. Viande" << std::endl<< "2. Poisson" <<std::endl<< "3. Fruit" <<std::endl<< "4. Cereales"<< std::endl<< "5. Animaux" << std::endl<< "6. Oxyg�ne" <<std::endl<< "7. Soleil" <<std::endl<< "8. Eau" <<std::endl<< "9. Foret" <<std::endl;
        std::cin >> Choix;
        }
        while(Choix < '0'  || Choix > '9');
            {
            switch(Choix)
                {
                case '0' :
                    PicName = "homme.pnj";
                    break;

                case '1':
                    PicName = "viande.png";
                    break;

                case '2':
                    PicName = "poisson.png";
                    break;

                case '3':
                    PicName = "fruits.png";
                    break;

                case '4':
                    PicName = "cereales.jpg";
                    break;

                case '5':
                    PicName = "animaux.png";
                    break;

                case '6':
                    PicName = "O2.png";
                    break;

                case '7':
                    PicName = "sun.png";
                    break;

                case '8':
                    PicName = "eau.png";
                    break;

                case '9':
                    PicName = "arbre.png";
                    break;

                default:
                    std::cout << "Ce sommet n'existe pas!"<< std::endl;
                }
            }

        add_interfaced_vertex(indice,valeur, 300,300, PicName);
    }


    if(m_interface->m_Button5.clicked())
    {
        std::cout << std::endl << "--------Creation d'un nouvel arc :--------- " << std::endl;
         do
        {
        std::cout << "--Choisir l'indice de votre arc:-- " << std::endl;
        std::cin >> indiceEdge;
        }while(indiceEdge < m_edges.size() );
        do {
        std::cout << "--Indiquez le sommet influence :--" << std::endl;
        std::cin >> indicesommet1;}while(indicesommet1>m_edges.size());
        do{
        std::cout << "--Indiquez le sommet influent :-- " << std::endl;
        std::cin >> indicesommet2 ;}while (indicesommet2>m_edges.size());
        do{
        std::cout << "--Indiquez la valeure de votre arc :-- " << std::endl;
        std::cin >> poids;}while (valeur > 100 || valeur <0);
        add_interfaced_edge(indiceEdge, indicesommet1, indicesommet2, poids );
        std::cout << "L'arc a ete cree avec succes !" <<std::endl;
    }
}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}


/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    /// OOOPS ! Prendre en compte l'arc ajout� dans la topologie du graphe !

    m_edges[idx].m_from = id_vert1;



    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::test_remove_vertex(int eadx)
{
    Vertex &remed=m_vertices.at(eadx);

    if (m_interface && remed.m_interface)

    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_box );
    }
}

void Graph::test_remove_edge(int eidx)
{

    /// r�f�rence vers le Edge � enlever

    Edge &remed=m_edges.at(eidx);
    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;


    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :

        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */

        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }


    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );


    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;
}

void Graph::Sauvegarde()
{
   std::string NomFichier, nom;
    std::cout << "Entrez le nom du fichier a sauvegarder : " << std::endl;
    std::cin >> nom;
    NomFichier = nom+".txt";

    std::ofstream fichier(NomFichier, std::ios::out | std::ios::trunc);
    {// ouverture du fichier en ecriture

    fichier << m_vertices.size();
    fichier << std::endl;
    fichier << m_edges.size();
    fichier <<  std::endl;

   for (unsigned int i=0; i<m_vertices.size();i++)
    {
        fichier<<i<<" " << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " << m_vertices[i].m_interface->m_top_box.get_posy()<<" " << m_vertices[i].m_interface->m_img.get_pic_name() <<std::endl;
        fichier << std::endl;
    }

    for (unsigned int i=0; i<m_edges.size(); i++)
    {
        //ecriture numero du sommet, poids, sommet incident, sommet d'arriv�
        fichier<<i<<" "<<m_edges[i].m_from<<" "<< m_edges[i].m_to<<" "<<m_edges[i].m_weight;
        fichier<<std::endl;
    }

    fichier.close();
    }
}

void Graph::ChargerGraphe()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    std::string nom,NomFichier;
    std::cout << std::endl << "-----Quel graphe voulez vous recuperer ? :-----" << std::endl;
    ///Montrer la liste des fichiers
    std::cin >> nom;
    NomFichier=nom+".txt";

    std::ifstream fichier (NomFichier,ios::in);
    //ouverture du fichier en lecture
    if(fichier)
    {
        int K = m_edges.size();

        for (int i=0; i<m_vertices.size(); i++){
           test_remove_vertex(i);
        }
        for (int i=0; i<K; i++){
           test_remove_edge(i);
        }

        m_edges.clear();
        m_vertices.clear();

        int nbSommet, nbArrete;
        int indice1,coord1,coord2;
        double Poids1;
        std::string nomfich;

        ///Nombre de Sommet/Arrete
        fichier >>nbSommet>>nbArrete;

        ///Charge Sommet

        for (int i=0;i<nbSommet;i++)
        {
            fichier >>indice1>>Poids1>>coord1>>coord2>>nomfich;
            //std::cout << "ok"<<std::endl;
            add_interfaced_vertex(indice1,Poids1,coord1,coord2,nomfich);

        }
        ///Charge Arc
        int indice2,Sommet1,Sommet2;
        double Poids2;
        //std::cout << "ko" << std::endl;
        for (int i=0;i<nbArrete;i++)
        {
            fichier >>indice2>>Sommet1>>Sommet2>>Poids2;
            add_interfaced_edge(indice2,Sommet1,Sommet2, Poids2);
        }

        fichier.close();
    }
    else
    {
        std::cout<< "Impossible d'ouvrir le fichier"<<std::endl;
    }
}

void Graph::ChargerGrapheBase(std::string name)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    std::ifstream fichier (name,ios::in);

    ///ouverture du fichier en lecture
    if(fichier){
        int nbSommet, nbArrete;
        int indice1,coord1,coord2;
        double Poids1;
        std::string nomfich;

        ///Nombre de Sommet/Arrete
        fichier >>nbSommet>>nbArrete;

        ///Charge Sommet
        for (int i=0;i<nbSommet;i++){
            fichier >>indice1>>Poids1>>coord1>>coord2>>nomfich;
            add_interfaced_vertex(indice1,Poids1,coord1,coord2,nomfich);

        }

        ///Charge Arc
        int indice2,Sommet1,Sommet2;
        double Poids2;

        for (int i=0;i<nbArrete;i++){
            fichier >>indice2>>Sommet1>>Sommet2>>Poids2;
            add_interfaced_edge(indice2,Sommet1,Sommet2, Poids2);
        }

        fichier.close();
    }
    else{
        std::cout<< "Impossible d'ouvrir le fichier"<<std::endl;
    }
}
void Graph::Interaction()
{

    for(int i=0;i<m_vertices.size();i++)
    {
        /// Reproduction
        m_vertices[i].m_value = 1.0002*m_vertices[i].m_value;
        /// Deces
       // m_vertices[i].m
        for(int j=0; j< m_edges.size();j++)
        {
            /// Influence des autres especes sur la premiere
            if(m_edges[j].m_to == i)
            {

                //m_vertices[i].m_value = m_vertices[i].m_value + 1,00002*m_vertices[i].m_value*(1-m_vertices[i].m_value/(m_edges[j].m_weight*m_vertices[m_edges[j].m_from].m_value));


                m_vertices[i].m_value = m_vertices[i].m_value + m_vertices[m_edges[j].m_from].m_value*0.0001*m_edges[j].m_weight;
                m_vertices[m_edges[j].m_from].m_value = m_vertices[m_edges[j].m_from].m_value -m_vertices[m_edges[j].m_from].m_value*0.0001*m_edges[j].m_weight;
            }


        }

    }
}
