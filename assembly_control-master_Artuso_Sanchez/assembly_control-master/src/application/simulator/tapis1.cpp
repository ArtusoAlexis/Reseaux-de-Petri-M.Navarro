#include <iostream>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "tapis1.h"

using namespace std;

enum EtatsTapis1 {											// Enumeration des differents etats du tapis 1
	Tapis1EnMarche,
	Tapis1Arrete
};

void tache_tapis1(struct signaux *sig) {

	EtatsTapis1 etat = Tapis1EnMarche;						// Etat initial du tapis 1

	while(1) {
		switch(etat) {
			case Tapis1EnMarche:
				cout << "Tapis1EnMarche" << endl;
				cout << endl;

				sim.set_AV_T1(true);						// Mise en marche du tapis 1
				sim.wait_co();								// Attend la detection d'une piece par la cellule optique
				etat = Tapis1Arrete;						// Changement d'etat vers Tapis1Arrete
				sig-> info_CO.set();						// Informe le gestionnaire de stock de l'arrivee d'une piece
			break;


			case Tapis1Arrete:
				cout << "Tapis1Arrete" << endl;
				cout << endl;

				sim.set_AV_T1(false);						// Arret du tapis 1
				sig-> demande_demarrage_tapis1.wait();		// Attend l'ordre de redemarrer venant du controleur
				etat = Tapis1EnMarche;						// Changement d'etat vers l'etat initial
			break;
		}
	}
}
