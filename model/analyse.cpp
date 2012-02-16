#include "analyse.h"

Analyse::Analyse(){
    this->pourcentage=0;

    //p->playDemo(music);
}

Analyse::~Analyse(){

}

Analyse::Analyse(float pourc,float seuil)
{
    qDebug()<< "CREATION ANALYSE !!!!!!!! "<< endl;
    this->pourcentage=pourc;
    this->seuil= seuil;
    //this->calculBITG(moves);
    this->p = new SoundPlayer(1);
    this->music = new SampleAudio("toto","../../../test.mp3",1, false);

}

void Analyse::calculBITG(QList<Movement*>* mouv){


    qint32 tailleBuffer = SIZE_MAX_BUFFERS;

    /* for(int k=0; k<mouv->size();k++) { // chaque Mvt

    }*/



    QList<Position*> *EnregistrementVite;

    QList<Position*> *ListEcartEnr = new QList<Position*>();
    QList<Position*> *ListEcartBuf = new QList<Position*>();

    qint8 nbEcarts =0;

    int m=0;
    int jt=0;
    int T=0;
    int t=0;
    int r=0;



    float moyenneJ=0;
    float moyenneG=0;

    float moyenneGtemp=seuil+1;
    quint16 decal=0;



    qint16 TempsEnregistrement;
    qint16 TempsRallonge;
    qint16 TempsReduit;

    qint16 variation;

    quint16 PasVitesse=0;
    quint16 PasDecalage=0;


    //Parcours tous les mouvements

    for(m=0;m<mouv->size();m++){
        /*
         qDebug()<< "Joint BEFORE : "<<  mouv->at(m)->getListJointsMvt()->at(0)->getJointRef()->getNom() << endl;
        qDebug()<< "Taille Joint BEFORE : "<<  mouv->at(m)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size() << endl;

        qDebug()<< "JointBEFORE : "<<  mouv->at(m)->getListJointsMvt()->at(1)->getJointRef()->getNom() << endl;
       qDebug()<< "Taille Joint BEFORE: "<<  mouv->at(m)->getListJointsMvt()->at(1)->getJointRef()->getBufferPositions()->size() << endl;*/

        TempsEnregistrement = mouv->at(m)->getListJointsMvt()->at(0)->getListPositions()->size(); //taille originale du mouvement
        TempsRallonge = TempsEnregistrement + TempsEnregistrement*pourcentage; //augmentation de la frequence : borne sup.
        TempsReduit = TempsEnregistrement - TempsEnregistrement*pourcentage; //diminution de la frequence : borne inf.

        PasVitesse = TempsEnregistrement*(pourcentage/10);
        if(PasVitesse<1){
            PasVitesse = TempsEnregistrement*(pourcentage/10)+1;
        }

        PasDecalage = PasVitesse;

        /**
          * Boucle pour modifier la frequence :
          * On va de borne inf. a borne sup. en incrementant de PasVitesse
          */

        tailleBuffer = mouv->at(m)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size();

        for(T =TempsReduit;T<=TempsRallonge;T+=PasVitesse){
            if(tailleBuffer-T>=0){
                variation= tailleBuffer-T;

                /**
                  * Decaler le mouvement sur le buffer
                  */

                for(t=0; t<=variation; t+=PasDecalage){

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
                    for(jt=0;jt<mouv->at(m)->getListJointsMvt()->size();jt++){



                        EnregistrementVite = modifFreq(mouv->at(m)->getListJointsMvt()->at(jt)->getListPositions(),T);


                        nbEcarts = NbPivots(EnregistrementVite);

                        /**
                          * Difference d'amplitude
                          */
                        for(r=t; r<t+T; r++){

                            if(r-t+nbEcarts < EnregistrementVite->size()){

                                ListEcartEnr->append(EnregistrementVite->at(r-t)->EcartPivot(*EnregistrementVite->at(r-t+nbEcarts)));
                                ListEcartBuf->append(mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r)->EcartPivot(*mouv->at(m)->getListJointsMvt()->at(jt)->getJointRef()->getBufferPositions()->at(r+nbEcarts)));

                            }

                        }

                        moyenneJ = EcartRelatif(ListEcartEnr,ListEcartBuf);

                        moyenneG += moyenneJ;

                        if(jt==mouv->at(m)->getListJointsMvt()->size()-1){


                            moyenneG=moyenneG/mouv->at(m)->getListJointsMvt()->size();
                            moyenneG=moyenneG*100;


                            //qDebug() << "ITERATION: "<< t <<endl;
                            //qDebug() <<"Taille Actuelle Enregistrement : "<< T << endl ;

                            qDebug() <<"Moyenne General : "<< moyenneG<< " % "<<endl<<endl;

                            if(moyenneG < seuil ){
                                qDebug() << "CORRESPONDANCE OK !" << endl;
                                qDebug() << "MOYENNE G !" <<moyenneG <<endl;
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

                                for(int w=0; w<mouv->at(m)->getListJointsMvt()->size();w++){

                                    for(int s=t; s<t+T; s++){
                                        mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(w)->setX(0);
                                        mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(w)->setDx(0);
                                        mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(w)->setY(0);
                                        mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(w)->setDy(0);
                                        mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(w)->setZ(0);
                                        mouv->at(m)->getListJointsMvt()->at(w)->getJointRef()->getBufferPositions()->at(w)->setDz(0);
                                    }
                                }
                                moyenneJ=0;
                                moyenneG=0;
                                qDebug() << "CORRESPONDANCE VRAIMENT OK :) !" << endl;
                                p->playDemo(music);
                                sleep(9);
                                //exit(-1);
                                //return;
                                //}
                            }
                        }


                       moyenneJ=0;
                       ListEcartEnr->clear();
                       ListEcartBuf->clear();
                    }
                    moyenneG=0;
                }
            }
        }
    }


}


qint8 Analyse::NbPivots(QList<Position *> * enr){

    if(enr->size()<=150){

        return 10;
    }else if(enr->size()>150){

        return 20;
    }


}

float Analyse::EcartRelatif(QList<Position *> *ListEnr, QList<Position *> *ListBuf){

    if(ListEnr->size()==ListBuf->size()){

        float ecartX=0;
        float ecartY=0;
        float ecartZ=0;
        float ecartG=0;


        for(int i=0; i<ListEnr->size();i++){

            ecartX=abs(ListEnr->at(i)->getX()-ListBuf->at(i)->getX());
            ecartX= ecartX / abs(ListEnr->at(i)->getX());

            ecartY=abs(ListEnr->at(i)->getY()-ListBuf->at(i)->getY());
            ecartY= ecartY / abs(ListEnr->at(i)->getY());

            ecartZ=abs(ListEnr->at(i)->getZ()-ListBuf->at(i)->getZ());
            ecartZ= ecartZ / abs(ListEnr->at(i)->getZ());

            ecartG += (ecartX + ecartY + ecartZ)/3;



            ecartX=ecartY=ecartZ=0;

        }

        ecartG = ecartG/ListEnr->size();

        return ecartG;


    }else{
        return 2;
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


