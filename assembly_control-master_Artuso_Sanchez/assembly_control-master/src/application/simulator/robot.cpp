#include <iostream>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "robot.h"

using namespace std;

enum EtatsRobot{																// Enumeration des differents etats du robot
	DepartRobot,
	Droite,
	Gauche,
	Prendre,
	Poser,
	ActionOP1,
	ActionOP2,
	ActionOP3
};


void tache_robot(struct signaux *sig) {

	EtatsRobot etat = DepartRobot;												// Etat initial du robot
	OrdreRobot ordre = stop;													// Variable definissant les ordre donnes par le controleur

	while(1) {
		switch(etat) {
			case DepartRobot:
				cout << "DepartRobot" << endl;
				cout << endl;

				sig-> attend_ordre.set();										// Informe le controleur de l'attente d'un ordre
				ordre = sig-> ordre.receive();									// Reception d'un ordre

				if(ordre == droite)												// Si l'ordre "droite" est recu
				{
					cout << "Robot: ordre == droite: " << ordre << endl;
					cout << endl;

					etat = Droite;													// Changement d'etat vers Droite

					while(not sig-> temporisation.has_arrived());					// Attend d'avoir recu le signal "temporisation" venant de la position du robot
					sig-> temporisation.clear();									// Reinitialisation du signal "temporisation"

					sig->Tempo.send(1);												// Informe la position du robot d'un deplacement vers la droite
				}

				if(ordre == gauche)												// Si l'ordre "gauche" est recu
				{
					cout << "Robot: ordre == gauche: " << ordre << endl;
					cout << endl;

					etat = Gauche;													// Changement d'etat vers Gauche

					while(not sig-> temporisation.has_arrived());					// Attend d'avoir recu le signal "temporisation" venant de la position du robot
					sig-> temporisation.clear();									// Reinitialisation du signal "temporisation"

					sig->Tempo.send(2);												// Informe la position du robot d'un deplacement vers la gauche
				}

				if(ordre == prend)												// Si l'ordre "prend" est recu
				{
					cout << "Robot: ordre == prend: " << ordre << endl;
					cout << endl;

					etat = Prendre;													// Changement d'etat vers Prendre
				}

				if(ordre == pose)												// Si l'ordre "pose" est recu
				{
					cout << "Robot: ordre == pose: " << ordre << endl;
					cout << endl;

					etat = Poser;													// Changement d'etat vers Poser
				}

				if(ordre == OP1)												// Si l'ordre "OP1" est recu
				{
					cout << "Robot: ordre == OP1: " << ordre << endl;
					cout << endl;

					etat = ActionOP1;												// Changement d'etat vers ActionOP1
				}
				
				if(ordre == OP2)												// Si l'ordre "OP2" est recu
				{
					cout << "Robot: ordre == OP2: " << ordre << endl;
					cout << endl;

					etat = ActionOP2;												// Changement d'etat vers ActionOP2
				}	

				if(ordre == OP3)												// Si l'ordre "OP3" est recu
				{
					cout << "Robot: ordre == OP3: " << ordre << endl;
					cout << endl;

					etat = ActionOP3;												// Changement d'etat vers ActionOP3
				}				
			break;

//=============================================================//

			case Droite:
				cout << "Robot: Droite" << endl;
				cout << endl;

				sim.set_D(true);												// Deplacement du robot vers la droite
				ordre = sig-> ordre.receive();									// Reception d'un nouvel ordre du controleur

				while(ordre != stop);											// Attend l'ordre "stop" du controleur
				sim.set_D(false);												// Arret du robot
				etat = DepartRobot;												// Changement d'etat vers l'etat initial
			break;

//=============================================================//

			case Gauche:
				cout << "Robot: Gauche" << endl;
				cout << endl;

				sim.set_G(true);												// Deplacement du robot vers la gauche
				ordre = sig-> ordre.receive();									// Reception d'un nouvel ordre du controleur

				while(ordre != stop);											// Attend l'ordre "stop" du controleur
				sim.set_G(false);												// Arret du robot
				etat = DepartRobot;												// Changement d'etat vers l'etat initial
			break;

//=============================================================//

			case Prendre:
				cout << "Robot: Prendre" << endl;
				cout << endl;

				sim.set_Prend(true);											// Effectue l'operation "prendre la piece"
				sim.wait_fprise();												// Attend la fin de l'operation "prendre la piece"

				sim.set_Prend(false);											// Arret de l'operation "prendre la piece"
				etat = DepartRobot;												// Changement d'etat vers l'etat initial
				sig-> info_fprise.notify();										// Informe le controleur que la piece est prise
			break;

//=============================================================//

			case Poser:
				cout << "Robot: Poser" << endl;
				cout << endl;

				sim.set_Pose(true);												// Effectue l'operation "poser la piece"
				sim.wait_fpose();												// Attend la fin de l'operation "poser la piece"

				sim.set_Pose(false);											// Arret de l'operation "poser la piece"
				etat = DepartRobot;												// Changement d'etat vers l'etat initial
				sig-> info_fpose.notify();										// Informe le controleur que la piece est posee
			break;

//=============================================================//

			case ActionOP1:
				cout << "Robot: ActionOP1" << endl;
				cout << endl;

				sim.set_OP1(true);												// Effectue l'operation OP1
				while(not sim.read_fin_OP1());									// Attend la fin de OP1

				sim.set_OP1(false);												// Arret de OP1
				etat = DepartRobot;												// Changement d'etat vers l'etat initial

				sig-> info_finOP1.notify();										// Informe le controleur de la fin de OP1
				sig-> info_P1_dans_assembleur.notify();							// Informe le gestionnaire de stock que la piece 1 est dans l'assembleur
			break;

//=============================================================//
	
			case ActionOP2:
				cout << "Robot: ActionOP2" << endl;
				cout << endl;

				sim.set_OP2(true);												// Effectue l'operation OP2
				while(not sim.read_fin_OP2());									// Attend la fin de OP2

				sim.set_OP2(false);												// Arret de OP2
				etat = DepartRobot;												// Changement d'etat vers l'etat initial

				sig-> info_finOP2.notify();										// Informe le controleur de la fin de OP2
				sig-> info_P2_dans_assembleur.notify();							// Informe le gestionnaire de stock que la piece 2 est dans l'assembleur
			break;

//=============================================================//
	
			case ActionOP3:
				cout << "Robot: ActionOP3" << endl;
				cout << endl;

				sim.set_OP3(true);												// Effectue l'operation OP3
				while(not sim.read_fin_OP3());									// Attend la fin de OP3

				sim.set_OP3(false);												// Arret de OP3
				etat = DepartRobot;												// Changement d'etat vers l'etat initial

				sig-> info_finOP3.notify();										// Informe le controleur de la fin de OP3
				sig-> info_P3_dans_assembleur.notify();							// Informe le gestionnaire de stock que la piece 3 est dans l'assembleur
			break;
		}
	}
}
