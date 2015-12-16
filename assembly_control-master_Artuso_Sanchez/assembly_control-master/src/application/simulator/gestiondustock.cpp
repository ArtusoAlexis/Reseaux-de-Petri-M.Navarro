#include <iostream>
#include <thread>
#include "simulator.h"
#include "struct.h"
#include "gestiondustock.h"

using namespace std;

enum EtatsGestionDuStock {													// Enumeration des differents etats du gestionnaire de stock
	DepartGestiondustock,
	TypeDePiece,
	VerificationStock1,
	VerificationStock2,
	VerificationStock3
};

void tache_gestiondustock(struct signaux *sig) {

	EtatsGestionDuStock etat = DepartGestiondustock;						// Etat initial du gestionnaire de stock
	int piece = 0;															// Variable definissant le type de la piece traitee
	int Stock1 = 0, Stock2 = 0, Stock3 = 0;									// Variables definissants l'etat des stocks 1, 2 et 3 (vide ou plein)

	while(1) {
		switch(etat) {
			case DepartGestiondustock:
				cout << "DepartGestiondustock" << endl;
				cout << endl;

				while(not sig-> info_CO.has_arrived());						// Attend d'avoir recu le signal "info_CO" venant du tapis 1
				sig-> info_CO.clear();										// Reinitialisation du signal "info_CO"

				etat = TypeDePiece;											// Changement d'etat vers TypeDePiece
				sig-> ordre_CO.set();										// Informe la camera qu'une piece se trouve devant la cellule optique
			break;

//====================================================================//

			case TypeDePiece:
				cout << "Stock: TypeDePiece" << endl;
				cout << endl;

				piece = sig-> info_P.receive();								// Reception du type de piece par la camera

				if(piece == 1)												// Si la piece est de type 1
				{
					etat = VerificationStock1;									// Changement d'etat vers VerificationStock1
				}

				if(piece == 2)												// Si la piece est de type 2
				{
					etat = VerificationStock2;									// Changement d'etat vers VerificationStock2
				}

				if(piece == 3)												// Si la piece est de type 3
				{
					etat = VerificationStock3;									// Changement d'etat vers VerificationStock3
				}
			break;

//====================================================================//

			case VerificationStock1:
				cout << "Stock: VerificationStock1" << endl;
				cout << endl;

				while(not sig-> attente_execution_piece.has_arrived());		// Attend d'avoir recu le signal "attente_execution_piece" venant du controleur
				sig-> attente_execution_piece.clear();						// Reinitialisation du signal "attente_execution_piece"

				if(Stock1 == 0)												// Si le stock 1 est vide
				{
					Stock1 = 1;													// Remplissement du stock 1
					sig-> execution_piece.send(1);								// Informe le controleur qu'il faut assembler la piece
					sig-> demande_assemblage.wait();							// Attend que le controleur demande le type de piece

					sig-> assemblage_piece.send(1);								// Informe le controleur que la piece est de type 1
					sig-> info_P1_dans_assembleur.wait();						// Attend confirmation du robot que la piece soit dans l'assembleur
				}

				else if(Stock1 == 1)										// Si le stock 1 est plein
				{
					sig-> execution_piece.send(2);								// Informe le controleur qu'il faut evacuer la piece
					sig-> demande_evacuation.wait();							// Attend l'information du controleur que la piece se fait evacuer
				}
				
				etat = DepartGestiondustock;									// Changement d'etat vers l'etat initial
			break;

//====================================================================//

			case VerificationStock2:
				cout << "Stock: VerificationStock2" << endl;
				cout << endl;

				while(not sig-> attente_execution_piece.has_arrived());		// Attend d'avoir recu le signal "attente_execution_piece" venant du controleur
				sig-> attente_execution_piece.clear();						// Reinitialisation du signal "attente_execution_piece"

				if(Stock2 == 0)												// Si le stock 2 est vide
				{
					Stock2 = 1;													// Remplissement du stock 2
					sig-> execution_piece.send(1);								// Informe le controleur qu'il faut assembler la piece
					sig-> demande_assemblage.wait();							// Attend que le controleur demande le type de piece

					sig-> assemblage_piece.send(2);								// Informe le controleur que la piece est de type 2
					sig-> info_P2_dans_assembleur.wait();						// Attend confirmation du robot que la piece soit dans l'assembleur
				}

				else if(Stock2 == 1)										// Si le stock 2 est plein
				{
					sig-> execution_piece.send(2);								// Informe le controleur qu'il faut evacuer la piece
					sig-> demande_evacuation.wait();							// Attend l'information du controleur que la piece se fait evacuer
				}

				etat = DepartGestiondustock;									// Changement d'etat vers l'etat initial
			break;

//====================================================================//

			case VerificationStock3:
				cout << "Stock: VerificationStock3" << endl;
				cout << endl;

				while(not sig-> attente_execution_piece.has_arrived());		// Attend d'avoir recu le signal "attente_execution_piece" venant du controleur
				sig-> attente_execution_piece.clear();						// Reinitialisation du signal "attente_execution_piece"

				if(Stock3 == 0)												// Si le stock 3 est vide
				{
					Stock3 = 1;													// Remplissement du stock 3
					sig-> execution_piece.send(1);								// Informe le controleur qu'il faut assembler la piece
					sig-> demande_assemblage.wait();							// Attend que le controleur demande le type de piece

					sig-> assemblage_piece.send(3);								// Informe le controleur que la piece est de type 3
					sig-> info_P3_dans_assembleur.wait();						// Attend confirmation du robot que la piece soit dans l'assembleur
				}

				else if(Stock3 == 1)										// Si le stock 3 est plein
				{
					sig-> execution_piece.send(2);								// Informe le controleur qu'il faut evacuer la piece
					sig-> demande_evacuation.wait();							// Attend l'information du controleur que la piece se fait evacuer
				}
				
				etat = DepartGestiondustock;									// Changement d'etat vers l'etat initial
			break;
		}

//====================================================================//
//======================== Fin du switch case ========================//
//====================================================================//


		if( (Stock1==1)&&(Stock2==1)&&(Stock3==1) )							// Si les trois stocks sont pleins
		{
			cout << "Stock: tous les stocks sont pleins" << endl;
			cout << endl;

			sig-> ordre_verif.notify();										// Donne l'ordre à l'assembleur de verifier l'assemblage
			sig->info_Ass_conforme.wait();									// Attend la confirmation de l'assembleur que l'assemblage est conforme
			sig->info_Ass_evacue.wait();									// Attend la confirmation de l'assembleur que l'assemblage est evacue

			Stock1 = 0;														// Liberation du stock 1
			Stock2 = 0;														// Liberation du stock 2
			Stock3 = 0;														// Liberation du stock 3
		}
	}
}
