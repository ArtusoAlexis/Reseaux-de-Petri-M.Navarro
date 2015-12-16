#include <iostream>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "assembleur.h"

using namespace std;

enum EtatsAssembleur {											// Enumeration des differents etats de l'assembleur
	DepartAssembleur,
	Verification,
	Temporisation
};

void tache_assembleur(struct signaux *sig) {

	EtatsAssembleur etat = DepartAssembleur;					// Etat initial de l'assembleur

	while(1) {
		switch(etat) {
			case DepartAssembleur:
				cout << "DepartAssembleur" << endl;
				cout << endl;

				sig-> ordre_verif.wait();						// Attend que le gestionnaire de stock ordonne une verificaton de l'assemblage
				etat = Verification;							// Changement d'etat vers Verification
			break;
				

			case Verification:
				cout << "Assembleur: Verification" << endl;
				cout << endl;

				sim.set_Verif(true);							// Lancement de la verification
				if(sim.read_assemblage_conforme())				// Si l'assemblage est conforme
				{
					etat = Temporisation;							// Changement d'etat vers Temporisation
					sig-> info_Ass_conforme.notify();				// Informe le gestionnaire de stock que l'assemblage est bien conforme
				}
			break;


			case Temporisation:
				cout << "Assembleur: Temporisation" << endl;
				cout << endl;

				sim.set_Verif(false);							// Arret de la verification
				while(not sim.read_assemblage_evacue());		// Attend la confirmation que l'assemblage est evacue
				etat = DepartAssembleur;						// Changement d'etat vers l'etat initial
				sig-> info_Ass_evacue.notify();					// Informe le gestionnaire de stock que l'assemblage est bien evacue
			break;
		}
	}
}
