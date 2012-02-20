#include "managerjointmvt.h"

ManagerJointMvt::ManagerJointMvt() : Manager()
{
    this->initSystem();
    this->listJointsMvts = new QList<JointMvt*>();
    this->managerPosition = new ManagerPosition();
}

void ManagerJointMvt::loadAll(){
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierJointMvt.allKeys().size() ; i++)
    {
	this->listJointsMvts->append(new JointMvt(fichierJointMvt.value(fichierJointMvt.allKeys().at(i), qVariantFromValue(JointMvt())).value<JointMvt>()));
    }
    this->managerPosition->loadAll();

}

void ManagerJointMvt::saveAll()
{
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ;  i < this->listJointsMvts->size() ; i++)
	fichierJointMvt.remove(QString::number(this->listJointsMvts->at(i)->getIdJointMvt()));
    for(int i = 0 ; i < this->listJointsMvts->size() ; i++)
	this->save(this->listJointsMvts->at(i), fichierJointMvt);
    fichierJointMvt.sync();
}

void ManagerJointMvt::save(JointMvt *jointMvt, QSettings &fichierJointMvt){
    QString key = QString::number(jointMvt->getIdJointMvt());
    fichierJointMvt.setValue(key, qVariantFromValue(*jointMvt));
}

void ManagerJointMvt::save(QList<JointMvt*> *listJointsMvt)
{
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    // enregistrer en fonction du jointmvt ayant le moins de positions
    int size = listJointsMvt->at(0)->getListPositions()->size();
    for(int i=0; i<listJointsMvt->size();i++) {
	if(listJointsMvt->at(i)->getListPositions()->size() < size)
	    size = listJointsMvt->at(i)->getListPositions()->size();
    }

    for(int j = 0 ; j < listJointsMvt->size() ; j++)
    {
	JointMvt *jointMvtTemp = listJointsMvt->at(j);
	//parcourir jusqu'a la position SIZE correspondant a l'indice max du tableau de positions le plus petit
	for(int k = 0 ; k < size ; k++){
	    this->managerPosition->save(jointMvtTemp->getListPositions()->at(k), fichierPosition);
	}
	this->save(jointMvtTemp, fichierJointMvt);
    }
    fichierJointMvt.sync();
    fichierPosition.sync();
}

void ManagerJointMvt::remove(QList<JointMvt*> *listJointsMvtsToDelete)
{
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listJointsMvtsToDelete->size() ; i++)
    {
	JointMvt *jointMvtTemp = listJointsMvtsToDelete->at(i);
	//On supprime toutes les positions du joint mouvement dans le fichier
	this->managerPosition->remove(jointMvtTemp->getListPositions());
	//on supprime chaque position dans la liste de position du joint mouvement
	for(int i = 0 ; i < jointMvtTemp->getListPositions()->size() ; i++)
	    jointMvtTemp->getListPositions()->removeAt(i);
	//on supprime le joint mouvement du fichier
	fichierJointMvt.remove(QString::number(jointMvtTemp->getIdJointMvt()));
	//on supprime le dernier joint du fichier
	fichierJointMvt.remove(QString::number(this->listJointsMvts->last()->getIdJointMvt()));
	//on update son id avec celle du joint mouvement a supprimer
	this->listJointsMvts->last()->updateIdJointMvt(jointMvtTemp->getIdJointMvt());
	//on le save a nouveau update en memoire
	this->save(this->listJointsMvts->last(), fichierJointMvt);
	JointMvt::idJointMvtStatic--;
    }
    fichierJointMvt.sync();
}

QList<JointMvt*>* ManagerJointMvt::getListJointsMvts()
{
    return this->listJointsMvts;
}

ManagerPosition* ManagerJointMvt::getManagerPosition()
{
    return this->managerPosition;
}

void ManagerJointMvt::initSystem()
{
    qRegisterMetaTypeStreamOperators<JointMvt>("JointMvt");
    qMetaTypeId<JointMvt>();
}

ManagerJointMvt::~ManagerJointMvt()
{
    for(int i = 0 ; i < this->listJointsMvts->size() ; i++)
	delete(this->listJointsMvts->at(i));
    delete(this->listJointsMvts);
    delete(this->managerPosition);
}
