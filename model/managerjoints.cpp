#include "managerjoints.h"

ManagerJoints::ManagerJoints() : Manager()
{
    this->listJoints = new QList<Joint*>();

    Joint* lefthand = new Joint("lefthand");
    Joint* leftelbow = new Joint("leftelbow");
    Joint* leftknee = new Joint("leftknee");
    Joint* leftfoot = new Joint("leftfoot");
    Joint* righthand = new Joint("righthand");
    Joint* rightelbow = new Joint("rightelbow");
    Joint* rightknee = new Joint("rightknee");
    Joint* rightfoot = new Joint("rightfoot");
    Joint* head = new Joint("head");
    Joint* torso = new Joint("torso");
/*
    lefthand->initializeBuffer();
    leftelbow->initializeBuffer();
    leftknee->initializeBuffer();
    leftfoot->initializeBuffer();
    righthand->initializeBuffer();
    rightelbow->initializeBuffer();
    rightknee->initializeBuffer();
    rightfoot->initializeBuffer();
    head->initializeBuffer();
    torso->initializeBuffer();
*/
    this->listJoints->append(lefthand);
    this->listJoints->append(leftelbow);
    this->listJoints->append(leftknee);
    this->listJoints->append(leftfoot);
    this->listJoints->append(righthand);
    this->listJoints->append(rightelbow);
    this->listJoints->append(rightknee);
    this->listJoints->append(rightfoot);
    this->listJoints->append(head);
    this->listJoints->append(torso);

    //Initialisation de la serialisation des joints
    this->initSystem();
    //Chargement donnees
    //std::cout << std::endl << "Chargement de la liste des joints..." << std::endl;
    //this->saveAll();
    //this->loadAll();
    //std::cout << "...Chargement de la liste des joints terminé" << std::endl;
}

Joint* ManagerJoints::getJoint(QString name)

{

    for(int i = 0 ; i < this->listJoints->size() ; i++)

        if(this->listJoints->at(i)->getNom() == name)

            return this->listJoints->at(i);

    return NULL;

}

void ManagerJoints::loadAll()
{
    std::cout << "   debut loadJoint()..." << std::endl;
    this->listJoints = new QList<Joint*>();
    //CHARGEMENT LISTE JOINTS
    QSettings fichierJoint("joint.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierJoint.allKeys().length() ; i++)
	this->listJoints->append(new Joint(fichierJoint.value(fichierJoint.allKeys().at(i), qVariantFromValue(Joint())).value<Joint>()));

    std::cout << "   ...fin loadJoint()" << std::endl;
}

void ManagerJoints::saveAll()
{
    std::cout << "   debut saveJoint()..." << std::endl;

    //SAUVEGARDE DE LA LISTE DES JOINTS SANS LA LISTE DES POSITIONS (BUFFER)
    QFile::remove("joint.ini");
    QSettings fichierJoint("joint.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->listJoints->length() ; i++)
    {
	//std::cout << "Affichage nom du joint : " << this->listJoints->at(i)->getNom().toStdString() << std::endl;
	//std::cout << "Affichage id du joint : " << this->listJoints->at(i)->getId() << std::endl;
	QString key(this->listJoints->at(i)->getId());
	fichierJoint.setValue(key, qVariantFromValue(*(this->listJoints->at(i))));
    }
    fichierJoint.sync();
    std::cout << "   ...fin saveJoint()" << std::endl;
}

void ManagerJoints::initSystem()
{
    qRegisterMetaTypeStreamOperators<Joint>("Joint");
    qMetaTypeId<Joint>();
}

QList<Joint*>* ManagerJoints::getListJoints()
{
    return this->listJoints;
}
