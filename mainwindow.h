
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "model/controller.h"
#include "view/blackboard.h"
#include "view/stickman.h"




namespace Ui {
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //void loadLeftTree();
    //QTreeWidgetItem* findItemMovement();
    ~MainWindow();
    Controller* getController();
    //Methodes d'initialisation
    void initTreeView();
    void initBlackBoard();
    QString textDisplay(Movement *movement);
    QString textDisplay(ClientOSC *port);
    QString textDisplay(SampleAudio *sampleAudio);


public slots :
    void slotTimeOutRecord();
    void slotNewSelectionOnStickMan();
    void save(Movement *movement);
    void save(SampleAudio *sampleAudio);
    void save(ClientOSC *clientOSC);
    void slotPlayPause();
    void slotStop();
    void slotNewSelectionOnBlackBoard();
    int slotLockNodesForNewMouvement();
    void slotUnlockStickMan();
    void slotDisplayInfos(QTreeWidgetItem* item,int column);
    void slotDisplayInfos();
    void slotRecordNewMovement();
    void slotValidNewMovement();
    void slotEscNewMovement();
    void slotLeftTreeDoubleClicked(QTreeWidgetItem* item, int column);
    void decocherCheckBoxLink();
    void boutonAddSample();
    void updateLCDTimer();


signals :
    void refreshLeftTree();
    void refreshBlackBoard();
    void sigPlaySample();
    void emitTime(QString currentTime);


private:
    Ui::MainWindow *ui;
    Controller *controller;
    Movement *movement;
    QTimer *timer;
    //QMap<qint16, Movement*> movementsMap; //Pour test ( correspond a la map dans le MANAGER
    //QMap<qint16, QTreeWidgetItem*> itemsLeftMap; //liste des QtreeWidgetItems indexé par l'id des mvt associés
    //QMap<QTreeWidgetItem*, Movement*> mapTreeItemsMovement; //map qui fait correspondre le pointeur sur le move avec le pointeur sur l'item de LeftTree

};

#endif // MAINWINDOW_H
