/**
 * @file main.cpp
 * @brief 	Pilotage d’une cellule d’assemblage robotisee
 * @author	ARTUSO - SANCHEZ
 * @version	1.0.0
 * @date 	2015-12-09
 */

#include <unistd.h>
#include <iostream>
#include <thread>

#include "simulator.h"
#include "struct.h"

#include "tapis1.h"
#include "tapis2.h"
#include "assembleur.h"
#include "robot.h"
#include "positiondurobot.h"
#include "camera.h"
#include "controleur.h"
#include "gestiondustock.h"


using namespace std;

/**
 * @brief
 *
 * @param argc
 * @param argv[]
 *
 * @return
 */


int main(int argc, char const *argv[])
{
	signaux sig;

    if(not sim.start(10)) {
    	return -1;
    }

	thread positiondurobot(tache_positiondurobot,&sig);
	thread robot(tache_robot,&sig);
	thread tapis1(tache_tapis1, &sig);
	thread tapis2(tache_tapis2,&sig);
	thread assembleur(tache_assembleur,&sig);
	thread camera(tache_camera,&sig);
	thread gestiondustock(tache_gestiondustock,&sig);
	thread controleur(tache_controleur,&sig); 


	tapis1.join();
	tapis2.join();
	assembleur.join();
	camera.join();
	robot.join();
	positiondurobot.join();
	gestiondustock.join();
	controleur.join();


    sim.stop();
	return 0;
}
