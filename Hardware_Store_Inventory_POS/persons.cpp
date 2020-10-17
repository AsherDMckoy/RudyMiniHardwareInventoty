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
#include "persons.h"
#include <QDebug>

persons::persons(long int id, QString fname, QString mname, QString lname, QString date, QString ss, QString phone, QString email, QString gender)
{
        ID = id;

        firstName = fname;

        middleName = mname;

        lastName = lname;    //assigning private variables to ones received from construcror

        dateofBirth = date;

        social = ss;

        PHONE = phone;

        Email = email;

        Gender = gender;

}

QVariant persons::getid()
{


    return store(ID);    //get id returns id
}

QString persons::getFname()
{
   firstName;      //get fname return firstname

}

QString persons::getMname()
{
    return middleName;    //get mname return middle name
}

QString persons::getLname()
{
    return lastName;   //get lastname return last name
}

QString persons::getDOB()
{
    return dateofBirth;        //teturning date of birth
}

QString persons::getSSNO()
{
    return social;       //returning social security
}
QString persons::getphoneNo()
{
    return PHONE;        //returning phone number
}
QString persons::getEmail()
{
    return Email;
}

QString persons::getGender()
{
    return Gender;
}

QVariant persons::store (long int input)
{
    qDebug()<<input;
    unsigned long long data = (long int) input;        //store changes variable to QVariant

    QVariant qvariant( data );

    return qvariant;

 }



void persons::setmember()
{
    member = "NO";             //setting customer as not a member
}

QString persons::getmember()
{
    return member;
}


//void persons::setSalary(double salary)
//{
//    Salary = salary;
//}

//double persons::getSalary()
//{
//    return Salary;
//}

//double persons::calculateNetSalary(double TotalSalary)
//{
//    if(TotalSalary > 2166.66)
//    {
//        TotalSalary *= 0.75;
//    }


//    return TotalSalary;
//}
