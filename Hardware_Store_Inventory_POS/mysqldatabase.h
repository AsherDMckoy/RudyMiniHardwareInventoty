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
#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>


class mysqldatabase
{
public:
    mysqldatabase();
    void loadDatabase(QString password, QString username, QString host);

    void createTables();

    void showtables();

    void createDatabase();

    int insertIntoEmployees(QVariant id,QString fname,QString sname,QString lname,QString dob,QString ssno,QString phoneNo,QString email,QString gender);

    int insertIntoUsers(QString username, QString password, int role_ID, QVariant emp_ID);


    int insertintoItems(QVariant barcode,QString itemname, QString itembrand,QString itemNO,double price, double bulkprice, int itemsonstock, int reorderLevel);

    void removeCountry();

    int insertIntoCustomers(QVariant id,QString fname, QString secName, QString lName,QString DOB, QString SSNO, QString phone, QString email, QString gender, QString member);

    QStringList populateComboBox();

    void populateCountry(const QString &filename ,const QString &file1);

    int countryId(QString country);

    int insertIntoSuppliers(QString name, QString address, QString office, QString mobile, QString email, QString city, int country);

    int updateSuppliers(QVariant id,QString name, QString address, QString office, QString mobile, QString email, QString city, int country);

    void insertIntoRoles();

    QStringList populateRoles();

    int getRoleID(QString role);

    int deleteEmployee(QVariant id);

    int deleteCustomer(QVariant id);

    int deleteUser(QVariant id);

    int deleteSale(QVariant id);

    int deleteItem(QVariant barcode);

    int deletesupplier(QVariant id);

    QString searchEmployee(QVariant empID);

    QString findRole(int id);

    QString userName(QVariant empId);

    QString validateLogin(QString username, QString password);

    QStringList searchBarcode(QVariant barcode);

    QStringList findEmployeeToEdit(QVariant id);

     QStringList findCustomerToEdit(QVariant id);

     QStringList findusertoEdit(QVariant id);

     QStringList findItemtoEdit(QVariant barcode);

    int updateEmployee(QVariant id,QString fname,QString sname,QString lname,QString dob,QString ssno,QString phoneNo,QString email,QString gender);

    int updateCustomer(QVariant id,QString fname,QString sname,QString lname,QString dob,QString ssno,QString phoneNo,QString email,QString gender);

    int updateUser(int id,QString username, QString password, int role_ID, QVariant emp_ID);

    int updateItem(QVariant barcode,QString itemname, QString itembrand,QString itemNO,double price, double bulkprice, int itemsonstock, int reorderLevel);

    QString receipts();

    int InsertIntoReceipt(QString time, QString date, QString Teller, QString receiptno, QString nettotal, QString taxable, QString GrossTotal, QString cashPaid, QString YourChange);

    void removeRoles();

    QString getRole();

    QString getRoleName(int id);

    QStringList findSupplierToEdit(int id);

    QString findCountry(int id);

    void subtractItems(QString name, int amount);


};

#endif // MYSQLDATABASE_H
