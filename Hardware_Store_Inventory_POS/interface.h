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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QProgressBar>
#include <mysqldatabase.h>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QLabel>

#include "persons.h"
#include "customers.h"
#include "employees.h"
#include "ui_mainwindow.h"
#include "QCloseEvent"
#include "address.h"
#include <QTranslator>


namespace Ui {
class interface;   //interface class definition
}

class interface : public QMainWindow// public inheritance from Qmainwindow
{
    Q_OBJECT

public:
    explicit interface(QWidget *parent = 0); //constructor

    void salesController();        //functions

    void createSalesTable();

    void validateEmployeeLogin();

    void setRights(QString Role);

    void tables();

    void itemControl();

    long int id;
    QString fname;
    QString mname;
    QString lname;  //public datamambers for customer
    QString dob;
    QString ssno;
    QString phone;
    QString email ;
    QString gender;




    ~interface();

public slots:
    void updatetime(); //public slot to control Cl0ck

private slots:

       void connections();

       void preparations();

       void insertInUsers();

       void addItems();

       void insertInSuppliers();  //Privte Slots for functionality

       QVariant store(long int);

       void close();

       void customers();

       void employees();

       void addEmployeeButton_clicked();

       void addCustomerButton_clicked();

       void AddSupplierButton_clicked();

       void AddItemsButton_clicked();

       void addUserButton_clicked();

        void validatePassword();

        void salesPoint();

        void on_deleteEmpployeeButton_clicked();

        void on_viewtablesButton_clicked();

        void on_addcustomerButton2_clicked();

        void on_addSupplierButton_clicked();   //controlling buttons

        void on_addUserButton_clicked();

        void on_addemployeebutton_clicked();

        void on_deletecostumerButton_clicked();

        void on_ADDSupplierButton_clicked();

        void EmpclearButtonclicked();

        void on_supplierClear_2_clicked();

        void on_itemClear_clicked();

        void on_userClear_clicked();

        void clear();

        void on_BarcodeSold_returnPressed();

        void on_salesCashPaid_returnPressed();

        void salesCheckoutButton_clicked(QString,QString);

        void on_updateEmployeeButton_clicked();

        void on_Update_clicked();

        void on_updateCustomerButon_clicked();

        void on_customerUpdateButton_clicked();

        void on_deleteuserButton_clicked();



        void on_addItemtoTable_clicked();

        void on_deleteitemButton_clicked();

        void on_deleteSaleRecordButton_clicked();   //functions to control buttons

        void on_deletesupplierButton_clicked();

        void on_updateUserButton_clicked();

        void on_userUpdateButton_clicked();

        void on_updateItemButton_clicked();

        void on_updateItemButton_2_clicked();

        void on_salesClearButton_2_clicked();

        void on_salesCheckoutButton_2_clicked();

        void on_logoutButton_clicked();

        void on_SupplierSubmit_2_clicked();

        void on_updateSupplierButton_clicked();

        void on_supplierUpdateButton_clicked();

        void on_action_Close_triggered();

        void on_action_Maximize_triggered();

        void on_action_Minimize_triggered();

        void on_actionAdd_User_triggered();

        void on_action_Add_Customer_triggered();

        void on_action_Add_Supplier_triggered();

        void on_action_Add_Item_triggered();

        void on_action_About_Program_triggered();

        void on_action_About_Corona_Hardware_triggered();

        void on_action_Sales_Point_triggered();

        void on_action_View_Tables_triggered();

        void on_action_Add_Emplloyee_triggered();


        void on_action_Hide_Toolbar_triggered();

        void on_action_Log_Out_triggered();

        void on_action_Show_Toolbar_triggered();

        void on_action_English_triggered();

        void on_action_Spanish_triggered();

protected:
         Ui::MainWindow *ui;

        mysqldatabase databaseManager;     //protected datamembers

        persons * PERSONS;


private:

        void CalculateTotal(int items,double price);   //private functions

        void calculateChange(double paidAmount);

        QSqlTableModel *employeeTable;

         QSqlTableModel *itemsTable;

         QSqlTableModel *customersTable;  //objects for implementation

         QSqlTableModel * usersTable;

         QSqlTableModel * totalSales;

         QSqlTableModel * suppliersTable;

         QWidget * login;

         QLabel * label1;

         QLabel *label2;

         QLabel * useridLabel;

         QLineEdit *userID;

         QLineEdit * usename;

         QLineEdit *password;

         QString username;

         QString role;

         QProgressBar * bar;

         double TOTAL;

        QVariant ID ;
         QString FNAME;
         QString MNAME;
         QString LNAME;
         QString DOB;
         QString SSNO;   //private datamembers for employee
         QString PHONE;
         QString EMAIL ;
         QString GENDER;
         QString MEMBER;

         QString dateString;

         QString text;

         QString supplierId;

         int user;

         int row = 0;

         int column = 2;

         void checkBounds(int i, int size);  //esception function

         static void newFailed();
         void closeEvent(QCloseEvent *e);

         QTranslator appTranslator;

         QTranslator qtTranslator;

         int amountoftimes = 0;



};

#endif // INTERFACE_H
