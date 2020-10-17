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
#include "customers.h"
#include "persons.h"
#include <QMessageBox>
#include <QObject>
#include <QDebug>

#include "mysqldatabase.h"


Customers::Customers( long int id, QString fname, QString mname, QString lname, QString date, QString ss, QString phone, QString email, QString gender)
    :persons(id,fname,mname,lname,date,ss,phone,email,gender)
{

   iD = id;

   firstname = fname;

   middleName = mname;

   lastname = lname;

   dateofbirth = date;

   socialsecrity = ss;

   PhonE = phone;

   Gender = gender;

   emailaddress = email;

}

QVariant Customers::getid()
{
    return store(iD);
}

QString Customers::getFname()
{
    if(Gender == "Male")
    {
      return "Mr. "+firstname;
    }
    else
    {
        return "Mrs."+ firstname;
    }
}

QString Customers::getLname()
{

    return lastname;
}

QString Customers::getMname()
{
    return middleName;
}

QString Customers::getDOB()
{
    return dateofbirth;
}

QString Customers::getSSNO()
{
    return socialsecrity;
}

QString Customers::getphoneNo()
{
    return PhonE;
}

QString Customers::getGender()
{
    return Gender;
}

QString Customers::getEmail()
{
    return emailaddress;
}






