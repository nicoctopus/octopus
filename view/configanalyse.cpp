#include "configanalyse.h"
#include "ui_configanalyse.h"

ConfigAnalyse::ConfigAnalyse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigAnalyse)
{
    ui->setupUi(this);
}


ConfigAnalyse::~ConfigAnalyse()
{
    delete ui;
}

void ConfigAnalyse::on_buttonBox_accepted()
{
    int temp2 = 40 + ui->horizontalSliderAmpl->value() * 5;
    float temp = (float)ui->horizontalSliderVitesse->value();
    emit signalConfigAnalyse(temp /10, temp2);
}

void ConfigAnalyse::setParamDefaut(float vitesse, quint16 amplitude)
{
    int temp = (amplitude - 45)  / 5;
    temp++;
    ui->horizontalSliderAmpl->setValue(temp);
    ui->horizontalSliderVitesse->setValue(vitesse*10);
    ui->horizontalSliderAmpl->setSliderPosition(temp);
    ui->horizontalSliderVitesse->setSliderPosition(vitesse*10);
    ui->label_current_amplitude->setText(QString::number(amplitude) + " %");
    ui->label_current_vitesse->setText(QString::number(vitesse * 100) + " %");
}

void ConfigAnalyse::on_horizontalSliderVitesse_valueChanged(int value)
{
    ui->label_current_vitesse->setText(QString::number(value * 10) + " %");
}

void ConfigAnalyse::on_horizontalSliderAmpl_valueChanged(int value)
{
    ui->label_current_amplitude->setText(QString::number(40 + value * 5) + " %");
}
