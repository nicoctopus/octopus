#ifndef CONFIGRECORDMOUVEMENT_H
#define CONFIGRECORDMOUVEMENT_H

#include <QDialog>
#include <QDebug>

namespace Ui {
    class ConfigRecordMouvement;
}

class ConfigRecordMouvement : public QDialog
{
    Q_OBJECT
signals:
    void signalConfigTempsRecord(float tempsLatence, float tempsRecord);
public:
    explicit ConfigRecordMouvement(QWidget *parent = 0);
    ~ConfigRecordMouvement();
    void setDefautSettings(quint16 tempsLatence, quint16  tempsRecordMovement);

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_buttonBox_accepted();

    void on_horizontalSliderTempsLatence_valueChanged(int value);

    void on_horizontalSliderTempRecord_valueChanged(int value);

private:
    Ui::ConfigRecordMouvement *ui;
};

#endif // CONFIGRECORDMOUVEMENT_H
