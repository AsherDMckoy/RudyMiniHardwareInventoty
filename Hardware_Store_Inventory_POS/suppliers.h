/*Jose Urbina
 * Randy Tzib
 * Dane Cabb
 * Nadir Chi
 * Asher Mckoy
 * David Hernandez
 *
 * GUI && Object Oriented Final Project
 * may 8 2017
 * */
#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include <QWidget>

#include "address.h"

class suppliers
{
public:
    suppliers(QString name,Address address);

    QString getsupplierName();

    QStringList getAddress();

private:

     QString supplierName;

     QStringList addressFetched;

     Address supplierAddress;


};

#endif // SUPPLIERS_H
