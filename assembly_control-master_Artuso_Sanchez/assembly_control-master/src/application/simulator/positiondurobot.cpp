#include <iostream>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "positiondurobot.h"

using namespace std;

enum EtatsPositionDuRobot {											// Enumeration des differents etats de la position du robot
	PositionAssembleur,
	PositionTapis1,
	PositionTapis2,
	DeplacementT1,
	DeplacementT2,
	DeplacementAss
};

void tache_positiondurobot(struct signaux *sig) {

	EtatsPositionDuRobot etat = PositionAssembleur;					// Etat initial de la position du robot
	int temporisation = 0;											// Variable definissant la temporisation avec le robot

	while(1) {
		switch(etat) {
			case PositionAssembleur:
				cout << "Position: Assembleur" << endl;
				cout << endl;

				sig-> temporisation.set();							// Demande au robot la direction de deplacement
				temporisation = sig-> Tempo.receive();				// Reception de la direction a prendre

				if(temporisation == 1)								// Si "temporisation = 1", deplacement vers la droite
				{
					etat = DeplacementT1;								// Changement d'etat vers DeplacementT1
					temporisation = 0;									// Reinitialisation de la variable "temporisation"
				}
			break;

//==============================================================//

			case PositionTapis1:
				cout << "Position: Tapis1" << endl;
				cout << endl;

				sig-> temporisation.set();							// Demande au robot la direction de deplacement
				temporisation = sig-> Tempo.receive();				// Reception de la direction a prendre

				if(temporisation == 1)								// Si "temporisation = 1", deplacement vers la droite
				{
					etat = DeplacementT2;								// Changement d'etat vers DeplacementT2
					temporisation = 0;									// Reinitialisation de la variable "temporisation"
				}

				if(temporisation == 2)								// Si "temporisation = 2", deplacement vers la gauche
				{
					etat = DeplacementAss;								// Changement d'etat vers DeplacementAss
					temporisation = 0;									// Reinitialisation de la variable "temporisation"
				}
			break;

//==============================================================//

			case PositionTapis2:
				cout << "Position: Tapis2" << endl;
				cout << endl;

				sig-> temporisation.set();							// Demande au robot la direction de deplacement
				temporisation = sig-> Tempo.receive();				// Reception de la direction a prendre

				if(temporisation == 2)								// Si "temporisation = 2", deplacement vers la gauche
				{
					etat = DeplacementT1;								// Changement d'etat vers DeplacementT1
					temporisation = 0;									// Reinitialisation de la variable "temporisation"
				}
			break;

//==============================================================//

			case DeplacementT1:
				cout << "Position: Deplacement vers T1" << endl;
				cout << endl;

				sim.wait_pos_t1();										// Attend d'etre arrive devant le tapis 1
				etat = PositionTapis1;									// Changement d'etat vers PositionTapis1
				sig-> info_Pos_t1.notify();								// Informe le controleur de la position actuelle
			break;

//==============================================================//

			case DeplacementT2:
				cout << "Position: Deplacement vers T2" << endl;
				cout << endl;

				sim.wait_pos_t2();										// Attend d'etre arrive devant le tapis 2
				etat = PositionTapis2;									// Changement d'etat vers PositionTapis2
				sig-> info_Pos_t2.notify();								// Informe le controleur de la position actuelle
			break;

//==============================================================//

			case DeplacementAss:
				cout << "Position: Deplacement vers Assembleur" << endl;
				cout << endl;

				sim.wait_pos_assem();									// Attend d'etre arrive devant l'assembleur
				etat = PositionAssembleur;								// Changement d'etat vers PositionAssembleur
				sig-> info_Pos_Ass.notify();							// Informe le controleur de la position actuelle
			break;
		}
	}
}
