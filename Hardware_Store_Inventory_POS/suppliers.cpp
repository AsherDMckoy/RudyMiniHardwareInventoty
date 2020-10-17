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
#include "suppliers.h"

#include "address.h"

suppliers::suppliers(QString name,Address address)  //constructor

        : supplierName(name), supplierAddress(address)  //initializing name and address object
{

}

QString suppliers::getsupplierName()
{
    return supplierName;  //returning string name
}

QStringList suppliers::getAddress()
{
    addressFetched << supplierAddress.getAddress();

    addressFetched << supplierAddress.getofficePhone();    //getting information from address and placing it into stringlist

    addressFetched << supplierAddress.getmobilePhone();

    addressFetched << supplierAddress.getEmail();

    addressFetched << supplierAddress.getCity();

    addressFetched << supplierAddress.getCountry();

    return addressFetched;

}

