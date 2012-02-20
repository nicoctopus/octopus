#include "threadmovestickman.h"

ThreadMoveStickman::ThreadMoveStickman()
{
}

void ThreadMoveStickman::playThisMovement(Movement* movement){
    this->movement = movement;
}

void ThreadMoveStickman::run(){


    QList<JointMvt*> listeDeJointsMvt = movement->getListJointsMvt();

    int idxPosition = 0;
    //tant que que l'on a pas parcouru toute la liste des position on continue � parcourir la liste de joints du mouvement.
    // On d�place tous les joints d'une position, puis au tour d'apres on red�place tous les joints d'une autre position.
    while(idxPosition<listeDeJointsMvt.at(0)->getListPositions().size()){
        for(int i=0; i<listeDeJointsMvt.size(); ++i){
            emit sigMoveNode(listeDeJointsMvt.at(i)->getJointRef()->getNom(), listeDeJointsMvt.at(i)->getListPositions().at(idxPosition)->getX(), listeDeJointsMvt.at(i)->getListPositions().at(idxPosition)->getY(),listeDeJointsMvt.at(i)->getListPositions().at(idxPosition)->getZ());
            Sleep(020);
        }
        idxPosition++;
    }
    //emit sigEndOfMoveStickman();

}


