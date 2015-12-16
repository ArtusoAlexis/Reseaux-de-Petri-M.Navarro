#ifndef STRUCT_H
#define STRUCT_H
#include "simulator.h"


enum OrdreRobot{		// Enumeration des directives du robot pour le signal "ordre" 
	droite,
	gauche,
	stop,
	prend,
	pose,
	OP1,
	OP2,
	OP3
};

struct signaux{

//==========================================================//
//===================== Signaux tapis 1 ====================//
//==========================================================//

	Signal demande_demarrage_tapis1;

	StatusSignal info_CO;

//==========================================================//
//===================== Signaux tapis 2 ====================//
//==========================================================//

	Signal demande_demarrage_tapis2;
	Signal ordre_extinction_t2;

	StatusSignal info_t2_arrete;

//==========================================================//
//===================== Signaux camera =====================//
//==========================================================//

	StatusSignal ordre_CO;
	StatusSignal info_finReccam;

	MessageBox<int> info_P;
	
//==========================================================//
//=================== Signaux assembleur ===================//
//==========================================================//

	Signal ordre_verif;
	Signal info_Ass_conforme;
	Signal info_Ass_evacue;

//==========================================================//
//===================== Signaux robot ======================//
//==========================================================//

	StatusSignal attend_ordre;
	StatusSignal temporisation;

	MessageBox<OrdreRobot> ordre;
	MessageBox<int> Tempo;

	Signal info_fprise;
	Signal info_fpose;

	Signal info_finOP1;
	Signal info_P1_dans_assembleur;

	Signal info_finOP2;
	Signal info_P2_dans_assembleur;

	Signal info_finOP3;
	Signal info_P3_dans_assembleur;

//==========================================================//
//================ Signaux position du robot ===============//
//==========================================================//

	Signal info_Pos_t1;
	Signal info_Pos_t2;
	Signal info_Pos_Ass;

//==========================================================//
//================ Signaux gestion du stock ================//
//==========================================================//

	MessageBox<int> execution_piece;
	MessageBox<int> assemblage_piece;

	Signal demande_assemblage;
	Signal demande_evacuation;

	StatusSignal attente_execution_piece;

};


#endif
