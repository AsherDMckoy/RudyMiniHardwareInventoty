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
#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <QWidget>
#include "mysqldatabase.h"
#include "persons.h"

class Employees :public persons
{
public:
    Employees(long int id, QString fname, QString mname, QString lname, QString date, QString ss, QString phone, QString email, QString gender);


    QString getFname();

    QVariant getid();

    QString getMname();

    QString getLname();

    QString getDOB();

    QString getSSNO();

    QString getphoneNo();

    QString getEmail();

    QString getGender();


    QVariant iD;

    QString firstname;

    QString middleName;

    QString lastname;

    QString dateofbirth;

    QString socialsecrity;

    QString PhonE;

    QString emailaddress;

    QString Gender;
};

#endif // EMPLOYEES_H
