
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QStringList>
#include "model/controller.h"
#include "view/blackboard.h"
#include "view/stickman.h"
#include "view/courbes.h"
#include "view/configanalyse.h"
#include "view/configrecordmouvement.h"
#include "view/widgetcourbes.h"

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
    void linkActionsMenu();

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
    void updateLCDTimerSong();
    void updateLCDTimerLive();
    void updateLabelTimeRecord();
    void slotDisplayInfos(QGraphicsItem* item);
    void slotStartLivePerformance();
    void slotChangeMovementForCourbe(QString);
    void slotSetSampleResetMode(int);
    void slotChangeConfigAnalyse(float, quint16);
    void slotConfigTempsRecord(float, float);
    void slotSetSampleLoopMode(int);
    void slotSetSampleSpinBox(QString);
    void slotRemoveButton();
    void slotMoveStickman();
    void about();
    void slotAboutToQuit();


signals :
    void refreshLeftTree();
    void refreshBlackBoard();
    void sigPlaySample();
    void emitTime(QString);
    void emitTimeLabelRecord(QString);
    void emitTimeLive(QString);
    void sigMoveStickman(Movement* movement);
    void clearComboBox();



private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Controller *controller;
    Movement *movement;
    QTimer *timer, *timerLive;
    SampleAudio* audioTemp;
    Movement * movTemp;
    ClientOSC * clientTemp;
    ConfigAnalyse *configAnalyse;
    ConfigRecordMouvement *configRecordMouvement;
    float tempLatence;
    float tempRecordMovement;
    float tempRecordMovementPartantDe0;
    qint16 timeTimerLive;
    //QMap<qint16, Movement*> movementsMap; //Pour test ( correspond a la map dans le MANAGER
    //QMap<qint16, QTreeWidgetItem*> itemsLeftMap; //liste des QtreeWidgetItems index par l'id des mvt associs
    //QMap<QTreeWidgetItem*, Movement*> mapTreeItemsMovement; //map qui fait correspondre le pointeur sur le move avec le pointeur sur l'item de LeftTree

};

#endif // MAINWINDOW_H
