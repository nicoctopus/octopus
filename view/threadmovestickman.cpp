#include "threadmovestickman.h"

ThreadMoveStickman::ThreadMoveStickman()
{
}

ThreadMoveStickman::ThreadMoveStickman(Movement* movement){
    this->movement = movement;
}

void ThreadMoveStickman::run(){
    //if(!movement->getListJointsMvt()->isEmpty())
    //{
	QList<JointMvt*> *listeDeJointsMvt = movement->getListJointsMvt();

	int idxPosition = 0;
	//tant que que l'on a pas parcouru toute la liste des position on continue � parcourir la liste de joints du mouvement.
	// On d�place tous les joints d'une position, puis au tour d'apres on red�place tous les joints d'une autre position.
	if(!listeDeJointsMvt->isEmpty())
	    while(idxPosition<listeDeJointsMvt->at(0)->getListPositions()->size())
	    {
		for(int i=0; i<listeDeJointsMvt->size(); ++i){
		    emit sigMoveNode(listeDeJointsMvt->at(i)->getJointRef()->getNom(), -(listeDeJointsMvt->at(i)->getListPositions()->at(idxPosition)->getX()) / (3*Position::DIVISEUR), -(listeDeJointsMvt->at(i)->getListPositions()->at(idxPosition)->getY()) / (3*Position::DIVISEUR),(listeDeJointsMvt->at(i)->getListPositions()->at(idxPosition)->getZ()) / (3*Position::DIVISEUR));
		    usleep(25000);
		}
		idxPosition++;
	    }
    //}
    emit sigEndOfMoveStickman();

}


