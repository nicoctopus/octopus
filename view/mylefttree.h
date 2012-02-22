#ifndef MYLEFTTREE_H
#define MYLEFTTREE_H
#include <QWidget>
#include <QtGui>
#include <QTreeWidget>
#include "../model/movement.h"
#include "../model/sample.h"
#include "../model/clientosc.h"

class MyLeftTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MyLeftTree(QWidget *parent = 0);

    QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;
    void setListMovements(QList<Movement*> *listMovements);
    void setListPorts(QList<ClientOSC*> *listPorts);
    void setListSamplesAudio(QList<SampleAudio*> *listSamplesAudio);

    QMap<QTreeWidgetItem*, Movement*> getMapTreeItemsMovement();
    QMap<QTreeWidgetItem*, SampleAudio*> getMapTreeItemsSample();
    QMap<QTreeWidgetItem*, ClientOSC*> getMapTreeItemsPort();

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    
signals:
    void remove(Movement *movement);
    void remove(ClientOSC *clientOSC);
    void remove(SampleAudio *sampleAudio);
    
public slots:
    void refresh();
    void slotRemove();
    void slotEdit();

private:
    QTreeWidgetItem *movements;
    QTreeWidgetItem *samples;
    QTreeWidgetItem *ports;
    QTreeWidgetItem *videos;

    QList<Movement*> *listMovements;
    QList<SampleAudio*> *listSamplesAudio;
    QList<ClientOSC*> *listPorts;

    QMap<Movement, QTreeWidgetItem*> itemsLeftMap; //liste des QtreeWidgetItems index� par l'id des mvt associ�s
    QMap<QTreeWidgetItem*, Movement*> mapTreeItemsMovement; //map qui fait correspondre le pointeur sur le move avec le pointeur sur l'item de LeftTree
    QMap<QTreeWidgetItem*, SampleAudio*> mapTreeItemsSample; //map qui fait correspondre le pointeur sur le sample avec le pointeur sur l'item de LeftTree
    QMap<QTreeWidgetItem*, ClientOSC*> mapTreeItemsPort; //map qui fait correspondre le pointeur sur le port avec le pointeur sur l'item de LeftTree

    QAction *actionRemove;
    QAction *actionEdit;

    void createActions();
    
};

#endif // MYLEFTTREE_H
