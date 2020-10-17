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
#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include <QWidget>
#include "persons.h"

#include "mysqldatabase.h"

class Customers : public persons
{
public:
    Customers(long int id, QString fname, QString mname, QString lname, QString date, QString ss, QString phone, QString email, QString gender);

    QString getFname();

    QVariant getid();

    QString getMname();

    QString getLname();

    QString getDOB();

    QString getSSNO();

    QString getphoneNo();

    QString getEmail();

    QString getGender();

    void setid(long int id);

    void setfname(QString fname);

    long int iD;

    QString firstname;

    QString middleName;

    QString lastname;

    QString dateofbirth;

    QString socialsecrity;

    QString PhonE;

    QString emailaddress;

    QString Gender;



};

#endif // CUSTOMERS_H
