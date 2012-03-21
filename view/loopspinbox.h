#ifndef LOOPSPINBOX_H
#define LOOPSPINBOX_H

#include "QtGui/QSpinBox"
#include "QMap"

class LoopSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    LoopSpinBox(QWidget* p);

private:
   static QMap< int,     QString > textes;
   static QMap< QString, int     > values;

protected:
    QString textFromValue( int value ) const;
    int valueFromText( const QString & texte );

};

#endif // LOOPSPINBOX_H
