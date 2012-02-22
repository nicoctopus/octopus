#include "analyse.h"

Analyse::Analyse(){
    this->seuilFrequence=0;

    //p->playDemo(music);
}

Analyse::~Analyse(){
   delete(this->p);
    delete(this->music);
}

Analyse::Analyse(float pourc,float seuil)
{
    qDebug()<< "CREATION ANALYSE !!!!!!!! "<< endl;
    this->seuilFrequence=pourc;
    this->seuilAmplitude= seuil;
    //this->calculBITG(moves);
    this->p = new SoundPlayer(1);
    this->music = new SampleAudio("toto","../../../test.mp3",1, false);

}

void Analyse::calculBITG(QList<Movement*>* mouv){

    qint32 tailleBuffer = SIZE_MAX_BUFFERS;

    QList<Position*> *EnregistrementVite;

    QList<Position*> *ListEcartEnr = new QList<Position*>();
    QList<Position*> *ListEcartBuf = new QList<Position*>();

    qint8 nbEcarts =0;

    int tailleCourante=0;
    int indiceDepart=0;

    float moyenneGenerale=0;

    float moyenneGtemp=seuilAmplitude+1;
    quint16 decal=0;



    qint16 TempsEnregistrement;
    qint16 TempsRallonge;
    qint16 TempsReduit;

    qint16 variation;

    quint16 PasVitesse=0;
    quint16 PasDecalage=0;


    //Parcours tous les mouvements

    for(int m=0;m<mouv->size();m++){
        /*
         qDebug()<< "Joint BEFORE : "<<  mouv->at(m)->getListJointsMvt()->at(0)->getJointRef()->getNom() << endl;
        qDebug()<< "Taille Joint BEFORE : "<<  mouv->at(m)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size() << endl;

        qDebug()<< "JointBEFORE : "<<  mouv->at(m)->getListJointsMvt()->at(1)->getJointRef()->getNom() << endl;
       qDebug()<< "Taille Joint BEFORE: "<<  mouv->at(m)->getListJointsMvt()->at(1)->getJointRef()->getBufferPositions()->size() << endl;*/

        TempsEnregistrement = mouv->at(m)->getListJointsMvt()->at(0)->getListPositions()->size(); //taille originale du mouvement
        TempsRallonge = TempsEnregistrement + TempsEnregistrement*seuilFrequence; //augmentation de la frequence : borne sup.
        TempsReduit = TempsEnregistrement - TempsEnregistrement*seuilFrequence; //diminution de la frequence : borne inf.

        PasVitesse = TempsEnregistrement*(seuilFrequence/10);
        if(PasVitesse<1){
            PasVitesse = TempsEnregistrement*(seuilFrequence/10)+1;
        }

        PasDecalage = PasVitesse;

        /**
          * Boucle pour modifier la frequence :
          * On va de borne inf. a borne sup. en incrementant de PasVitesse
          */

        tailleBuffer = mouv->at(m)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size();

        //variation de frequence (taille)
        for(tailleCourante =TempsReduit; tailleCourante<=TempsRallonge; tailleCourante+=PasVitesse){
            if(tailleBuffer-tailleCourante>=0){
                variation= tailleBuffer-tailleCourante;

                /**
                  * Decaler le mouvement sur le buffer
                  */
                for(indiceDepart=0; indiceDepart<=variation; indiceDepart+=PasDecalage){

                    //qDebug() << " for variation  ..." << endl;

                    //qDebug() << "ITERATION: "<< t <<endl;
                    /*
                    //qDebug() <<"Variation : "<< variation <<endl<<endl;
                    qDebug() <<"Taille Actuelle Enregistrement : "<< T << endl ;

                    qDebug() <<"MOUVEMENT : " << m+1 << endl;
                    qDebug() <<"TAILLE MOUV : " << mouv->at(m)->getListJointsMvt()->at(0)->getListPositions()->size() << endl;
                    qDebug() <<"NBR JOINTS : " << (int)mouv->at(m)->getListJointsMvt()->size() << endl;
                    */

                    /**
                      * Parcourir tous les JointsMouvement du mouvement courant
                      */
                    for(int jt=0;jt<mouv->at(m)->getListJointsMvt()->size();jt++){

                        // copier le mouvement avec une nouvelle taille (tailleCourante)
                        EnregistrementVite = modifFreq(mouv->at(m)->getListJointsMvt()->at(jt)->getListPositions(), tailleCourante);
                        nbEcarts = NbPivots(EnregistrementVite); //nombre de cases entre chaque pivot

                        //qDebug()<< "Pivots : " << nbEcarts << endl;
                        //qDebug()<< "Taille Enregistrement Vite : "<< EnregistrementVite->size()<< endl;

                        /**
                          * Difference d'amplitude
                          */
                        //qDebug() << mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getNom() << endl;
                        //qDebug() << "taille buf : "<< mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->size() << endl;

                        for(int r=indiceDepart; r<indiceDepart+tailleCourante; r++){
                            //si le pivot suivant est accessible

                            if((r-indiceDepart)%nbEcarts==0 && (r-indiceDepart+nbEcarts < EnregistrementVite->size())){
                                Position* ecartEnr = EnregistrementVite->at(r-indiceDepart)->ecartPivot(*EnregistrementVite->at(r-indiceDepart+nbEcarts));
                                Position* ecartBuf = mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r)->ecartPivot(*mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r+nbEcarts));
                                //qDebug() << "X record : "<< mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r)->getX() << endl;
                                ListEcartEnr->append(ecartEnr);
                                ListEcartBuf->append(ecartBuf);

                            }
                        }
                        //qDebug() << "taille listEcartEnr : "<< ListEcartEnr->size() << endl;

                        //qDebug() << "taille listEcartBuf : "<< ListEcartBuf->size() << endl;


                        moyenneGenerale += EcartRelatif(ListEcartEnr,ListEcartBuf); //calculer la moyenne des ecarts du joint courant
                        //qDebug()<< "moyenne generale : "<< moyenneGenerale <<endl;

                        //si le jointMvt courant est le dernier du mouvement
                        if(jt==mouv->at(m)->getListJointsMvt()->size()-1){

                            //qDebug()<<mouv->at(m)->getListJointsMvt()->size()<<endl;
                            moyenneGenerale /= mouv->at(m)->getListJointsMvt()->size();
                            moyenneGenerale *= 100;
                            //qDebug() <<"Moyenne General : "<< moyenneGenerale<< " % "<<endl<<endl;

                            //le seuil de tolerance est respecte = le mouvement est correct
                            if(moyenneGenerale < seuilAmplitude ){
                                qDebug() << "CORRESPONDANCE OK !" << endl;
                                qDebug() << "MOYENNE G !" <<moyenneGenerale <<endl;

                                //qDebug() << "MOYENNE G temp!" <<moyenneGtemp <<endl;
                                /*if(moyenneG<moyenneGtemp){
                                    moyenneGtemp = moyenneG;
                                }else{*/

                                /* if(T<TempsEnregistrement){
                                        decal=TempsEnregistrement-T;
                                        decal=decal/2;
                                    }*/
                                //mettre le buffer du jointReference a 0
                                //for(int s=t-decal; s<t+T+decal; s++){
                                    //remettre a 0 les cases du buffer de chaque joint du mouvement valide
                                    for(int w=0; w<mouv->at(m)->getListJointsMvt()->size();w++){
                                        for(int s=indiceDepart; s<indiceDepart+tailleCourante; s++){
                                            mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setX(0);
                                            mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setDx(0);
                                            mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setY(0);
                                            mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setDy(0);
                                            mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setZ(0);
                                            mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(s)->setDz(0);
                                        }
                                    }
                                    qDebug() << "CORRESPONDANCE VRAIMENT OK :) !" << endl;
                                    p->playDemo(music);
                                    //sleep(2);
                                    //p->Stop();
                                    //sleep(1);
                                    //exit(-1);
                                    //return;
                                //}
                            }
                        }

                        ListEcartEnr->clear();
                        ListEcartBuf->clear();
                        EnregistrementVite->clear();
                    }
                    moyenneGenerale=0;
                }
            }
        }
    }
   // qDebug() << "Analyse time : " << mouv->at(0)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size() << endl;


    /*delete(ListEcartEnr);
    delete(ListEcartBuf);
    delete(EnregistrementVite);*/
}


qint8 Analyse::NbPivots(QList<Position *> * enr){

    if(enr->size()<=150){

        return 10;
    }else if(enr->size()>150){

        return 20;
    }


}

//calculer la difference entre un ecart du buffer et enregistre
float Analyse::EcartRelatif(QList<Position *> *ListEnr, QList<Position *> *ListBuf){

    if(ListEnr->size()==ListBuf->size()){

        float ecartX=0;
        float ecartY=0;
        float ecartZ=0;
        float ecartG=0;


        for(int i=0; i<ListEnr->size();i++){

            ecartX=abs(ListEnr->at(i)->getX()-ListBuf->at(i)->getX());
           // qDebug() << "ecart relatif x " << ListEnr->at(i)->getX() << "-" << ListBuf->at(i)->getX() << "/" << ListEnr->at(i)->getX() << endl;
            ecartX= ecartX / abs(ListEnr->at(i)->getX());
            //qDebug() << ecartX << endl;

            ecartY=abs(ListEnr->at(i)->getY()-ListBuf->at(i)->getY());
           // qDebug() << "ecart relatif y " << ListEnr->at(i)->getY() << "-" << ListBuf->at(i)->getY() << "/" << ListEnr->at(i)->getY() << endl;
            ecartY= ecartY / abs(ListEnr->at(i)->getY());
            //qDebug() << ecartY << endl;

            ecartZ=abs(ListEnr->at(i)->getZ()-ListBuf->at(i)->getZ());
            //qDebug() << "ecart relatif z " << ListEnr->at(i)->getZ() << "-" << ListBuf->at(i)->getZ() << "/" << ListEnr->at(i)->getZ() << endl;
            ecartZ= ecartZ / abs(ListEnr->at(i)->getZ());
            //qDebug() << ecartZ << endl;

            ecartG += (ecartX + ecartY + ecartZ)/3;


        }


        ecartG = ecartG/ListEnr->size();
        //qDebug() << "ecart g " << ecartX << endl;

        return ecartG;

    }else{
        return 2.0;
    }

}



QList<Position*> * Analyse::modifFreq(QList<Position*>* listPositionsJointMvt, int T){
    QList<Position*> *newPositionsJointMvt = new QList<Position*>();
    int j=0;
    int k=0;
    int reste=0;



    // travail sur la taille souhaitee
    if(listPositionsJointMvt->size()!=T){
        j= (listPositionsJointMvt->size())/(abs(listPositionsJointMvt->size()-T));
        reste = (listPositionsJointMvt->size()/j)-abs((listPositionsJointMvt->size()-T));
        k=j;
        j=j-1;
    }

    // Agrandir le jointMvt
    if(T >= listPositionsJointMvt->size() ){
        //parcourir les positions du jointMvt
        for(int i=0; i<listPositionsJointMvt->size(); i++){
            if(i==j && j!=0){
                newPositionsJointMvt->append(moyenne(listPositionsJointMvt->at(i-1), listPositionsJointMvt->at(i)));
                newPositionsJointMvt->append(listPositionsJointMvt->at(i));
                j=j+k;
            }else{

                newPositionsJointMvt->append(listPositionsJointMvt->at(i));
            }

        }
        //Si y'a des cases en trop, on a supprime aleatoirement
        if(reste!=0){
            for(int i=0; i<reste;i++){
                newPositionsJointMvt->removeAt(rand()%(T-reste));
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
                newPositionsJointMvt->append(listPositionsJointMvt->at(i));
            }
        }
        if(reste!=0){
            int random;
            for(int i=0; i<reste;i++){
                random = rand()%(T-1)+1;
                newPositionsJointMvt->insert(random,moyenne(listPositionsJointMvt->at(random-1), listPositionsJointMvt->at(random)));
            }
        }
    }

    //qDebug() << "Taille nouveau : "<< Temp.size() <<endl<<endl;
    return newPositionsJointMvt;
}

Position* Analyse::moyenne(Position *p1, Position *p2)
{
    return new Position(
                0,
                (p1->getX() + p2->getX()) / 2,
                (p1->getY() + p2->getY()) / 2,
                (p1->getZ() + p2->getZ()) / 2,
                (p1->getDx() + p2->getDx()) / 2,
                (p1->getDy() + p2->getDy()) / 2,
                (p1->getDz() + p2->getDz()) / 2,
                (p1->getA() + p2->getA()) /2,
                (p1->getB() + p2->getB()) /2,
                (p1->getC() + p2->getC()) /2
                );
}


