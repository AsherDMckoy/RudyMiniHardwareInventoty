#ifndef ADDRESS_H
#define ADDRESS_H

#include <QWidget>

class Address
{
public:
    Address(QString Address, QString officePhone, QString mobilePhone, QString Email, QString City, QString Country);

    QString getAddress();

    QString getofficePhone();

    QString getmobilePhone();

    QString getEmail();

    QString getCity();

    QString getCountry();


private:
    QString mainAddress;
    QString officeNumber;
    QString mobileNumber;
    QString businessEmail;
    QString businessCity;
    QString countryofOperation;
};

#endif // ADDRESS_H
