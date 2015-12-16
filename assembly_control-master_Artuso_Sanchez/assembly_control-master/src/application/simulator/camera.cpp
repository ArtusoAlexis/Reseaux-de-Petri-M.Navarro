#include <iostream>
#include <unistd.h>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "camera.h"

using namespace std;

enum EtatsCamera {											// Enumeration des differents etats de la camera
	DepartCamera,
	Reconnaissance,
	TypeDePiece
};

void tache_camera(struct signaux *sig) {

	EtatsCamera etat = DepartCamera;						// Etat initial de la camera

	while(1) {
		switch(etat) {
			case DepartCamera:
				cout << "DepartCamera" << endl;
				cout << endl;

				while(not sig-> ordre_CO.has_arrived());	// Attend d'avoir recu le signal "ordre_CO" venant du gestionnaire de stock
				sig-> ordre_CO.clear();						// Reinitialisation du signal "ordre_CO"

				etat = Reconnaissance;						// Changement d'etat vers Reconnaissance
				sleep(1);
			break;
				

			case Reconnaissance:
				cout << "Camera: Reconnaissance" << endl;
				cout << endl;

				sim.set_Reccam(true);						// Lancement de la reconnaissance de la piece
				while(not sim.read_fin_reccam());			// Attend la confirmation que la reconnaissance soit finie

				etat = TypeDePiece;							// Changement d'etat vers TypeDePiece
				sig-> info_finReccam.set();					// Informe le controleur que la reconnaissance de piece est terminee
				sleep(10);
			break;


			case TypeDePiece:
				cout << "Camera: TypeDePiece" << endl;
				cout << endl;

				sim.set_Reccam(false);						// Arret de la reconnaissance de la piece

				if(sim.read_p1())							// Si la piece 1 a ete reconnue
				{
					etat = DepartCamera;						// Changement d'etat vers l'etat initial
					sig-> info_P.send(1);						// Informe le gestionnaire de stock que l'on a une piece de type 1
				}

				if(sim.read_p2())							// Si la piece 2 a ete reconnue
				{
					etat = DepartCamera;						// Changement d'etat vers l'etat initial
					sig-> info_P.send(2);						// Informe le gestionnaire de stock que l'on a une piece de type 2
				}

				if(sim.read_p3())							// Si la piece 3 a ete reconnue
				{
					etat = DepartCamera;						// Changement d'etat vers l'etat initial
					sig-> info_P.send(3);						// Informe le gestionnaire de stock que l'on a une piece de type 3
				}
				sleep(1);
			break;
		}
	}
}
