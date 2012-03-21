#include "configrecordmouvement.h"
#include "ui_configrecordmouvement.h"

ConfigRecordMouvement::ConfigRecordMouvement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigRecordMouvement)
{
    ui->setupUi(this);
}

ConfigRecordMouvement::~ConfigRecordMouvement()
{
    delete ui;
}

void ConfigRecordMouvement::setDefautSettings(quint16 tempsLatence, quint16  tempsRecordMovement)
{
    qDebug() << tempsLatence << tempsRecordMovement << endl;
    ui->horizontalSliderTempRecord->setValue(tempsRecordMovement);
    ui->horizontalSliderTempRecord->setSliderPosition(tempsRecordMovement);
    ui->horizontalSliderTempsLatence->setValue(tempsLatence);
    ui->horizontalSliderTempsLatence->setSliderPosition(tempsLatence);
    ui->label_temps_latence->setText(QString::number(tempsLatence) + " sec");
    ui->label_temp_record->setText(QString::number(tempsRecordMovement) + "msec");
}

void ConfigRecordMouvement::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
	ui->horizontalSliderTempRecord->setEnabled(false);
	ui->horizontalSliderTempsLatence->setEnabled(false);
	ui->label->setEnabled(false);
	ui->label_2->setEnabled(false);
	ui->label_temps_latence->setEnabled(false);
	ui->label_temp_record->setEnabled(false);
    }
    else
    {
	ui->horizontalSliderTempRecord->setEnabled(true);
	ui->horizontalSliderTempsLatence->setEnabled(true);
	ui->label->setEnabled(true);
	ui->label_2->setEnabled(true);
	ui->label_temps_latence->setEnabled(true);
	ui->label_temp_record->setEnabled(true);
    }
}

void ConfigRecordMouvement::on_buttonBox_accepted()
{
    if(ui->checkBox->isChecked() == false)
    {
	emit signalConfigTempsRecord(0, 4000);
    }
    else
    {
	emit signalConfigTempsRecord(ui->horizontalSliderTempsLatence->value(), ui->horizontalSliderTempRecord->value());
    }
}

void ConfigRecordMouvement::on_horizontalSliderTempsLatence_valueChanged(int value)
{
    ui->label_temps_latence->setText(QString::number(value) + " sec");
}

void ConfigRecordMouvement::on_horizontalSliderTempRecord_valueChanged(int value)
{
    int temp = value / 1000;
    float temp5 = (float)value / 1000 - temp;
    int temp2 = 100*temp5;
    float temp3 = (float)temp2 / 100;
    float res = temp + temp3;
    ui->label_temp_record->setText(QString::number(res) + "sec");
}
