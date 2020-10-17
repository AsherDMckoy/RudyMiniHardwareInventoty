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
#ifndef PERSONS_H
#define PERSONS_H

#include <QWidget>
#include <QVariant>


class persons
{
public:
    persons(long int id, QString fname, QString mname, QString lname, QString date, QString ss, QString phone, QString email, QString gender);


    virtual QVariant getid();

    virtual QString getFname();

    virtual QString getMname();

    virtual QString getLname();

    virtual QString getDOB();

    virtual QString getSSNO();

    virtual QString getphoneNo();

    virtual QString getEmail();

    virtual QString getGender();

    QVariant store (long int input);



    QString getmember();

    void setmember();

//    virtual int getAge();

//    virtual double getSalary();

//    virtual void setAge(int age);

//    virtual void setSalary(double salary);

//    virtual void setAddress(QString address);

//    virtual QString getAddress();

//    double calculateNetSalary(double TotalSalary);




protected:

   long int ID;

    QString firstName;

    QString middleName;

    QString lastName;

    QString dateofBirth;

    QString social;

    QString PHONE;

    QString Email;

    QString Gender;

    double Salary;

    int Age;

    QString Address;

       QString member;



};

#endif // PERSONS_H
