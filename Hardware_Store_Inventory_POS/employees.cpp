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
#include "employees.h"
#include "mysqldatabase.h"
#include <QDebug>
#include "interface.h"

#include <QMessageBox>

Employees::Employees(long int id, QString fname, QString mname, QString lname, QString date, QString ss, QString phone, QString email, QString gender)
          :persons(id,fname,mname,lname,date,ss,phone,email,gender)
{
    iD = store(id);

    firstname = fname;

    middleName = mname;

    lastname = lname;

    dateofbirth = date;

    socialsecrity = ss;

    PhonE = phone;

    emailaddress = email;

    Gender = gender;

}

QVariant Employees::getid()
{

    return iD;
}

QString Employees::getFname()
{
    return firstname;
}

QString Employees::getMname()
{
    return middleName;
}

QString Employees::getLname()
{
    return lastname;
}

QString Employees::getDOB()
{
    return dateofbirth;
}

QString Employees::getSSNO()
{
    return socialsecrity;
}

QString Employees::getphoneNo()
{
    return PhonE;
}

QString Employees::getEmail()
{
    return emailaddress;
}

QString Employees::getGender()
{
    return Gender;
}
