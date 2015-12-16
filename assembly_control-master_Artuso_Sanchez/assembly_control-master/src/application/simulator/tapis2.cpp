#include <iostream>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "tapis2.h"

using namespace std;

enum EtatsTapis2 {											// Enumeration des differents etats du tapis 2
	Tapis2EnMarche,
	Tapis2MiseEnArret,
	Tapis2Arrete
};

void tache_tapis2(struct signaux *sig) {

	EtatsTapis2 etat = Tapis2EnMarche;						// Etat initial du tapis 2

	while(1) {
		switch(etat) {
			case Tapis2EnMarche:
				cout << "Tapis2EnMarche" << endl;
				cout << endl;

				sim.set_AV_T2(true);						// Mise en marche du tapis 2
				sig-> ordre_extinction_t2.wait();			// Attend l'ordre venant du controleur d'arreter le tapis 2
				etat = Tapis2MiseEnArret;					// Changement d'etat vers Tapis2MiseEnArret
			break;
				

			case Tapis2MiseEnArret:
				cout << "Tapis2MiseEnArret" << endl;
				cout << endl;

				sim.set_AV_T2(false);						// Mise en arret du tapis 2
				sim.wait_arret_t2();						// Attend l'arret total du tapis 2
				etat = Tapis2Arrete;						// Changement d'etat vers Tapis2Arrete
				sig-> info_t2_arrete.set();					// Informe le controleur que le tapis 2 est arrete
			break;


			case Tapis2Arrete:
				cout << "Tapis2Arrete" << endl;
				cout << endl;

				sig-> demande_demarrage_tapis2.wait();		// Attend l'ordre de redemarrer venant du controleur
				etat = Tapis2EnMarche;						// Changement d'etat vers l'etat initial
			break;
		}
	}
}
