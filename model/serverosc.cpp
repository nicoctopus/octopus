#include "serverosc.h"

ServerOSC::ServerOSC()
{
    this->recording = false;
}

ServerOSC::ServerOSC(const quint16 &portNb, const bool &active) : Port(portNb, active){
    this->nameElement = QString(portNb);
    this->recording = false;
}

const char* ServerOSC::className() {
    return "ServerOSC";
}


void ServerOSC::run() {
    socket->bindTo(this->portNumber);
    //socket ouverte
    if(socket->isOk()) {
	while (socket->isOk()) {
	    while(runnable) {
		if(!isRecording()) // si on enregistre pas
		    fillBuffers(); // on remplit les buffers
		else
		    record(); // sinon on enregistre le mouvement
	    }
	}
    }
    exec();
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

    if (socket->receiveNextPacket(1 /* timeout, in ms */)) {
	reader->init(socket->packetData(), socket->packetSize());
	while (reader->isOk() && (message = reader->popMessage()) != 0) {
	    /** track joints
	      * repere : body
	      * Donnees : x, y, z
	      **/
	    if (message->match("/righthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		//chercher le joint correspondant dans la liste de tous les joints
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "righthand"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/lefthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "lefthand"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/rightelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "rightelbow"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/leftelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "leftelbow"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/rightfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "rightfoot"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/leftfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "leftfoot"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/rightknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "rightknee"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/leftknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "leftknee")
			this->listJoints->at(i)->addPosition(x,y,z);
		}
	    }
	    else if (message->match("/head_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "head"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/torso_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<this->listJoints->size(); i++){
		    if(this->listJoints->at(i)->getNom() == "torso"){
			this->listJoints->at(i)->addPosition(x,y,z);
		    }
		}
	    }
	    else if (message->match("/tracking_skeleton").popBool(d).isOkNoMoreArgs()) {
		//tracking ok ...
	    }
	    else {
		//qDebug() << "Server Error : unknown type of data " << endl;
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

    if (socket->receiveNextPacket(1 /* timeout, in ms */)) {
	reader->init(socket->packetData(), socket->packetSize());
	while (reader->isOk() && (message = reader->popMessage()) != 0) {
	    /** track joints
	      * repere : body
	      * Donnees : x, y, z
	      */
	    QList<JointMvt*>* listJointsMvt = this->listMovements->last()->getListJointsMvt();
	    if (message->match("/righthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		//chercher le joint correspondant dans le mouvement
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "righthand"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/lefthand_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "lefthand"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/rightelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "rightelbow"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/leftelbow_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "leftelbow"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/rightfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "rightfoot"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/leftfoot_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "leftfoot"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/rightknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "rightknee"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/leftknee_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "leftknee")
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		}
	    }
	    else if (message->match("/head_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "head"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/torso_pos_body").popFloat(x).popFloat(y).popFloat(z).isOkNoMoreArgs()) {
		for (int i=0; i<listJointsMvt->size(); i++){
		    if(listJointsMvt->at(i)->getJointRef()->getNom() == "torso"){
			listJointsMvt->at(i)->addPosition(new Position(listJointsMvt->at(i)->getIdJointMvt(),x,y,z,0,0,0));
		    }
		}
	    }
	    else if (message->match("/tracking_skeleton").popBool(d).isOkNoMoreArgs()) {
		//tracking ok ...
	    }
	    else {
		//qDebug() << "Server Error : unknown type of data" << endl;
	    }
	}
    }
}

void ServerOSC::setListJoints(QList<Joint *> *list) {
    this->listJoints = list;
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

