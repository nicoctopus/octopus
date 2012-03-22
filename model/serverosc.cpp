#include "serverosc.h"

ServerOSC::ServerOSC()
{
    this->listJoints = new QList<Joint*>();
    this->recording = false;
    this->setRunnable(false);
    size1,size2,size3,size4,size5,size6,size7,size8,size9,size10 = false;
    this->vitesse = 0.3;
    this->amplitude = 55;
}

ServerOSC::ServerOSC(const quint16 &portNb, const bool &active) : Port(portNb, active){
    this->listJoints = new QList<Joint*>();
    this->nameElement = QString(portNb);
    this->recording = false;
    this->setRunnable(false);
    size1,size2,size3,size4,size5,size6,size7,size8,size9,size10 = false;
    this->vitesse = 0.3;
    this->amplitude = 55;
}

const char* ServerOSC::className() {
    return "ServerOSC";
}

/**
  * Thread
  */
void ServerOSC::run() {


    socket->bindTo(this->portNumber);
    //socket ouverte
    if(socket->isOk()) {


        while(runnable && socket->isOk()) {

            if(!isRecording()) { // si on enregistre pas

                fillBuffers(); // on remplit les buffers
            } else {
                record(); // on enregistre le mouvement
            }

	}
	if(!recording)
	    this->deleteAnalyse();

    }

    // exec();
}

/**
  * FONCTION EN MODE PERFORMANCE
  *
  * Messages envoyes par semaine :
  * /tracking_skeleton <bool>  -  Sent with 1 when we start tracking a skeleton, and 0 when we lose it.
  * /<joint>_pos_world <float> <float> <float>  -  The x,y,z position of the joint in world space, in millimeters.
  * /<joint>_pos_body <float> <float> <float>  -  The x,y,z position of the joint relative to the torso, in millimeters.
  * /<joint>_pos_screen <float> <float> <float>  -  The x,y,z position of the joint as projected onto the screen, in pixels.
  * /<joint> [up,down,left,right,forward,back]  -  Sent when we detect a “hit” event, such as a punch forward, which would be “righthand forward”.
  * /<joint>_requiredlength <float>  -  The current tuning value of how far you must move in a straight line (in mm) before a hit event is generated
  * /<joint>_pointhistorysize <int>  -  The current tuning value of how many points are being tracked for hit event detection.
  **/
void ServerOSC::fillBuffers(){
    float x,y,z; // repere : torso (pour les mouvements)
    float a,b,c; // repere : world (pour la position dans l'espace)
    bool d;
    bool launchAnalyze; // savoir s'il faut lancer l'analyse ou s'il s'agit d'un message "inutile"
    //qDebug()<< "CACA ANALYSE "<< endl;

    if (socket->receiveNextPacket()) {
        reader->init(socket->packetData(), socket->packetSize());
        while (reader->isOk() && (message = reader->popMessage()) != 0 && runnable) {
	    launchAnalyze = false;
	    /** track joints
	      * repere : body
	      * Donnees : x, y, z
	      **/
            if (message->match("/righthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
                //chercher le joint correspondant dans la liste de tous les joints
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "righthand"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size1 = true;
			//AFFICHAGE TAILLE BUFFER
			/*if(this->listJoints->at(i)->getBufferPositions()->size() == 450)
			{
			    for(int j = 0 ; j< this->listJoints->at(i)->getBufferPositions()->size(); j++)
				if(j%10 == 0)
				    qDebug() << "buf " << j << ":" << this->listJoints->at(i)->getBufferPositions()->at(j)->getX();
			    qDebug() << endl;
			}*/
                    }
                }
            }
            else if (message->match("/lefthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "lefthand"){
			this->listJoints->at(i)->addPosition(x,y,z);
			//qDebug() << y << endl;
			//qDebug() << "REMPLISSAGE LEFTHAND last y " << this->listJoints->at(i)->getBufferPositions()->last()->getY() << endl;
			//qDebug() << "REMPLISSAGE LEFTHAND first y " << this->listJoints->at(i)->getBufferPositions()->first()->getY() << endl;
			size2 = true;
                    }
                }
            }
            else if (message->match("/rightelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "rightelbow"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size3 = true;
                    }
                }
            }
            else if (message->match("/leftelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "leftelbow"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size4 = true;
                    }
                }
            }
            else if (message->match("/rightfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "rightfoot"){
			this->listJoints->at(i)->addPosition(x,y,z);
			//qDebug() << "size buffer :" << this->listJoints->at(i)->getBufferPositions()->size() << endl;
			size5 = true;
                    }
                }
            }
            else if (message->match("/leftfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "leftfoot"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size6 = true;
                    }
                }
            }
            else if (message->match("/rightknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "rightknee"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size7 = true;
                    }
                }
            }
            else if (message->match("/leftknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "leftknee")
			this->listJoints->at(i)->addPosition(x,y,z);
			size8 = true;
                }
            }
            else if (message->match("/head_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "head"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size9 = true;
                    }
                }
            }
            else if (message->match("/torso_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		launchAnalyze = true; // on lancera l'analyse a la fin ...
		for (int i=0; i<this->listJoints->size(); i++){
                    if(this->listJoints->at(i)->getNom() == "torso"){
			this->listJoints->at(i)->addPosition(x,y,z);
			size10 =  true;
                    }
                }
            }
            else if (message->match("/tracking_skeleton").popBool(d).isOkNoMoreArgs()) {
                //tracking ok ...
            }
            else {

                //qDebug() << "Server Error : unknown type of data " << endl;
            }


	    //lancer l'analyse
	    if(launchAnalyze && allBuffersSameSize()) {
		if(size1*size2*size3*size4*size5*size6*size7*size8*size9*size10 == true) {
		    //qDebug() << "début analyse" << endl;
		    this->analyse->calculBITG();
		    size1,size2,size3,size4,size5,size6,size7,size8,size9,size10 = false;
		}
	    }else{
	     //qDebug()<< "ON SORT DU FILL ! "<< endl;
	    }
	}
    }



}

/**
  * Enregistrer un mouvement :
  * Mettre les Positions dans les JointsMouvements relatifs au mouvement a enregistrer
  **/
void ServerOSC::record(){
    float x,y,z; // repere : torso (pour les mouvements)
    float a,b,c; // repere : world (pour la position dans l'espace)
    bool d;

    if(movement->getListJointsMvt()->at(0)->getListPositions()->size() < SIZE_MAX_JOINT_MOVEMENT)
    {
        if (socket->receiveNextPacket(1 /* timeout, in ms */)) {
            reader->init(socket->packetData(), socket->packetSize());
            while (reader->isOk() && (message = reader->popMessage()) != 0) {
                /** track joints
       * repere : body
       * Donnees : x, y, z
       */
		QList<JointMvt*> *listJointsMvt = movement->getListJointsMvt();
                if (message->match("/tracking_skeleton").popBool(d).isOkNoMoreArgs()) {
                    //tracking ok ...
                }
                else if (message->match("/lefthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
                    //chercher le joint correspondant dans le mouvement
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "lefthand")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/righthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "righthand")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));

                        }
                    }
                }
                else if (message->match("/rightelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "rightelbow")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/leftelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "leftelbow")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/rightfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "rightfoot")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/leftfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "leftfoot")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/rightknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "rightknee")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/leftknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "leftknee")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
			}
		    }
                }
                else if (message->match("/head_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "head")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else if (message->match("/torso_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		    for (int i=0; i<listJointsMvt->size(); i++){
			if(listJointsMvt->at(i)->getJointRef()->getNom() == "torso")
			{
			    listJointsMvt->at(i)->addPosition(new Position(x,y,z,0,0,0));
                        }
                    }
                }
                else {
                    //qDebug() << "Server Error : unknown type of data" << endl;
                }
            }
        }
    }
    else
    {
	emit jointMvtTooBig();
    }
}

void ServerOSC::setListJoints(QList<Joint *> *list) {
    this->listJoints = list;
}

void ServerOSC::setMovement(Movement *movement)
{
    this->movement = movement;
}

void ServerOSC::setListMovements(QList<Movement*> *list) {
    this->listMovements = list;
}
void ServerOSC::setRecording(const bool &b) {
    this->recording = b;
}
bool ServerOSC::isRecording() {
    return this->recording;
}

void ServerOSC::setAnalyse() {
    this->analyse = new Analyse(this->vitesse,this->amplitude, this->listMovements); //vitesse, tx erreur
}

void ServerOSC::deleteAnalyse(){
    if(this->listJoints->size()!=0){
        for (int i=0; i<this->listJoints->size(); i++){
	    for(int j = 0 ; j < this->listJoints->at(i)->getBufferPositions()->size() ; j++)
		delete this->listJoints->at(i)->getBufferPositions()->at(j);
	    this->listJoints->at(i)->getBufferPositions()->clear();
        }
	delete this->analyse ;
    }

}

bool ServerOSC::allBuffersSameSize() {
    bool sameSize = true;
    int size = 0;
    int i = 0;
    while (sameSize && i<listJoints->size()) {
        if(i == 0){
            size = listJoints->at(i)->getBufferPositions()->size();
        } else {
            if(listJoints->at(i)->getBufferPositions()->size() != size)
                sameSize = false;
        }
        i++;
    }
    return sameSize;
}

ServerOSC::~ServerOSC()
{
    delete(this->analyse);
    delete(this->movement);
    for(int i = 0 ; this->listJoints->size(); i++)
	delete(this->listJoints->at(i));
    delete(this->listJoints);
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	delete(this->listMovements->at(i));
    delete(this->listMovements);
}

void ServerOSC::setAmplitude(quint16 &amplitude)
{
    this->amplitude = amplitude;
}

void ServerOSC::setVitesse(float &vitesse)
{
    this->vitesse = vitesse;
}

float ServerOSC::getVitesse()
{
    return this->vitesse;
}

quint16 ServerOSC::getAmplitude()
{
    return this->amplitude;
}
