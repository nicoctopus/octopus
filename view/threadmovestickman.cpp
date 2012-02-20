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
	//tant que que l'on a pas parcouru toute la liste des position on continue à parcourir la liste de joints du mouvement.
	// On déplace tous les joints d'une position, puis au tour d'apres on redéplace tous les joints d'une autre position.
	while(idxPosition<listeDeJointsMvt->at(0)->getListPositions()->size()){
	    for(int i=0; i<listeDeJointsMvt->size(); ++i){
		emit sigMoveNode(listeDeJointsMvt->at(i)->getJointRef()->getNom(), -(listeDeJointsMvt->at(i)->getListPositions()->at(idxPosition)->getX()) / (3*COEF_FLOAT2INT), -(listeDeJointsMvt->at(i)->getListPositions()->at(idxPosition)->getY()) / (3*COEF_FLOAT2INT),(listeDeJointsMvt->at(i)->getListPositions()->at(idxPosition)->getZ()) / (3*COEF_FLOAT2INT));
		usleep(20000);
	    }
	    idxPosition++;
	}
    //}
    emit sigEndOfMoveStickman();

}


