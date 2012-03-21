#ifndef CONFIGANALYSE_H
#define CONFIGANALYSE_H

#include <QDialog>
#include <QDebug>

namespace Ui {
    class ConfigAnalyse;
}

class ConfigAnalyse : public QDialog
{
    Q_OBJECT
signals:
    void signalConfigAnalyse(float, quint16);

public:
    explicit ConfigAnalyse(QWidget *parent = 0);
    ~ConfigAnalyse();
    void setParamDefaut(float vitesse, quint16 amplitude);

private slots:
    void on_buttonBox_accepted();
    void on_horizontalSliderVitesse_valueChanged(int value);

    void on_horizontalSliderAmpl_valueChanged(int value);

private:
    Ui::ConfigAnalyse *ui;
};

#endif // CONFIGANALYSE_H
