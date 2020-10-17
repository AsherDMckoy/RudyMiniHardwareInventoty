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
#include "interface.h"
#include "mysqldatabase.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QProgressBar>
#include <ctime>
#include <new>
#include <cstdlib>
#include <QSqlTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QModelIndex>
#include <QTimer>
#include <QSettings>
#include <QTime>
#include <QPalette>
#include <QDateTime>
#include <QInputDialog>
#include "customers.h"
#include "persons.h"
#include "employees.h"
#include <QProgressBar>
#include <QFile>
#include <fstream>
#include <iostream>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <stdexcept>
#include <QTextStream>
#include "address.h"
#include "suppliers.h"

using namespace std;

interface::interface(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect position = frameGeometry();

    position.moveCenter(QDesktopWidget().availableGeometry().center());

    move(position.topLeft());

    databaseManager.loadDatabase("davidhernandez00", "root", "localhost");//MAY CHANGE DEPEDING ON YOUR MACHNE

    QFile File(":/stylesheet.qss");

    File.open(QFile::ReadOnly);

    QString StyleSheet = QLatin1String(File.readAll());         //fulling counry combobox

    qApp->setStyleSheet(StyleSheet);

    validateEmployeeLogin();

    ui->itemsSoldWidget->verticalHeader()->setVisible(false);

    tables();

    qApp->installTranslator(&appTranslator);              //translator added

    qApp->installTranslator(&qtTranslator);



    QTimer *timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updatetime()));

    timer->start(1000);

    QSettings settings;

    restoreGeometry(settings.value("MainGeometry").toByteArray());

    restoreState(settings.value("MainState").toByteArray());

    QPalette color;

    color.setColor(QPalette::Foreground, Qt::black);

    ui->CurrentTime->setPalette(color);

    updatetime();


    QDate date = QDate::currentDate();
    dateString = date.toString();              //used to manipulate clock in salespoint
    ui->todaysDate->setText(dateString);


     ui->itemsSoldWidget->setColumnCount(3);

    ui->itemsSoldWidget->alternatingRowColors();

     for (int c = 0; c < ui->itemsSoldWidget->horizontalHeader()->count(); ++c)
     {
         ui->itemsSoldWidget->horizontalHeader()->setSectionResizeMode(
             c, QHeaderView::Stretch);
     }



       connections();      //call to function connections


       preparations();  //making necessary preparations for tables


}

interface::~interface()
{
    delete ui;
}

void interface::preparations()
{


    databaseManager.populateCountry(":/countries/countries.txt",":/countries/countryCodes.txt");

    QStringList words = databaseManager.populateComboBox();


    foreach (QString i, words)
    {
        ui->supplierCountry_2->addItem(i);

    }

    QStringList roles = databaseManager.populateRoles();  //popularing roles combobox

    foreach (QString x, roles)
    {
        ui->roleID->addItem(x);
    }


    ui->UserPassword->setEchoMode(QLineEdit::Password);  //this will show data as hidden

    ui->userConfirm->setEchoMode(QLineEdit::Password);

    ui->PasswordInput->setEchoMode(QLineEdit::Password);

    ui->ItemPrice->setRange(0,10000);

    ui->ItemsOnStock->setRange(0,10000);       //ranges for spinbox

    ui->bulkPrice->setRange(0,10000);

    ui->reorderLevel->setRange(0,10);
    QPixmap pict(":/images/staff_login.png");
    ui->loginImage->setPixmap(pict);

}

void interface::connections()              //signals for operations
{

        connect(ui->userSubmit,SIGNAL(clicked(bool)),this,SLOT(insertInUsers()));
        connect(ui->closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));
        connect(ui->userClose,SIGNAL(clicked(bool)),this,SLOT(close()));          //signals
        connect(ui->itemClose,SIGNAL(clicked(bool)),this,SLOT(close()));

        connect(ui->itemSubmit,SIGNAL(clicked(bool)),this,SLOT(addItems()));

        connect(ui->CustomerClose,SIGNAL(clicked(bool)),this,SLOT(close()));
        connect(ui->CustomerSubmit,SIGNAL(clicked(bool)),this,SLOT(customers()));
        connect(ui->empsubmitButton,SIGNAL(clicked(bool)),this,SLOT(employees()));

        connect(ui->salesCancelButton,SIGNAL(clicked(bool)),this,SLOT(close()));


        connect((ui->AddEmployeeButton),SIGNAL(clicked(bool)),this,SLOT(addEmployeeButton_clicked()));     //signals
        connect((ui->AddCustomerButton),SIGNAL(clicked(bool)),this,SLOT(addCustomerButton_clicked()));
        connect((ui->AddItemButton),SIGNAL(clicked(bool)),this,SLOT(AddItemsButton_clicked()));

        connect((ui->AddUserButton),SIGNAL(clicked(bool)),this,SLOT(addUserButton_clicked()));
        connect((ui->salesPointButton),SIGNAL(clicked(bool)),this,SLOT(salesPoint()));
        connect((ui->CustomerClear),SIGNAL(clicked(bool)),this,SLOT(clear()));
        connect((ui->clearButton),SIGNAL(clicked(bool)),this,SLOT(EmpclearButtonclicked()));        //signals

        connect((ui->UserLoginButton),SIGNAL(clicked(bool)),this,SLOT(validatePassword()));
        connect((ui->userCancelButton),SIGNAL(clicked(bool)),this,SLOT(close()));

        connect((ui->action_About_Qt),SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));              //signals

}


void interface::close()        //function close
{
    databaseManager.removeCountry();

    databaseManager.removeRoles();

    exit(0);
}

void interface::addItems()            //adding items to database
{
    QString itemName = ui->itemName->text();

    QString itemBrand = ui->ItemBrand->text();

    double itemPrice = ui->ItemPrice->text().toDouble();

    int itemsOnStock = ui->ItemsOnStock->value();

    int reorderLevel = ui->reorderLevel->value();

    QString itemNO = ui->itemNo->text();

    double stockprice = ui->bulkPrice->text().toDouble();

    long int barc = ui->itemBar->text().toLong();

    QVariant barcode = store(barc);


    if(databaseManager.insertintoItems(barcode,itemName, itemBrand,itemNO,itemPrice,stockprice,itemsOnStock,reorderLevel))
    {
         QMessageBox::information(this,tr("Items Form"), tr("Insertion of Item Successful"));   //error message if passed

         itemsTable->select();

         on_itemClear_clicked();

    }
    else
    {
        QMessageBox::warning(this,tr("Items Form"), tr("Insertion Failed"));          //error message if failed
    }
}

void interface::insertInSuppliers() //inserting into suppliers
{
   int i = 0;
    QString name = ui->supplierName_2->text();

    QString address = ui->supplerAddress_2->text();

    QString officePhone = ui->supplierOfficePhone_2->text();   //getting data from form

    QString mobilePhone = ui->supplierMobilePhone_2->text();

    QString email = ui->supplierEmail_2->text();

    QString city = ui->supplierCity_2->text();

    QString country = ui->supplierCountry_2->currentText();

     Address addressive(address,officePhone,mobilePhone,email,city,country);     //creating address object

    suppliers supplierInformation(name,addressive);

    QString supplierName = supplierInformation.getsupplierName();

    QStringList fullAddress = supplierInformation.getAddress();

    name = supplierName;

     address = fullAddress[i];
     i++;                                        //setting address to corresponing variables
     checkBounds(i,supplierName.size());

     officePhone = fullAddress[i];
     i++;
     checkBounds(i,supplierName.size());

     mobilePhone = fullAddress[i];
     i++;
     checkBounds(i,supplierName.size());

     email = fullAddress[i];
     i++;
     checkBounds(i,supplierName.size());

     city = fullAddress[i];
     i++;
     checkBounds(i,supplierName.size());

     country = fullAddress[i];


    int country_ID = databaseManager.countryId(country);   //searching for id from country

    if(databaseManager.insertIntoSuppliers(name,address,officePhone,mobilePhone,email,city,country_ID)) //inserting into suppliers table
    {
        QMessageBox::information(this,tr("Supplier Form"), tr("Insertion Successful"));      //successful insertion
        suppliersTable->select();
        on_supplierClear_2_clicked();
    }
    else
    {
        QMessageBox::warning(this,tr("Customer Form"), tr("Insertion Failed")); //insertion failed
        return;
    }

}

  QVariant interface::store (long int input)         //function convers long int to QVariant
  {
      unsigned long long data = (long int) input;

      QVariant qvariant( data );

      return qvariant;          //return the QVariant
   }

  void interface::insertInUsers()             //getting information from user form
  {
      QString username = ui->userName->text();

      QString password = ui->UserPassword->text();

      QString role_ID = ui->roleID->currentText();

      QString confirm = ui->userConfirm->text();

      int id = databaseManager.getRoleID(role_ID);

      long int empid = ui->userEmpID->text().toLong();

      QVariant ids = store(empid);

      if((username == "") | (password == "") | (confirm == "")) //validating for empty fields
      {
          QMessageBox::warning(this,tr("User Entry Form"),tr("One or more fields may be Empty, verify and try again"));

          return;
      }
      if(password != confirm)
      {
          QMessageBox::warning(this,tr("Password"),tr("Password does not match, Please Try again")); //passwords do not match

          return;
      }

     if(databaseManager.insertIntoUsers(username,password,id,ids))
     {
         QMessageBox::information(this,tr("User Form"),tr("User Added Successfully")); //user was added
         usersTable->select();

     }
     else
     {
         QMessageBox::warning(this,tr("User Form"),tr("Insertion to Users Table failed")); //user was not added

     }
  }

  void interface::customers()        //getting info from customer form
  {
       id = ui->custID->text().toLongLong();
       fname = ui->customerFName->text();
       mname = ui->customerSName->text();
       lname = ui->customerLName->text();
       dob = ui->customerDOB->text();
       ssno = ui->customerSocial->text();
       phone = ui->customerPhone1->text();
       email = ui->customerEmail->text();
       gender = ui->customerGender->currentText();

       if((fname == "") | (lname == "") | (dob == "") | (gender == "") | (id == 0))
       {
           QMessageBox::warning(this,tr("Customer Entry"),tr("One or more fields Empty! verify and Try again")); //empty line encountered
           return;
       }

       Customers CUSTOMERS(id,fname,mname,lname,dob,ssno,phone,email,gender);    //customer object

       PERSONS = &CUSTOMERS;      //customer object referenced to parent




       ID = PERSONS->getid();

       FNAME = PERSONS->getFname();

       MNAME = PERSONS->getMname();

       LNAME = PERSONS->getLname();     //calling functions for child

       DOB = PERSONS->getDOB();

       SSNO = PERSONS->getSSNO();

       PHONE = PERSONS->getphoneNo();

       EMAIL = PERSONS->getEmail();

       GENDER = PERSONS->getGender();

       PERSONS->setmember();

       MEMBER = PERSONS->getmember();

if(databaseManager.insertIntoCustomers(ID,FNAME,MNAME,LNAME,DOB,SSNO,PHONE,EMAIL,GENDER,MEMBER))
{
    QMessageBox::information(this,tr("Customer Entry"), tr("Successful insert into Customers table")); //inserted successful into customers

    customersTable->select();

    clear();


}

else
   {

    QMessageBox::warning(this,tr("Customer Entry"),tr("Failed to insert customer"));//insert to customer failed

    return;

   }
}

void interface::employees()
  {
    id = ui->empID->text().toLongLong();

    fname = ui->empFname->text();               //getting information from employee table

    mname = ui->empSName->text();

    lname = ui->empLName->text();

    dob = ui->empDOB->text();

    ssno = ui->empSSNo->text();

    phone = ui->empPhone->text();

    email = ui->empEmail->text();

    gender = ui->empGender->currentText();


    if((fname == "") | (lname == "") | (dob == "") | (gender == "") | (id == 0)) //checking for empty fields
    {
        QMessageBox::warning(this,tr("Employee Entry"),tr("One or more fields Empty! verify and Try again"));

        return;
    }

    Employees Employee(id,fname,mname,lname,dob,ssno,phone,email,gender); //creating employee object

    PERSONS = &Employee;

    ID = PERSONS->getid();

    FNAME = PERSONS->getFname();

    MNAME = PERSONS->getMname();

    LNAME = PERSONS->getLname();

    DOB = PERSONS->getDOB();

    SSNO = PERSONS->getSSNO();         //calling child functions

    PHONE = PERSONS->getphoneNo();

    EMAIL = PERSONS->getEmail();

    GENDER = PERSONS->getGender();


    if(databaseManager.insertIntoEmployees(ID,FNAME,MNAME,LNAME,DOB,SSNO,PHONE,EMAIL,GENDER)) //passing datat to database for insertion
    {
      QMessageBox::information(this,"Employee Form","Insertion to Employee Table Successful"); //successful intertion to database

      employeeTable->select();

      EmpclearButtonclicked(); //clear form

    }

    else
    {
        QMessageBox::warning(this,"Employee Insert", "Failed to insert into Employees. \n Primary key may have already been utilized\n"
                                                            "or some data field may be too large");

       return;
    }
  }

void interface::addEmployeeButton_clicked() //employee button clicked
{
    ui->customers->hide();
    ui->Items->hide();
    ui->SalesPoint->hide();
    ui->User->hide();
    ui->tableViews->hide();
    ui->Suppliers->hide();

    ui->interfaceForms->setCurrentIndex(6); //current index for employee form
}

void interface::addCustomerButton_clicked() //customer button clicked
{
    ui->Employee->hide();
    ui->Items->hide();
    ui->SalesPoint->hide();
    ui->User->hide();
    ui->tableViews->hide();
    ui->Suppliers->hide();

  ui->interfaceForms->setCurrentIndex(4);
}

void interface::AddSupplierButton_clicked()  //add supplier button clicked
{
    ui->customers->hide();
    ui->Items->hide();
    ui->SalesPoint->hide();
    ui->User->hide();
    ui->tableViews->hide();
    ui->Employee->hide();
    ui->interfaceForms->setCurrentIndex(2);
}

void interface::AddItemsButton_clicked() //items button clicked
{
    ui->customers->hide();
    ui->Employee->hide();
    ui->SalesPoint->hide();
    ui->User->hide();
    ui->tableViews->hide();
    ui->Suppliers->hide();

    ui->interfaceForms->setCurrentIndex(3);
    ui->itemBar->setEnabled(true);

}

void interface::addUserButton_clicked() //user button clicked
{
    ui->customers->hide();
    ui->Items->hide();
    ui->SalesPoint->hide();
    ui->Employee->hide();
    ui->tableViews->hide();
    ui->Suppliers->hide();
   ui->interfaceForms->setCurrentIndex(0);
}

void interface::clear()   //customer clear button clears customer form
{

   ui->custID->clear();
   ui->customerFName->clear();
   ui->customerSName->clear();
   ui->customerLName->clear();
   ui->customerDOB->clear();
   ui->customerEmail->clear();
   ui->customerSocial->clear();
   ui->customerPhone1->clear();

}

void interface::updatetime()   //time updating for clock in Sales point
{
       QTime time = QTime::currentTime();

       text = time.toString("hh:mm a");

       ui->CurrentTime->display(text);

       QPalette palette = ui->CurrentTime->palette();

       palette.setColor(ui->CurrentTime->backgroundRole(), Qt::blue);

       ui->CurrentTime->setPalette(palette);

       if ((time.second() % 2) == 0)
       {
           text[2] = ' ';
           ui->CurrentTime->display(text);
       }
       if(!time.second()%2 == 0)
       {
           ui->CurrentTime->display(text);

       }

}

void interface::validateEmployeeLogin()   //validate if employee is a user
{
    bool ok;
     QString id = QInputDialog::getText(this, tr("System Login"), tr("EmployeeID:"),QLineEdit::Normal, QString(), &ok); //Message asking user input

     QVariant ID = store(id.toLong());
     if(ok)
     {
         role = databaseManager.searchEmployee(ID); //search the database to see if it is a user

         if(role.size() > 0)
         {
             QMessageBox::information(this,"User Validation Success", "System Recognized you as "+role+".\n Press Ok and Enter Validation");

             username = databaseManager.userName(ID);

             ui->interfaceForms->show();

             ui->UserNameDefault->setText(username);

             ui->AddCustomerButton->hide();
             ui->AddEmployeeButton->hide();
             ui->AddItemButton->hide();          //hide all windows
             ui->AddUserButton->hide();
             ui->ADDSupplierButton->hide();
             ui->salesPointButton->hide();
             ui->viewtablesButton->hide();
             ui->logoutButton->hide();

             ui->Employee->hide();
             ui->customers->hide();
             ui->Suppliers->hide();
             ui->Items->hide();
             ui->User->hide();
             ui->SalesPoint->hide();
             ui->tableViews->hide();

             ui->actionAdd_User->setEnabled(false);
             ui->action_Add_Customer->setEnabled(false);
             ui->action_Add_Emplloyee->setEnabled(false);
             ui->action_Sales_Point->setEnabled(false);
             ui->action_Add_Supplier->setEnabled(false);
             ui->action_View_Tables->setEnabled(false);
             ui->action_Log_Out->setEnabled(false);
             ui->action_Add_Item->setEnabled(false);

             ui->interfaceForms->setCurrentIndex(7);



         }

         else
         {
             QMessageBox::warning(this,tr("User Validation Failed"), tr("Access Denied Please Contact System Administrator"));
             validateEmployeeLogin();      //not a user
         }
     }
     else
     {
         QMessageBox::StandardButton reply = QMessageBox::warning(this,tr("System Close"),tr("You are about to close the System.\n Do you wish to Proceed?"),
                            QMessageBox::Yes|QMessageBox::No);


         if(reply == QMessageBox::Yes)
         {
         close();
         }
         if(reply == QMessageBox::No)
         {                                          //asks if user really wants to quit
            validateEmployeeLogin();
         }
     }

}

void interface::validatePassword()
{

    QString role = databaseManager.validateLogin(username,ui->PasswordInput->text()); //validates for passowrd

    setRights(role);

}

void interface::setRights(QString Role)
{
     if(Role == "Administrator")            //setts rights for administrator
     {
         ui->interfaceForms->show();
         ui->AddCustomerButton->show();
         ui->AddEmployeeButton->show();
         ui->viewtablesButton->show();
         ui->AddItemButton->show();
         ui->AddUserButton->show();
         ui->ADDSupplierButton->show();
         ui->salesPointButton->show();
         ui->logoutButton->show();

         ui->actionAdd_User->setEnabled(true);
         ui->action_Add_Customer->setEnabled(true);
         ui->action_Add_Emplloyee->setEnabled(true);
         ui->action_Sales_Point->setEnabled(true);
         ui->action_Add_Supplier->setEnabled(true);
         ui->action_View_Tables->setEnabled(true);
         ui->action_Log_Out->setEnabled(true);
         ui->action_Add_Item->setEnabled(true);

         ui->empTableView->show();
         ui->supplierTableView->show();
         ui->customertableView->show();
         ui->totalsalestableView->show();
         ui->userTableView->show();

         ui->deleteEmpployeeButton->show();
         ui->updateEmployeeButton->show();
         ui->addemployeebutton->show();

         ui->deleteuserButton->show();
         ui->updateUserButton->show();
         ui->addUserButton->show();

         ui->deletesupplierButton->show();
         ui->updateSupplierButton->show();
         ui->addSupplierButton->show();

         ui->deletecostumerButton->show();
         ui->updateCustomerButon->show();
         ui->addcustomerButton2->show();

         ui->EmployeeTab->setEnabled(true);
         ui->UserTab->setEnabled(true);
         ui->SuppliersTab->setEnabled(true);
         ui->totalSalesTab->setEnabled(true);
         ui->CustomerTab->setEnabled(true);

         ui->custID->setEnabled(true);

          ui->updateItemButton_2->setEnabled(true);

         ui->deleteSaleRecordButton->show();



         ui->interfaceForms->setCurrentIndex(1);
     }

    if(Role == "Teller")                         //set rights to teller
    {
        ui->SalesTellerName->setText(username);
        ui->interfaceForms->show();

        ui->actionAdd_User->setEnabled(false);
        ui->action_Add_Customer->setEnabled(false);
        ui->action_Add_Emplloyee->setEnabled(false);
        ui->action_Sales_Point->setEnabled(true);
        ui->action_Add_Supplier->setEnabled(false);
        ui->action_View_Tables->setEnabled(false);
        ui->action_Log_Out->setEnabled(true);
        ui->action_Add_Item->setEnabled(false);

        ui->logoutButton->show();
        ui->AddCustomerButton->hide();
        ui->AddEmployeeButton->hide();
        ui->viewtablesButton->hide();
        ui->AddItemButton->hide();
        ui->AddUserButton->hide();
        ui->ADDSupplierButton->hide();
        ui->salesPointButton->hide();

        ui->interfaceForms->setCurrentIndex(5);
        ui->BarcodeSold->setFocus();
    }


    if(Role == "Ware House Manager")             //sets rights to warehouse
    {
        ui->interfaceForms->show();

        ui->actionAdd_User->setEnabled(false);
        ui->action_Add_Customer->setEnabled(false);
        ui->action_Add_Emplloyee->setEnabled(false);
        ui->action_Sales_Point->setEnabled(false);
        ui->action_Add_Supplier->setEnabled(false);
        ui->action_View_Tables->setEnabled(true);
        ui->action_Log_Out->setEnabled(true);
        ui->action_Add_Item->setEnabled(true);

        ui->AddCustomerButton->hide();
        ui->AddEmployeeButton->hide();
        ui->AddItemButton->hide();
        ui->AddUserButton->hide();
        ui->ADDSupplierButton->hide();
        ui->salesPointButton->hide();


        ui->viewtablesButton->show();
        ui->logoutButton->show();

        ui->EmployeeTab->setEnabled(false);
        ui->UserTab->setEnabled(false);
        ui->SuppliersTab->setEnabled(false);
        ui->totalSalesTab->setEnabled(false);
        ui->CustomerTab->setEnabled(false);

        ui->empTableView->hide();
        ui->supplierTableView->hide();
        ui->customertableView->hide();
        ui->totalsalestableView->hide();
        ui->userTableView->hide();

        ui->deleteEmpployeeButton->hide();
        ui->updateEmployeeButton->hide();
        ui->addemployeebutton->hide();

        ui->deleteuserButton->hide();
        ui->updateUserButton->hide();
        ui->addUserButton->hide();

        ui->deletesupplierButton->hide();
        ui->updateSupplierButton->hide();
        ui->addSupplierButton->hide();

        ui->deletecostumerButton->hide();
        ui->updateCustomerButon->hide();
        ui->addcustomerButton2->hide();

        ui->deleteSaleRecordButton->hide();


        ui->interfaceForms->setCurrentIndex(3);


    }




}

void interface::salesPoint()
{
    ui->customers->hide();
    ui->Items->hide();
    ui->Employee->hide();         //sales point page focus
    ui->User->hide();
    ui->tableViews->hide();
    ui->Suppliers->hide();
    ui->interfaceForms->setCurrentIndex(5);

    salesController();
}

void interface::tables()
{
   // char* p;
    std::set_new_handler(newFailed );


                employeeTable = new QSqlTableModel(this);

                employeeTable->setTable("Employees");

                employeeTable->setSort(1,Qt::AscendingOrder);           //employee table model

                employeeTable->select();

                ui->empTableView->setModel(employeeTable);

                ui->empTableView->setItemDelegate(new QSqlRelationalDelegate(this));

                ui->empTableView->resizeColumnsToContents();



                customersTable = new QSqlTableModel(this);

                customersTable->setTable("Customers");

                customersTable->setSort(1,Qt::AscendingOrder);

                customersTable->select();                        //customer table model

                ui->customertableView->setModel(customersTable);

                ui->customertableView->setItemDelegate(new QSqlRelationalDelegate(this));

                ui->customertableView->resizeColumnsToContents();



                itemsTable = new QSqlTableModel(this);

                itemsTable->setTable("Items");

                itemsTable->setSort(1,Qt::AscendingOrder);

                itemsTable->select();                           //items table model

                ui->itemTableView->setModel(itemsTable);

                ui->itemTableView->setItemDelegate(new QSqlRelationalDelegate(this));

                ui->itemTableView->resizeColumnsToContents();



                usersTable = new QSqlTableModel(this);

                usersTable->setTable("Users");

                usersTable->setSort(1, Qt::AscendingOrder);//user table model

                usersTable->select();

                ui->userTableView->setModel(usersTable);

                ui->userTableView->setItemDelegate(new QSqlRelationalDelegate(this));

                ui->userTableView->resizeColumnsToContents();


                suppliersTable = new QSqlTableModel(this);

                suppliersTable->setTable("Suppliers_Table");

                suppliersTable->setSort(1,Qt::AscendingOrder);        //suppliers table model

                suppliersTable->select();

                ui->supplierTableView->setModel(suppliersTable);

                ui->supplierTableView->setItemDelegate(new QSqlRelationalDelegate(this));

                ui->supplierTableView->resizeColumnsToContents();


                totalSales = new QSqlTableModel(this);

                totalSales->setTable("TOTAL_SALES");

                totalSales->setSort(1, Qt::AscendingOrder);        //total sales table model

                totalSales->select();

                ui->totalsalestableView->setModel(totalSales);

                ui->totalsalestableView->setItemDelegate(new QSqlRelationalDelegate(this));

                ui->totalsalestableView->resizeColumnsToContents();

//                while(1)
//                {
//                  qDebug() << "Attempting to allocate 1 GiB...";
//                   p = new char [1024*1024*1024];
//                  qDebug() << "Ok\n";
//                }

//                delete[] p;







}

 void interface::newFailed()
{
    qDebug()<< "Memory allocation failed, terminating\n";        //function to call if new fails

    QMessageBox::warning(0,"Bad Allocation","Failed to allocate memory for new! \nTry Closing all active Programs and Try Running the Program again");

    return;


}

void interface::on_deleteEmpployeeButton_clicked()
{
        QModelIndex index = ui->empTableView->currentIndex();

        if (!index.isValid())
            return;


        QSqlRecord record = employeeTable->record(index.row());          //deleting employee record

        long int empid = record.value(0).toLongLong();

        QVariant id = store(empid);

        if(databaseManager.deleteEmployee(id))
        {
            QMessageBox::information(this, tr("Employee Delete"),tr("Successfully Deleted Employee"));
        }

        else
        {
            QMessageBox::warning(this, tr("Employee Delete"),tr("Couln Not Delete Employee"));
        }

        employeeTable->select();
}

void interface::on_viewtablesButton_clicked()
{

  ui->interfaceForms->setCurrentIndex(1);        //view table focus
}

void interface::on_addcustomerButton2_clicked()
{
    addCustomerButton_clicked(); //add customer form focus
}

void interface::on_addSupplierButton_clicked()
{
    AddSupplierButton_clicked();             //add customer focus
}

void interface::on_addUserButton_clicked()
{
    addUserButton_clicked();            //add user focus
}

void interface::on_addemployeebutton_clicked()
{
    addEmployeeButton_clicked();          //add employee focus

}

void interface::on_ADDSupplierButton_clicked()
{
    ui->Items->hide();
    ui->User->hide();
    ui->Employee->hide();
    ui->customers->hide();                     //add supplier form show
    ui->SalesPoint->hide();
    ui->tableViews->hide();

    ui->Suppliers->show();

}

void interface::EmpclearButtonclicked()
{
    ui->empFname->clear();
    ui->empID->clear();
    ui->empSName->clear();
    ui->empLName->clear();
    ui->empDOB->clear();                     //clearing employee entries
    ui->empEmail->clear();
    ui->empPhone->clear();
    ui->empSSNo->clear();

}

void interface::on_addItemtoTable_clicked()
{

    AddItemsButton_clicked();
}

void interface::on_supplierClear_2_clicked()
{
    ui->supplierName_2->clear();
    ui->supplerAddress_2->clear();
    ui->supplierOfficePhone_2->clear();     //clearing supplier form entries
    ui->supplierMobilePhone_2->clear();
    ui->supplierEmail_2->clear();
    ui->supplierCity_2->clear();
}

void interface::on_itemClear_clicked()
{
    ui->itemBar->clear();
    ui->itemName->clear();
    ui->ItemBrand->clear();
    ui->itemNo->clear();
    ui->ItemPrice->setValue(0);
    ui->bulkPrice->setValue(0);                  //clearing item form
    ui->ItemsOnStock->setValue(0);
    ui->reorderLevel->setValue(0);
}

void interface::on_userClear_clicked()
{

    ui->userName->clear();
    ui->UserPassword->clear();          //clears form
    ui->userConfirm->clear();
    ui->userEmpID->clear();
}

void interface::salesController()
{
    ui->BarcodeSold->setFocus();
}

void interface::on_BarcodeSold_returnPressed()
{
    int i = 0;

    amountoftimes++;

    set_new_handler(newFailed);

    QString itemName;

    QString price;

    QVariant barcode = store(ui->BarcodeSold->text().toLong());

    QStringList nameandprice= databaseManager.searchBarcode(barcode);        //searchs for barcode in database
    try
    {
            if(nameandprice.size() == 0)
            {
                throw out_of_range("Not Registered");
            }

            itemName = nameandprice[i];

            i++;
            checkBounds(i,nameandprice.size());
            price = nameandprice[1];
    }
    catch(out_of_range &put)
    {
        QMessageBox::warning(this,tr("Item Scanned"),tr("Item Scanned not registed. Please Consult Ware House Manager or Administrator"));
        on_salesClearButton_2_clicked();            //database not found
        return;
    }

    QString priceInDollars = "$" + price;

    ui->itemsSoldWidget->setItem(row,0,new QTableWidgetItem(itemName));
    ui->itemsSoldWidget->setItem(row,column,new QTableWidgetItem(priceInDollars));

    bool ok;
     QString amount = QInputDialog::getText(this, tr("Items"), tr("Amount:"),QLineEdit::Normal, QString(), &ok); //Message asking user input

     if(ok)
     {

         int myNumber = amount.toInt();

         QTableWidgetItem *theItem = new QTableWidgetItem();

         theItem->setData(Qt::EditRole, myNumber);                 //asks for how many items will be bought

         ui->itemsSoldWidget->setItem(row,1,theItem);
     }

     else
     {
         return;
     }


    int itemnumber = ui->itemsSoldWidget->item(row,1)->text().toInt();              //gets item number from QTableWidget

    CalculateTotal(itemnumber,price.toDouble());

    row ++ ;

    ui->BarcodeSold->clear();

    ui->BarcodeSold->setFocus();
}

void interface::CalculateTotal(int items, double price)//Calculates Total
{
    double totalPrice = price * items;


    double gst = 1.125;

    totalPrice *= gst;

    TOTAL += totalPrice;


    QString valueAsString = QString::number(TOTAL,'f',2);

    ui->SalesTotal_2->setText("$" + valueAsString);
}

void interface::calculateChange(double paidAmount)//Calculates Change
{

    QString paid = QString::number(paidAmount,'f',2);

    double change = paidAmount - TOTAL;

    QString changeintext = QString::number(change,'f',2);

    ui->salesChange_2->setText("$" + changeintext);

    QMessageBox::information(this,tr("Ready for Check Out?"),"Total Amount Paid ......... $" + paid+"\n"
                                                                 "Change is ................... $"+changeintext ,QMessageBox::Ok,
                                                                  QMessageBox::Cancel);

        salesCheckoutButton_clicked("$" + paid,"$"+changeintext);



}

void interface::on_salesCashPaid_returnPressed()//gets cash entered
{
    if(ui->salesCashPaid->text().toInt() < TOTAL)
    {
        QMessageBox::warning(this,tr("Invalid Cash Field"),tr("The Cash Paid Field is less than Total amount to be paid, Please Verify and try again"));
        return;
    }
    double amount = ui->salesCashPaid->text().toDouble();

    calculateChange(amount);

}

void interface::salesCheckoutButton_clicked(QString amount, QString change) // When purchase is complete
{
    itemControl();

    ofstream fileout;

    QString recNo = databaseManager.receipts();

    QString receipt = "receipt"+recNo+".txt";

    qDebug()<<receipt;

    QString filename = "../Object_Oriented_Project/Receipts/"+receipt;

    QByteArray ba = filename.toLatin1();

    fileout.open(ba.data());

    fileout.close();

    QString teller = ui->SalesTellerName->text();

    QString changeinText = QString::number(TOTAL,'f',2);

    int AmountPaid = (TOTAL * 0.89);

    QString nettotal = QString::number(AmountPaid,'f',2);


    QString Your_Change = change;

    QString tax = ui->salesTax_2->text();

    fileout.seekp(0,std::ios::end);

    QFile file(ba.data());

    file.open(QIODevice::WriteOnly | QIODevice::Text);




    QTextStream outStream(&file);
    outStream << "Current Time:......................."<<text<<endl;
    outStream << "current Date:......................."<<dateString<<endl;
    outStream << "teller.............................."<<teller<<endl;
    outStream << "Receipt #..........................."<<recNo<<endl<<endl;
    outStream << "Net Total..........................$"<<nettotal<<endl;
    outStream << "Tax................................."<<tax<<endl;
    outStream << "Gross Total........................$"<<changeinText<<endl;
    outStream << "Cash Paid..........................."<<amount<<endl;
    outStream << "Your Change........................."<<Your_Change<<endl;

    outStream<<"\t"<<"Have a Nice Day!!!!!"<<endl;
    outStream<<"\t"<<"Return Soon!!!!"<<endl;

     qDebug()<<"receipt printed";

    if(databaseManager.InsertIntoReceipt(text,dateString,teller,recNo,nettotal,tax,changeinText,amount,Your_Change))
    {
        QMessageBox::information(this,tr("Receipt Generated"),tr("Receipt sent to Printer"));
         on_salesClearButton_2_clicked();
    }
    else
    {
        QMessageBox::warning(this,tr("Receipt Not Generated"), tr("Error in generating and Printing Receipt Please Contact System Administrator"));
        return;
    }



}

void interface::on_updateEmployeeButton_clicked()// updatesEmlopyee
{
    int i = 0;
    QStringList list;
    QModelIndex index = ui->empTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = employeeTable->record(index.row());

    long int empid = record.value(0).toLongLong();

    QVariant id = store(empid);

    list = databaseManager.findEmployeeToEdit(id);
try
    {
    ui->empID->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empFname->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empSName->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empLName->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empDOB->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empSSNo->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empPhone->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empEmail->setText(list[i]);
    i++;
    checkBounds(i,list.size());
    ui->empGender->setCurrentText(list[i]);
    }

    catch(out_of_range &rangeFail)
    {
        QMessageBox::warning(this,tr("List out of Range"),tr(rangeFail.what()));
        return;
    }

    ui->empID->setEnabled(false);



    addEmployeeButton_clicked();


}

void interface::on_Update_clicked()// does updates
{

    id = ui->empID->text().toLongLong();

    fname = ui->empFname->text();

    mname = ui->empSName->text();

    lname = ui->empLName->text();

    dob = ui->empDOB->text();

    ssno = ui->empSSNo->text();

    phone = ui->empPhone->text();

    email = ui->empEmail->text();

    gender = ui->empGender->currentText();


    if((fname == "") | (lname == "") | (dob == "") | (gender == "") | (id == 0))
    {
        QMessageBox::warning(this,tr("Employee Entry"),tr("One or more fields Empty! verify and Try again"));

        return;
    }

    Employees Employee(id,fname,mname,lname,dob,ssno,phone,email,gender);

    PERSONS = &Employee;

    ID = PERSONS->getid();

    FNAME = PERSONS->getFname();

    MNAME = PERSONS->getMname();

    LNAME = PERSONS->getLname();

    DOB = PERSONS->getDOB();

    SSNO = PERSONS->getSSNO();

    PHONE = PERSONS->getphoneNo();

    EMAIL = PERSONS->getEmail();

    GENDER = PERSONS->getGender();

    qDebug()<<ID<<FNAME<<MNAME<<LNAME<<DOB<<SSNO<<PHONE<<EMAIL<<GENDER;

    if(databaseManager.updateEmployee(ID,FNAME,MNAME,LNAME,DOB,SSNO,PHONE,EMAIL,GENDER))
    {

      QMessageBox::information(this,"Employee Update","Employee Update Successful");

      employeeTable->select();

      EmpclearButtonclicked();

      on_viewtablesButton_clicked();

    }

    else
    {
        QMessageBox::warning(this,tr("Employee Update"), tr("Failed to Update Employee"));

       return;
    }
}

void interface::on_updateCustomerButon_clicked()//calls function update
{
    QStringList list;

    QModelIndex index = ui->customertableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = customersTable->record(index.row());

    long int custid = record.value(0).toLongLong();

    QVariant id = store(custid);

    list = databaseManager.findCustomerToEdit(id);

    ui->custID->setText(list[0]);
    ui->customerFName->setText(list[1]);
    ui->customerSName->setText(list[2]);
    ui->customerLName->setText(list[3]);
    ui->customerDOB->setText(list[4]);
    ui->customerSocial->setText(list[5]);
    ui->customerPhone1->setText(list[6]);
    ui->customerEmail->setText(list[7]);
    ui->customerGender->setCurrentText(list[8]);

    ui->custID->setEnabled(false);

    ui->CustomerSubmit->setEnabled(false);

    addCustomerButton_clicked();
}

void interface::on_customerUpdateButton_clicked()//updates customer from form
{
    id = ui->custID->text().toLongLong();

    fname = ui->customerFName->text();

    mname = ui->customerSName->text();

    lname = ui->customerLName->text();

    dob = ui->customerDOB->text();

    ssno = ui->customerSocial->text();

    phone = ui->customerPhone1->text();

    email = ui->customerEmail->text();

    gender = ui->customerGender->currentText();


    if((fname == "") | (lname == "") | (dob == "") | (gender == "") | (id == 0))
    {
        QMessageBox::warning(this,tr("Customer Update"),tr("One or more fields Empty! verify and Try again"));

        return;
    }

    Customers customers(id,fname,mname,lname,dob,ssno,phone,email,gender);

    PERSONS = &customers;

    ID = PERSONS->getid();

    FNAME = PERSONS->getFname();

    MNAME = PERSONS->getMname();

    LNAME = PERSONS->getLname();

    DOB = PERSONS->getDOB();

    SSNO = PERSONS->getSSNO();

    PHONE = PERSONS->getphoneNo();

    EMAIL = PERSONS->getEmail();

    GENDER = PERSONS->getGender();

    qDebug()<<ID<<FNAME<<MNAME<<LNAME<<DOB<<SSNO<<PHONE<<EMAIL<<GENDER;

    if(databaseManager.updateCustomer(ID,FNAME,MNAME,LNAME,DOB,SSNO,PHONE,EMAIL,GENDER))
    {

      QMessageBox::information(this,"Customer Update","Customer Updated Successfully");

      customersTable->select();

      clear();

      on_viewtablesButton_clicked();

    }

    else
    {
        QMessageBox::warning(this,tr("Customer Update"), tr("Failed to Update Customer"));

       return;
    }
}

void interface::on_deleteuserButton_clicked()// self explantiory
{
    QModelIndex index = ui->userTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = usersTable->record(index.row());

     int empid = record.value(0).toInt();

    QVariant id = store(empid);

    if(databaseManager.deleteUser(id))
    {
        QMessageBox::information(this, tr("User Delete"),tr("Successfully Deleted User"));
    }

    else
    {
        QMessageBox::warning(this, tr("User Delete Delete"),tr("Could Not Delete User"));
    }

    usersTable->select();
}

void interface::on_deleteitemButton_clicked()//same thing sir
{
    QModelIndex index = ui->itemTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = itemsTable->record(index.row());

     long int barcode = record.value(0).toLongLong();

    QVariant id = store(barcode);

    if(databaseManager.deleteItem(id))
    {
        QMessageBox::information(this, tr("Item Delete"),tr("Successfully Deleted Item"));
    }

    else
    {
        QMessageBox::warning(this, tr("Item Delete"),tr("Could Not Delete Item"));
    }

    itemsTable->select();
}

void interface::on_deleteSaleRecordButton_clicked()//
{
    QModelIndex index = ui->totalsalestableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = totalSales->record(index.row());

     int saleid = record.value(0).toInt();

    QVariant id = store(saleid);

    if(databaseManager.deleteSale(id))
    {
        QMessageBox::information(this, tr("Record Delete"),tr("Successfully Deleted Sale Record"));
    }

    else
    {
        QMessageBox::warning(this, tr("Record Delete"),tr("Could Not Delete Sale Record"));
    }

    totalSales->select();
}

void interface::on_deletesupplierButton_clicked()
{
    QModelIndex index = ui->supplierTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = suppliersTable->record(index.row());

     int suppid = record.value(0).toInt();

    QVariant id = store(suppid);

    if(databaseManager.deletesupplier(id))
    {
        QMessageBox::information(this, tr("Supplier Delete"),tr("Successfully Deleted Supplier"));
    }

    else
    {
        QMessageBox::warning(this, tr("Record Delete"),tr("Could Not Delete Supplier"));
    }

    suppliersTable->select();
}

void interface::on_deletecostumerButton_clicked()
{
    QModelIndex index = ui->customertableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = customersTable->record(index.row());

    long int empid = record.value(0).toLongLong();

    QVariant id = store(empid);

    if(databaseManager.deleteCustomer(id))
    {
        QMessageBox::information(this, tr("Customer Delete"),tr("Successfully Deleted Customer"));
    }

    else
    {
        QMessageBox::warning(this, tr("Customer Delete"),tr("Could Not Delete Customer"));
    }

    customersTable->select();
}

void interface::on_updateUserButton_clicked()
{
    QStringList list;

    QModelIndex index = ui->userTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = usersTable->record(index.row());

    int userid = record.value(0).toLongLong();

    QVariant id = store(userid);

    list = databaseManager.findusertoEdit(id);

    try
    {
      int i = 0;

            QString userids = list[i];

            i++;
            checkBounds(i,list.size());

            user = userids.toInt();


            QString role = list[3];

            int roleid = role.toInt();

           ui->userName->setText(list[i]);
           i++;
           checkBounds(i,list.size());

           ui->UserPassword->setText(list[i]);


           ui->roleID->setCurrentIndex(roleid);

           ui->userEmpID->setText(list[4]);
    }

    catch(out_of_range &outofrange)
    {
        QMessageBox::warning(this,tr("Index Out of Bounds in QString List"),tr(outofrange.what()));
    }

   ui->userSubmit->setEnabled(false);

    addUserButton_clicked();
}

void interface::on_userUpdateButton_clicked()
{
    QString username = ui->userName->text();

    QString password = ui->UserPassword->text();

    QString role_ID = ui->roleID->currentText();

    QString confirm = ui->userConfirm->text();

    int id = databaseManager.getRoleID(role_ID);

    long int empid = ui->userEmpID->text().toLong();

    QVariant ids = store(empid);

    if((username == "") | (password == "") | (confirm == ""))
    {
        QMessageBox::warning(this,tr("User Entry Form"),tr("One or more fields may be Empty, verify and try again"));

        return;
    }
    if(password != confirm)
    {
        QMessageBox::warning(this,tr("Password"),tr("Password does nor match, Please Try again"));

        return;
    }

   if(databaseManager.updateUser(user,username,password,id,ids))
   {
       QMessageBox::information(this,tr("User Update"),tr("User Updated Successfully"));

       usersTable->select();

       on_userClear_clicked();

       on_viewtablesButton_clicked();

   }
   else
   {
       QMessageBox::warning(this,tr("User Update"),tr("Update to Users Record failed"));

   }
}

void interface::on_updateItemButton_clicked()
{
    QStringList list;

    QModelIndex index = ui->itemTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = itemsTable->record(index.row());

    long int code = record.value(0).toLongLong();

    QVariant id = store(code);

    list = databaseManager.findItemtoEdit(id);

    QString price = list[4];
    double price1 = price.toDouble();

    QString bulkp = list[5];
    double price2 = bulkp.toDouble();

    QString units = list[6];

    int amount = units.toInt();

    QString level = list[7];

    int levelorder = level.toInt();

    try
    {
     int i = 0;

     ui->itemBar->setText(list[i]);
     i++;
     checkBounds(i,list.size());

     ui->itemName->setText(list[i]);
     i++;
     checkBounds(i,list.size());

     ui->ItemBrand->setText(list[i]);
     i++;
     checkBounds(i,list.size());

     ui->itemNo->setText(list[i]);

    }

    catch(out_of_range &exceedRange)
    {

      QMessageBox::warning(this,tr("Index Out of Bounds in QString List"),tr(exceedRange.what()));

      return;
    }

     ui->ItemPrice->setValue(price1);

     ui->bulkPrice->setValue(price2);

     ui->ItemsOnStock->setValue(amount);

     ui->reorderLevel->setValue(levelorder);

     on_addItemtoTable_clicked();

     ui->itemBar->setEnabled(false);

}

void interface::checkBounds(int i, int size)
{
    if(i > size)
    {
       throw  out_of_range("Error: Index Requested is out of Range in QStringList Extracted From MYSQL Server");
    }
}

void interface::on_updateItemButton_2_clicked()
{
    QString itemName = ui->itemName->text();

    QString itemBrand = ui->ItemBrand->text();

    double itemPrice = ui->ItemPrice->text().toDouble();

    int itemsOnStock = ui->ItemsOnStock->value();

    int reorderLevel = ui->reorderLevel->value();

    QString itemNO = ui->itemNo->text();

    double stockprice = ui->bulkPrice->text().toDouble();

    long int barc = ui->itemBar->text().toLong();

    QVariant barcode = store(barc);


    if(databaseManager.updateItem(barcode,itemName, itemBrand,itemNO,itemPrice,stockprice,itemsOnStock,reorderLevel))
    {
         QMessageBox::information(this,tr("Item Update"), tr("Item Updated Successfully"));

         itemsTable->select();

         on_itemClear_clicked();

         on_viewtablesButton_clicked();

    }
    else
    {
        QMessageBox::warning(this,tr("Item Update"), tr("Error in Updating Item"));
    }
}

void interface::on_salesClearButton_2_clicked()
{
    ui->salesCashPaid->clear();
    ui->BarcodeSold->clear();
    ui->itemsSoldWidget->clearContents();
    ui->itemsSoldWidget->setCurrentCell(0,0);
    ui->salesChange_2->setText("$0.00");
    ui->SalesTotal_2->setText("$0.00");

    TOTAL = 0;
    amountoftimes = 0;
}

void interface::on_salesCheckoutButton_2_clicked()
{
    if(ui->salesCashPaid->text() == "")
    {
        QMessageBox::warning(this,tr("Empty Cash Field"),tr("The Cash Paid Field is Empty, Please Verify and try again"));
        return;
    }
    on_salesCashPaid_returnPressed();
}

void interface::closeEvent(QCloseEvent *e)    //Close event function
{
   databaseManager.removeRoles();
   databaseManager.removeCountry();

    e->accept();

}

void interface::on_logoutButton_clicked()
{

    ui->interfaceForms->hide();
    ui->AddCustomerButton->hide();
    ui->AddEmployeeButton->hide();
    ui->viewtablesButton->hide();
    ui->AddItemButton->hide();
    ui->AddUserButton->hide();
    ui->ADDSupplierButton->hide();
    ui->salesPointButton->hide();

    ui->PasswordInput->clear();

    ui->interfaceForms->hide();

    validateEmployeeLogin();

}

void interface::on_SupplierSubmit_2_clicked()
{
    insertInSuppliers();
}

void interface::on_updateSupplierButton_clicked()
{
    QStringList list;
    QString country;

    QModelIndex index = ui->supplierTableView->currentIndex();

    if (!index.isValid())
        return;


    QSqlRecord record = suppliersTable->record(index.row());

    int id = record.value(0).toInt();

    list = databaseManager.findSupplierToEdit(id);

    supplierId= list[0];

    QString ids = list[7];

    country = databaseManager.findCountry(ids.toInt());

    ui->supplierName_2->setText(list[1]);
    ui->supplerAddress_2->setText(list[2]);
    ui->supplierOfficePhone_2->setText(list[3]);
    ui->supplierMobilePhone_2->setText(list[4]);
    ui->supplierEmail_2->setText(list[5]);
    ui->supplierCity_2->setText(list[6]);
    ui->supplierCountry_2->setCurrentText(country);

     on_ADDSupplierButton_clicked();
}

void interface::on_supplierUpdateButton_clicked()//updating supplier
{
    int i = 0;

     QString name = ui->supplierName_2->text();

     QString address = ui->supplerAddress_2->text();

     QString officePhone = ui->supplierOfficePhone_2->text();

     QString mobilePhone = ui->supplierMobilePhone_2->text();

     QString email = ui->supplierEmail_2->text();

     QString city = ui->supplierCity_2->text();

     QString country = ui->supplierCountry_2->currentText();

      Address addressive(address,officePhone,mobilePhone,email,city,country);

     suppliers supplierInformation(name,addressive);

     QString supplierName = supplierInformation.getsupplierName();

     QStringList fullAddress = supplierInformation.getAddress();

     name = supplierName;

      address = fullAddress[i];
      i++;
      checkBounds(i,supplierName.size());

      officePhone = fullAddress[i];
      i++;
      checkBounds(i,supplierName.size());

      mobilePhone = fullAddress[i];
      i++;
      checkBounds(i,supplierName.size());

      email = fullAddress[i];
      i++;
      checkBounds(i,supplierName.size());

      city = fullAddress[i];
      i++;
      checkBounds(i,supplierName.size());

      country = fullAddress[i];


     int country_ID = databaseManager.countryId(country);

     QVariant suppid = store(supplierId.toLongLong());

     if(databaseManager.updateSuppliers(suppid,name,address,officePhone,mobilePhone,email,city,country_ID))
     {
         QMessageBox::information(this,tr("Supplier Updated"), tr("Supplier Updated Successfully"));

         suppliersTable->select();

         on_supplierClear_2_clicked();

         ui->Suppliers->hide();

         ui->tableViews->show();

         on_viewtablesButton_clicked();



     }
     else
     {
         QMessageBox::information(this,tr("Supplier Update"), tr("Unable to update supplier"));
         return;
     }
}

void interface::on_action_Close_triggered()
{
    close();
}

void interface::on_action_Maximize_triggered()//maximizing window
{
    this->setWindowState(Qt::WindowMaximized);
}

void interface::on_action_Minimize_triggered()
{
    this->setWindowState(Qt::WindowMinimized);
}

void interface::on_actionAdd_User_triggered()//adding usre from toolbar
{
    addUserButton_clicked();
}

void interface::on_action_Add_Customer_triggered()
{
    addCustomerButton_clicked();

}

void interface::on_action_Add_Supplier_triggered()//adding suppliers
{
    AddSupplierButton_clicked();
}

void interface::on_action_Add_Item_triggered()
{
    AddItemsButton_clicked();
}

void interface::on_action_About_Program_triggered()//info about the program
{
    QMessageBox::information(this,tr("About Program"),tr("This Program is Programmed using C++ and uses MYSQL Server,\n it is intended to "
                                                         "provide efficiency in Sales managing all its information in the Database."));
}

void interface::on_action_About_Corona_Hardware_triggered()
{
    QMessageBox::information(this,tr("Corona Hardware"),tr("Corona Hardware was Created on May 8 2017 By:\n"
                                                           "David Hernandez\nJose Urbina\nRandy Tzib,\nDane Cabb\n"
                                                           "Asher Mckoy\nNadir Chi\n"
                                                           "Copyright in this country is governed by the Belize Copyright Act, Chapter "
                                                           "252 of the Laws of Belize. According to the Act, copyright protection is automatic"
                                                           " upon the creation of the work, which clearly indicates that the works need not be registered"
                                                           " for it to be protected in this country."));
}

void interface::on_action_Sales_Point_triggered()
{
   salesPoint();
}

void interface::on_action_View_Tables_triggered()//prompts to view tables
{
    on_viewtablesButton_clicked();
}

void interface::on_action_Add_Emplloyee_triggered()
{
    addEmployeeButton_clicked();
}

void interface::on_action_Hide_Toolbar_triggered()
{
    ui->toolBar->setVisible(false);
}

void interface::on_action_Log_Out_triggered()//logs out of the system
{
    on_logoutButton_clicked();
}

void interface::on_action_Show_Toolbar_triggered()//shows the toolbar
{
    ui->toolBar->setVisible(true);
}


void interface::on_action_English_triggered()//changes language to english
{
    QString qmPath = ":/languages/"; //path of the qm files

    appTranslator.load("txteditor_en", qmPath); //loading the file names

    qtTranslator.load("qt_en", qmPath); //loading qt file names

    ui->retranslateUi(this); //retranslating the text editor

    statusBar()->showMessage(tr("Language has Changed to %1").arg("English"),2000); //notifiing the user that the language changed t enlish

    QPixmap pict(":/images/staff_login.png");
    ui->loginImage->setPixmap(pict);

    validatePassword();


}

void interface::on_action_Spanish_triggered()//changes the language to spanish
{
    QString qmPath = ":/languages/"; //path of the qm files

    appTranslator.load("txteditor_es", qmPath); //loading the file names

    qtTranslator.load("qt_en", qmPath); //loading qt file names

    ui->retranslateUi(this); //retranslating the text editor

    statusBar()->showMessage(tr("Idioma Cambiado a %1").arg("Espanol"),2000); //notifiing the user that the language changed t enlish

    QPixmap pict(":/images/staff_login.png");
    ui->loginImage->setPixmap(pict);

    validatePassword();
}

void interface::itemControl()//removes items sold from database
{
    int quantity;
    QString itemName;

    for(int i = 0 ; i < amountoftimes;i++)
    {
        itemName = ui->itemsSoldWidget->item(i,0)->text();

        quantity = ui->itemsSoldWidget->item(i,1)->text().toInt();


        if(itemName != "" )
        {

          databaseManager.subtractItems(itemName,quantity);

          itemsTable->select();
        }

    }
    return;

}
