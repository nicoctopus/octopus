
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QStringList>
#include "model/controller.h"
#include "view/blackboard.h"
#include "view/stickman.h"
#include "view/courbes.h"




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
    void initCourbes();
    QString textDisplay(Movement *movement);
    QString textDisplay(ClientOSC *port);
    QString textDisplay(SampleAudio *sampleAudio);
    void movingStickMan();
    void fillComboBox();
    void refreshCourbes(Movement* movement);

public slots :
    void slotTimeOutRecord();
    void slotNewSelectionOnStickMan();
    void save(Movement *movement);
    void save(SampleAudio *sampleAudio);
    void save(ClientOSC *clientOSC);
    void remove(Movement *movement);
    void remove(SampleAudio *sampleAudio);
    void remove(ClientOSC *clientOSC);
    void slotPlayPause();
    void slotStop();
    int slotLockNodesForNewMouvement();
    void slotUnlockStickMan();
    void slotDisplayInfos(QTreeWidgetItem* item,int column);
    //void slotDisplayInfos();
    void slotRecordNewMovement();
    void slotValidNewMovement();
    void slotEscNewMovement();
    void slotLeftTreeDoubleClicked(QTreeWidgetItem* item, int column);
    void boutonAddSample();
    void updateLCDTimer();
    void slotDisplayInfos(QGraphicsItem* item);
    void slotStartLivePerformance();
    void slotChangeMovementForCourbe(QString);
    void slotSetSampleResetMode(int);


signals :
    void refreshLeftTree();
    void refreshBlackBoard();
    void sigPlaySample();
    void emitTime(QString currentTime);
    void sigMoveStickman(Movement* movement);
    void clearComboBox();


private:
    Ui::MainWindow *ui;
    Controller *controller;
    Movement *movement;
    QTimer *timer;
    SampleAudio* audioTemp;
    //QMap<qint16, Movement*> movementsMap; //Pour test ( correspond a la map dans le MANAGER
    //QMap<qint16, QTreeWidgetItem*> itemsLeftMap; //liste des QtreeWidgetItems index� par l'id des mvt associ�s
    //QMap<QTreeWidgetItem*, Movement*> mapTreeItemsMovement; //map qui fait correspondre le pointeur sur le move avec le pointeur sur l'item de LeftTree

};

#endif // MAINWINDOW_H
