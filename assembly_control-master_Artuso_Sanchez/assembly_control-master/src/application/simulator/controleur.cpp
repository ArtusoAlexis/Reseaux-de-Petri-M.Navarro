#include <iostream>
#include <unistd.h>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "controleur.h"

using namespace std;

enum EtatsControleur {												// Enumeration des differents etats du controleur
	DepartControleur,
	AttentePositionT1,
	AttenteFinReccam,
	AttentePiecePrise,
	AttenteGererPiece,
	AttentePositionAss,
	AttentePositionT2,
	AttenteOPi,
	AttenteArretT2,
	AttentePiecePosee,
	AttenteFinOP3,
	AttenteFinOP2,
	AttenteFinOP1
};

void tache_controleur(struct signaux *sig) {

	EtatsControleur etat = DepartControleur;						// Etat initial du controleur
	int execution = 0;												// Variable definissant comment gerer la piece (assemblage ou evacuation)
	int operation = 0;												// Variable definissant le type d'operation a realiser sur la piece (OP1, OP2 ou OP3)

	while(1) {
		switch(etat) {
			case DepartControleur:
				cout << "DepartControleur" << endl;
				cout << endl;

				etat = AttentePositionT1;							// Changement d'etat vers AttentePositionT1

				while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
				sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

				sig-> ordre.send(droite);							// Donne l'ordre au robot d'aller à droite
			break;

//=============================================================//

			case AttentePositionT1:
				cout << "Controleur: AttentePositionT1" << endl;
				cout << endl;

				sig-> info_Pos_t1.wait();							// Attend que la position du robot soit devant le tapis 1
				etat = AttenteFinReccam;							// Changement d'etat vers AttenteFinReccam
				sig-> ordre.send(stop);								// Donne l'ordre au robot de s'arreter
			break;

//=============================================================//

			case AttenteFinReccam:
				cout << "Controleur: AttenteFinReccam" << endl;
				cout << endl;

				while(not sig-> info_finReccam.has_arrived());		// Attend d'avoir recu le signal "info_finReccam" venant de la camera
				sig-> info_finReccam.clear();						// Reinitialisation du signal "info_finReccam"

				etat = AttentePiecePrise;							// Changement d'etat vers AttentePiecePrise

				while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
				sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

				sig-> ordre.send(prend);							// Donne l'ordre au robot de prendre la piece
			break;

//=============================================================//

			case AttentePiecePrise:
				cout << "Controleur: AttentePiecePrise" << endl;
				cout << endl;

				sig-> info_fprise.wait();							// Attend que le robot ait fini de prendre la piece
				etat = AttenteGererPiece;							// Changement d'etat vers AttenteGererPiece
				sig-> demande_demarrage_tapis1.notify();			// Donne l'ordre au tapis 1 de redemarrer
			break;

//=============================================================//

			case AttenteGererPiece:
				cout << "Controleur: AttenteGererPiece" << endl;
				cout << endl;

				sig-> attente_execution_piece.set();				// Demande au gestionnaire de stock comment gerer la piece
				execution = sig-> execution_piece.receive();		// Attend de savoir quoi faire de la piece

				if(execution == 1)									// Si "execution = 1", il faut assembler la piece
				{
					etat = AttentePositionAss;							// Changement d'etat vers AttentePositionAss

					while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
					sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

					sig-> ordre.send(gauche);							// Donne l'ordre au robot d'aller à gauche
					execution = 0;										// Reinitialisation de la variable "execution"
				}
				
				if(execution == 2)									// Si "execution = 2", il faut evacuer la piece
				{
					etat = AttentePositionT2;							// Changement d'etat vers AttentePositionT2
					sig-> ordre_extinction_t2.notify();					// Donne l'ordre au tapis 2 de s'arreter

					while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
					sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

					sig-> ordre.send(droite);							// Donne l'ordre au robot d'aller à droite
					execution = 0;										// Reinitialisation de la variable "execution"
				}
			break;

//=============================================================//

			case AttentePositionAss:
				cout << "Controleur: AttentePositionAss" << endl;
				cout << endl;

				sig-> info_Pos_Ass.wait();							// Attend que la position du robot soit devant l'assembleur
				etat = AttenteOPi;									// Changement d'etat vers AttenteOPi
				sig-> ordre.send(stop);								// Donne l'ordre au robot de s'arreter
			break;

//=============================================================//

			case AttentePositionT2:
				cout << "Controleur: AttentePositionT2" << endl;
				cout << endl;

				sig-> info_Pos_t2.wait();							// Attend que la position du robot soit devant le tapis 2
				etat = AttenteArretT2;								// Changement d'etat vers AttenteArretT2
				sig-> ordre.send(stop);								// Donne l'ordre au robot de s'arreter
			break;

//=============================================================//

			case AttenteOPi:
				cout << "Controleur: AttenteOPi" << endl;
				cout << endl;

				sig-> demande_assemblage.notify();					// Demande le type de la piece au gestionnaire de stock
				operation = sig-> assemblage_piece.receive();		// Reception de la reponse du gestionnaire de stock

				if(operation == 1)									// Si la piece est de type 1
				{
					etat = AttenteFinOP1;								// Changement d'etat vers AttenteFinOP1

					while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
					sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

					sig-> ordre.send(OP1);								// Donne l'ordre au robot de faire l'operation OP1
					operation = 0;										// Reinitialisation de la variable d'operation
				}

				if(operation == 2)									// Si la piece est de type 2
				{
					etat = AttenteFinOP2;								// Changement d'etat vers AttenteFinOP2

					while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
					sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

					sig-> ordre.send(OP2);								// Donne l'ordre au robot de faire l'operation OP2
					operation = 0;										// Reinitialisation de la variable d'operation
				}

				if(operation == 3)									// Si la piece est de type 3
				{
					etat = AttenteFinOP3;								// Changement d'etat vers AttenteFinOP3

					while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
					sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

					sig-> ordre.send(OP3);								// Donne l'ordre au robot de faire l'operation OP3
					operation = 0;										// Reinitialisation de la variable d'operation
				}
			break;

//=============================================================//

			case AttenteArretT2:
				cout << "Controleur: AttenteArretT2" << endl;
				cout << endl;

				sig-> demande_evacuation.notify();					// Informe le gestionnaire de stock de l'evacuation de la piece

				while(not sig-> info_t2_arrete.has_arrived());		// Attend d'avoir recu le signal "info_t2_arrete" venantdu tapis 2
				sig-> info_t2_arrete.clear();						// Reinitialisation du signal "info_t2_arrete"

				etat = AttentePiecePosee;							// Changement d'etat vers AttentePiecePosee

				while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
				sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

				sig-> ordre.send(pose);								// Donne l'ordre au robot de poser la piece
			break;

//=============================================================//

			case AttentePiecePosee:
				cout << "Controleur: AttentePiecePosee" << endl;
				cout << endl;

				sig-> info_fpose.wait();							// Attend que le robot ait fini de poser la piece
				etat = AttentePositionT1;							// Changement d'etat vers AttentePositionT1

				while(not sig-> attend_ordre.has_arrived());		// Attend d'avoir recu le signal "attend_ordre" venant du robot
				sig-> attend_ordre.clear();							// Reinitialisation du signal "attend_ordre"

				sig-> ordre.send(gauche);							// Donne l'ordre au robot d'aller à gauche
				sig-> demande_demarrage_tapis2.notify();			// Donne l'ordre au tapis 2 de redemarrer
			break;

//=============================================================//

			case AttenteFinOP3:
				cout << "Controleur: AttenteFinOP3" << endl;
				cout << endl;

				sig-> info_finOP3.wait();							// Attend que le robot ait fini d'executer l'operation OP3
				etat = DepartControleur;							// Changement d'etat vers l'etat initial
				sleep(1);
			break;

//=============================================================//

			case AttenteFinOP2:
				cout << "Controleur: AttenteFinOP2" << endl;
				cout << endl;

				sig-> info_finOP2.wait();							// Attend que le robot ait fini d'executer l'operation OP2
				etat = DepartControleur;							// Changement d'etat vers l'etat initial
				sleep(1);
			break;

//=============================================================//

			case AttenteFinOP1:
				cout << "Controleur: AttenteFinOP1" << endl;
				cout << endl;

				sig-> info_finOP1.wait();							// Attend que le robot ait fini d'executer l'operation OP1
				etat = DepartControleur;							// Changement d'etat vers l'etat initial
				sleep(1);
			break;
		}
	}
}
