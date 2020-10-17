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
#include "address.h"

Address::Address(QString Address, QString officePhone, QString mobilePhone, QString Email, QString City, QString Country)
{
    mainAddress = Address;

    officeNumber = officePhone;

    mobileNumber = mobilePhone;

    businessEmail = Email;

    businessCity = City;

    countryofOperation = Country;
}

QString Address::getAddress()
{
    return mainAddress;
}

QString Address::getofficePhone()
{
    return officeNumber;
}

QString Address::getmobilePhone()
{
    return mobileNumber;
}

QString Address::getEmail()
{
    return businessEmail;
}

QString Address::getCity()
{
    return businessCity;
}

QString Address::getCountry()
{
    return countryofOperation;
}

