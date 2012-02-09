#include "databitg.h"

DataBITG::DataBITG() {
}

DataBITG::DataBITG(const quint32 &bary, const quint32 &idmove, const quint32 &t, const quint32 &g) {
    this->barycenter = bary;
    this->idMovement = idmove;
    this->time = t;
    this->gap = g;
}

void DataBITG::setBarycenter(const quint32 &b) {
    this->barycenter = b;
}

quint32 DataBITG::getBarycenter() {
    return barycenter;
}

void DataBITG::setIdMovement(const quint32 &id) {
    this->idMovement = id;
}

quint32 DataBITG::getIdMovement() {
    return idMovement;
}

void DataBITG::setTime(const quint32 &t) {
    this->time = t;
}

quint32 DataBITG::getTime() {
    return time;
}

void DataBITG::setGap(const quint32 &g) {
    this->gap = g;
}

quint32 DataBITG::getGap() {
    return gap;
}
