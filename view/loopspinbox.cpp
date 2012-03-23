#include "loopspinbox.h"


QMap< int, QString > LoopSpinBox::textes;
QMap< QString, int > LoopSpinBox::values;

LoopSpinBox::LoopSpinBox(QWidget* p): QSpinBox(p)
{


        if( ! textes.count() )
            {
            for(int j=1; j<11; j++){
                textes[j] = QString::number(j);
            }
            textes[11] = "INF";
                for( int i = 0; i < textes.count(); i++ ) values[textes[i]] = i;
            }
            //resize( 100, 40 );
        setSuffix(" boucle(s)");
            setRange( 1, 11 );
            setWrapping(true);


}


QString LoopSpinBox::textFromValue ( int value ) const
{
    return( textes[value] );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int LoopSpinBox::valueFromText( const QString & texte )
{
    return( values[texte] );
}
