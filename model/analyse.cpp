#include "analyse.h"

Analyse::Analyse(){
    this->seuilFrequence=0;
}

Analyse::~Analyse(){
    delete this->playerlive;
    delete this->ListEcartEnr;
    delete this->ListEcartBuf;
    /*int sizeTemp = this->listMovementToAnalyze->size();
    for(int i = 0 ; i < sizeTemp ; i++)
 delete this->listMovementToAnalyze->at(i);*/
    delete this->listMovementToAnalyze;
}

Analyse::Analyse(float pourc,float seuil, QList<Movement*>* listMovements)
{
    qDebug()<< "CREATION ANALYSE !!!!!!!! "<< endl;
    this->seuilFrequence=pourc;
    this->seuilAmplitude= seuil;
    this->listMovementToAnalyze = listMovements;
    this->ListEcartEnr = new QList<Position*>();
    this->ListEcartBuf = new QList<Position*>();
    this->playerlive = new SoundPlayer(32);

}

void Analyse::calculBITG(){

     qint32 tailleBuffer;
    if(this->listMovementToAnalyze->size()!=0){
     tailleBuffer = this->listMovementToAnalyze->at(0)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size() + (this->listMovementToAnalyze->at(0)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size() / 2) ;
    }else{
     tailleBuffer = SIZE_MAX_BUFFERS;
    }

    qint8 nbEcarts =0;
    qint8 difNbEcarts= 0;

    int ecartCourant=0;
    int indiceDepart = SIZE_MAX_BUFFERS - tailleBuffer;

    float moyenneGenerale=0;

    //float moyenneGtemp=seuilAmplitude+1;
    //quint16 decal=0;

    qint16 TempsEnregistrement;
    //qint16 TempsRallonge;
    //qint16 TempsReduit;

    qint16 variation;

    //quint16 PasVitesse=0;
    quint16 PasDecalage=0;



    //Parcourir tous les mouvements
    for(int m=0;m<this->listMovementToAnalyze->size();m++){ //MEM OK//

	TempsEnregistrement = this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(0)->getListPositions()->size(); //taille originale du mouvement
	//TempsRallonge = TempsEnregistrement + TempsEnregistrement*seuilFrequence; //augmentation de la frequence : borne sup.
	//TempsReduit = TempsEnregistrement - TempsEnregistrement*seuilFrequence; //diminution de la frequence : borne inf.

	/*
 PasVitesse = TempsEnregistrement*(seuilFrequence/10);
 if(PasVitesse<1){
     PasVitesse = TempsEnregistrement*(seuilFrequence/10)+1;
 }*/

	nbEcarts=10;
	difNbEcarts = nbEcarts* this->seuilFrequence;

	//qDebug()<< "Seuil frequence : "<< this->seuilFrequence<<endl;
	//qDebug()<< "DifNbEcarts : "  << difNbEcarts << endl;
	//qDebug()<< "NbEcarts : "  << nbEcarts << endl;


	PasDecalage = 2;

	/**
   * Boucle pour modifier la frequence :
   * On va de borne inf. a borne sup. en incrementant de PasVitesse
   */
	tailleBuffer = this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size();
	for(ecartCourant =nbEcarts-difNbEcarts; ecartCourant<=nbEcarts+difNbEcarts; ecartCourant++){ //MEM OK//
	    //le mouvement en cours d'analyse "rentre" dans le buffer

	    //qDebug()<< "EcartCourant : "  << ecartCourant << endl;

	    //qDebug()<< "ListEcartEnr taille : "<< this->ListEcartBuf->size()<<endl;
	    //qDebug()<< "ListEcartBuf taille : "<< this->ListEcartEnr->size()<<endl;

	    if(tailleBuffer-TempsEnregistrement>=0){
		variation= tailleBuffer-TempsEnregistrement;

		/**
    * Decaler le mouvement sur le buffer
    */
		for(indiceDepart=0; indiceDepart<=variation; indiceDepart+=PasDecalage){
		    /**
 * Parcourir tous les JointsMouvement du mouvement courant
 */
		    for(int jt=0;jt<this->listMovementToAnalyze->at(m)->getListJointsMvt()->size();jt++){

			// copier le mouvement avec une nouvelle taille (tailleCourante)
			//modifFreq(this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions(), ecartCourant); //MEM OK//
			//sleep(1);



			/**
     * Difference d'amplitude
     */

			for(int r=0; r<TempsEnregistrement; r+=nbEcarts){
			    //si le pivot suivant est accessible
			    if((r)%nbEcarts==0 && (r+nbEcarts < this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->size())){

				Position* posEcartEnr = new Position();

				ListEcartEnr->append(this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->at(r)->ecartPivot(this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->at(r+nbEcarts), posEcartEnr));

			    }
			}

			for(int r=indiceDepart; r<indiceDepart+TempsEnregistrement; r+=ecartCourant){
			    //si le pivot suivant est accessible
			    if((r-indiceDepart)%ecartCourant==0 && (r-indiceDepart+ecartCourant < this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->size())){


				Position* posEcartBuf = new Position();

				ListEcartBuf->append(this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r)->ecartPivot(this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r+ecartCourant), posEcartBuf));
			    }
			}

			if(ListEcartEnr->size() < ListEcartBuf->size())
			{
			    int size = this->ListEcartBuf->size();
			    for(int i = this->ListEcartEnr->size() ; i < size ; i++)
			    {
				delete this->ListEcartBuf->last();
				this->ListEcartBuf->removeLast();
			    }
			}
			else
			{
			    int size = this->ListEcartEnr->size();
			    for(int i = this->ListEcartBuf->size() ; i < size ; i++)
			    {
				delete this->ListEcartEnr->last();
				this->ListEcartEnr->removeLast();
			    }
			}

			//calculer la moyenne des ecarts du joint courant

			moyenneGenerale += ecartRelatif();

			//si le jointMvt courant est le dernier du mouvement
			if(jt==this->listMovementToAnalyze->at(m)->getListJointsMvt()->size()-1){
			    moyenneGenerale /= this->listMovementToAnalyze->at(m)->getListJointsMvt()->size();
			    moyenneGenerale *= 100;
			    /*if(moyenneGenerale< 60){
    qDebug() <<"Moyenne General : "<< moyenneGenerale<< " % "<<endl<<endl;
       }*/
			    //le seuil de tolerance est respecte = le mouvement est correct
			    if(moyenneGenerale < seuilAmplitude ){
				qDebug()<< "   Mouvement en cours d'Analyse : "<< this->listMovementToAnalyze->at(m)->getName()<<endl;
				qDebug() << "   CORRESPONDANCE OK !" << endl;
				qDebug() << "   MOYENNE G !" <<moyenneGenerale <<endl;

				//qDebug() << "MOYENNE G temp!" <<moyenneGtemp <<endl;
				//if(moyenneG<moyenneGtemp){
				//    moyenneGtemp = moyenneG;
				//}else{


				//    if(T<TempsEnregistrement){
				//        decal=TempsEnregistrement-T;
				//        decal=decal/2;
				//    }

				//mettre le buffer du jointReference a 0
				//for(int s=t-decal; s<t+T+decal; s++){
				//remettre a 0 les cases du buffer de chaque joint du mouvement valide
				for(int w=0; w<this->listMovementToAnalyze->at(m)->getListJointsMvt()->size();w++){
				    for(int s=indiceDepart; s<indiceDepart+TempsEnregistrement; s++){
					this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setX(0);
					this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setY(0);
					this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setZ(0);

				    }
				    qDebug()<< "Remise 0"<< endl;
				}
				qDebug() << "   Nom du Mouv : " << this->listMovementToAnalyze->at(m)->getName() << endl;
				qDebug()<< "   Nom du Sample : "<< this->listMovementToAnalyze->at(m)->getSampleAudio()->getName()<< endl;


				this->playerlive->play(this->listMovementToAnalyze->at(m)->getSampleAudio());

				//sleep(2);
				//p->Stop();
				//sleep(1);
				//exit(-1);

				/**
      *On détruit les pointeurs et on nettoie les listes
      **/
				int sizeTemp = this->ListEcartEnr->size();
				for(int i=0; i<sizeTemp; i++)
				{
				    delete ListEcartEnr->last();
				    ListEcartEnr->removeLast();
				}
				sizeTemp = this->ListEcartBuf->size();
				for(int i=0; i<sizeTemp; i++)
				{
				    delete ListEcartBuf->last();
				    ListEcartBuf->removeLast();
				}

				//delete EnregistrementVite;
				moyenneGenerale=0;

				return;
			    }
			}

			/**
     * Detruire le contenu des tableaux temporaires
     */

			int sizeTemp = this->ListEcartEnr->size();
			for(int i=0; i<sizeTemp; i++)
			{
			    delete ListEcartEnr->last();
			    ListEcartEnr->removeLast();
			}
			sizeTemp = this->ListEcartBuf->size();
			for(int i=0; i<sizeTemp; i++)
			{
			    delete ListEcartBuf->last();
			    ListEcartBuf->removeLast();
			}
		    }
		    moyenneGenerale=0;
		}
	    }
	}
    }

    //qDebug() << "FIN ANALYSE" << endl;

}


/*

qint8 Analyse::NbPivots(QList<Position *> * enr){

    if(enr->size()<=150){

 return 10;
    }else if(enr->size()>150){

 return 20;
    }


}
*/


//calculer la difference entre un ecart du buffer et enregistre
float Analyse::ecartRelatif(){


    if(this->ListEcartEnr->size()==ListEcartBuf->size()){


	float ecartX=0;
	float ecartY=0;
	float ecartZ=0;
	float ecartG=0;
	QList<float> listMoyenneEcartPivot;

	for(int i=0; i<this->ListEcartEnr->size();i++){

	    ecartX=abs(this->ListEcartEnr->at(i)->getX()-this->ListEcartBuf->at(i)->getX());
	    // qDebug() << "ecart relatif x " << ListEnr->at(i)->getX() << "-" << ListBuf->at(i)->getX() << "/" << ListEnr->at(i)->getX() << endl;
	    ecartX= ecartX / abs(this->ListEcartEnr->at(i)->getX());
	    //qDebug() << ecartX << endl;

	    ecartY=abs(this->ListEcartEnr->at(i)->getY()-this->ListEcartBuf->at(i)->getY());
	    // qDebug() << "ecart relatif y " << ListEnr->at(i)->getY() << "-" << ListBuf->at(i)->getY() << "/" << ListEnr->at(i)->getY() << endl;
	    ecartY= ecartY / abs(this->ListEcartEnr->at(i)->getY());
	    //qDebug() << ecartY << endl;

	    ecartZ=abs(this->ListEcartEnr->at(i)->getZ()-this->ListEcartBuf->at(i)->getZ());
	    //qDebug() << "ecart relatif z " << ListEnr->at(i)->getZ() << "-" << ListBuf->at(i)->getZ() << "/" << ListEnr->at(i)->getZ() << endl;
	    ecartZ= ecartZ / abs(this->ListEcartEnr->at(i)->getZ());
	    //qDebug() << ecartZ << endl;

	    listMoyenneEcartPivot.append((ecartX + ecartY + ecartZ)/3);
	    ecartG += (ecartX + ecartY + ecartZ)/3;

	    //qDebug() << i << ": " << (ecartX + ecartY + ecartZ)/3 << endl;

	}
	ecartG = ecartG/listMoyenneEcartPivot.size();

	/*for(int i = 0 ; i < listMoyenneEcartPivot.size() ; i++)
 {
     if(listMoyenneEcartPivot.size() >= 3)
     {
  if(i == 0)
  {
      if(listMoyenneEcartPivot.at(i) > 1 && listMoyenneEcartPivot.at(i + 1) <=1 && listMoyenneEcartPivot.at(i + 2) <= 1)
   qDebug() << i << " trop ecart prmeiere moyenne " << listMoyenneEcartPivot.at(i)  << endl;
      else{
   //qDebug() << i << " ok " << listMoyenneEcartPivot.at(i) << endl;
   ecartG += listMoyenneEcartPivot.at(i);
   compteur++;
      }
  }
  else if(i == listMoyenneEcartPivot.size() - 1)
  {
      if(listMoyenneEcartPivot.at(i) > 1 && listMoyenneEcartPivot.at(i - 1) <=1 && listMoyenneEcartPivot.at(i - 2) <= 1)
   qDebug() << i <<" trop ecart a la size " << listMoyenneEcartPivot.at(i) << endl;
      else{
   //qDebug() << i << " ok " << listMoyenneEcartPivot.at(i) << endl;
   ecartG += listMoyenneEcartPivot.at(i);
   compteur++;
      }
  }
  else{
      if(listMoyenneEcartPivot.at(i) > 1 && listMoyenneEcartPivot.at(i - 1) <=1 && listMoyenneEcartPivot.at(i + 1) <= 1)
   qDebug() << i <<" trop ecart cas normal " << listMoyenneEcartPivot.at(i) << endl;
      else{
   //qDebug() <<i << " ok " << listMoyenneEcartPivot.at(i) << endl;
   ecartG += listMoyenneEcartPivot.at(i);
   compteur++;
      }
  }
     }
 }
 if(compteur > 0)
     ecartG = ecartG/compteur;
 //qDebug() << "ecart g " << ecartX << endl;
*/
	return ecartG;

    }else{
	return 200.0;
    }

}


/*
void Analyse::modifFreq(QList<Position*>* listPositionsJointMvt, int T){
    int j=0;
    int k=0;
    int reste=0;

    // travail sur la taille souhaitee
    if(listPositionsJointMvt->size()!=T){
 j= (listPositionsJointMvt->size())/(abs(listPositionsJointMvt->size()-T));
 reste = (listPositionsJointMvt->size()/j)-abs((listPositionsJointMvt->size()-T));
 k=j;
 j--;
    }

    // Agrandir le jointMvt
    if(T >= listPositionsJointMvt->size() ){
 //parcourir les positions du jointMvt
 for(int i=0; i<listPositionsJointMvt->size(); i++){
     if(i==j && j!=0){
  this->this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->append(moyenne(listPositionsJointMvt->at(i-1), listPositionsJointMvt->at(i)));
  this->this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->append(new Position(*listPositionsJointMvt->at(i)));
  j=j+k;
     }else{

  this->this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->append(new Position(*listPositionsJointMvt->at(i)));
     }

 }
 //Si y'a des cases en trop, on a supprime aleatoirement
 if(reste!=0){
     for(int i=0; i<reste;i++){
  this->this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->removeAt(rand()%(T-reste));
     }
 }

    }
    // Retrecir le jointMvt
    else{
 //parcourir les positions du jointMvt
 for(int i=0; i<listPositionsJointMvt->size(); i++){
     if(i==j){
  j=j+k;
     }else{
  this->this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->append(new Position(*listPositionsJointMvt->at(i)));
     }
 }
 if(reste!=0){
     int random;
     for(int i=0; i<reste;i++){
  random = rand()%(T-1)+1;
  this->this->listMovementToAnalyze->at(m)->getListJointsMvt()->at(jt)->getListPositions()->insert(random,moyenne(listPositionsJointMvt->at(random-1), listPositionsJointMvt->at(random)));
     }
 }
    }

    //qDebug() << "Taille nouveau : "<< Temp.size() <<endl<<endl;
}

*/

/*Position* Analyse::moyenne(Position const* p1, Position const* p2)
{
    return new Position(
  (p1->getX() + p2->getX()) / 2,
  (p1->getY() + p2->getY()) / 2,
  (p1->getZ() + p2->getZ()) / 2,
  (p1->getA() + p2->getA()) /2,
  (p1->getB() + p2->getB()) /2,
  (p1->getC() + p2->getC()) /2
  );
}*/


