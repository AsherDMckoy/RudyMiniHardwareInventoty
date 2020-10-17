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
#include "mysqldatabase.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QTableView>
#include <QDebug>
#include <sys/time.h>
#include <QtSql/QSqlRelation>
#include <stdio.h>
#include <QFormLayout>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QProgressBar>
#include <QWidget>
#include <QSqlRecord>


mysqldatabase::mysqldatabase()
{

}
void mysqldatabase::loadDatabase(QString password, QString username, QString host)
{
    QSqlDatabase db;


    if (QSqlDatabase::contains())
     {
         db = QSqlDatabase::database(QLatin1String(QSqlDatabase::defaultConnection), false); //connection to database MYSQl
     }
     else
     {
         db = QSqlDatabase::addDatabase("QMYSQL");
        //loadDatabase("davidhernandez00", "root", "localhost");//MAY CHANGE DEPEDING ON YOUR MACHNE
     }


    db.setHostName(host);
    db.setUserName(username);
    db.setPassword(password);

    QSqlDatabase::database();

    createDatabase();

    createTables();

}

void mysqldatabase::createDatabase()
{
            QSqlQuery query;

   query.prepare("CREATE DATABASE IF NOT EXISTS System_Database");

   if(query.exec())
   {
       qDebug()<<"succesfully created database";
   }

}

void mysqldatabase::createTables()
{
   //                                                  CREATING TABLES                          //


     QSqlQuery("USE System_Database");

    QSqlQuery("PRAGMA foreign_keys=ON");

    QSqlQuery("CREATE TABLE IF NOT EXISTS Items(barcode BIGINT PRIMARY KEY NOT NULL, Item_Name VARCHAR(15), Item_Brand VARCHAR(15), Item_NO VARCHAR(25), Unit_Price REAL, bulk_Price REAL,"
              "Units_On_Stock NUMERIC, Reorder_Level NUMERIC)");

    QSqlQuery("CREATE TABLE IF NOT EXISTS Country_Table(Country_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL, Country_Code VARCHAR(5), Country_Name VARCHAR(40))");


    QSqlQuery("CREATE TABLE IF NOT EXISTS Role_Table(Role_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL, role_Type VARCHAR(50), role_Description VARCHAR(160))");

    insertIntoRoles();

    QSqlQuery("CREATE TABLE Suppliers_Table(Supplier_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL, supplier_Name VARCHAR(40), supplier_Address VARCHAR(50),"
              "supplier_office_phone VARCHAR(20), supplier_mobile_phone VARCHAR(20), supplier_EMail VARCHAR(40), supplier_City VARCHAR(40),"
              "Country_ID INT, FOREIGN KEY(Country_ID) REFERENCES Country_Table(Country_ID))");

    QSqlQuery("CREATE TABLE Customers(Customer_ID BIGINT PRIMARY KEY NOT NULL, Customer_FName VARCHAR(20),Customer_Mid_Name VARCHAR(20), Customer_LName VARCHAR(20),"
              " Customer_DOB VARCHAR(20),SSNO VARCHAR(9) NOT NULL, Customer_Phone VARCHAR(20), Customer_Email VARCHAR(25),gender VARCHAR(7), member VARCHAR(3))");


    QSqlQuery ("CREATE TABLE Employees(Employee_ID BIGINT PRIMARY KEY NOT NULL,First_Name VARCHAR(30),Second_Name VARCHAR(40), Last_Name VARCHAR(40), Date_of_Birth VARCHAR(30),SS_number VARCHAR(9),"
               " Phone_Number VARCHAR(20), E_Mail VARCHAR(40), Gender VARCHAR(8))");

    QSqlQuery("CREATE TABLE Item_Supplier(item_Supplier_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL,barcode BIGINT,Supplier_ID INT,"
              "FOREIGN KEY(barcode) REFERENCES Items(barcode), FOREIGN KEY(Supplier_ID) REFERENCES Suppliers_Table(Supplier_ID) )");


    QSqlQuery("CREATE TABLE Order_Details(Order_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL, order_Amount REAL, barcode BIGINT, Employee_ID BIGINT, Supplier_ID INT,"
              "FOREIGN KEY(barcode) REFERENCES Items(barcode),FOREIGN KEY(Supplier_ID) REFERENCES Suppliers_Table(Supplier_ID),"
              "FOREIGN KEY(Employee_ID) REFERENCES Employees(Employee_ID))");


    QSqlQuery("CREATE TABLE Users(User_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL, username VARCHAR(15), Password VARCHAR(25),Role_ID INT, Employee_ID BIGINT,"
              "FOREIGN KEY(Employee_ID) REFERENCES Employees(Employee_ID),FOREIGN KEY(Role_ID) REFERENCES Role_Table(Role_ID))");

    QSqlQuery("CREATE TABLE Receipt(Receipt_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL,Time VARCHAR(20), Date VARCHAR(20),Teller VARCHAR(25), ReceiptNo VARCHAR(20),"
              "NetTotal VARCHAR(20), Taxable VARCHAR(10), GrossTotal VARCHAR(10), CashPaid VARCHAR(10), YourChange VARCHAR(10))");


   QSqlQuery("CREATE TABLE TOTAL_SALES(SALE_ID INT AUTO_INCREMENT PRIMARY KEY NOT NULL,Date Varchar(20), Total REAL)");



}

int mysqldatabase::insertIntoEmployees(QVariant id,QString fname, QString sname, QString lname, QString dob, QString ssno, QString phoneNo, QString email, QString gender)
{
    QSqlQuery command;

    command.prepare("INSERT INTO Employees(Employee_ID, First_Name, Second_Name, Last_Name, Date_of_Birth, SS_number, Phone_Number, E_Mail, Gender)"
                    "VALUES(:id,'"+fname+"', '"+sname+"','"+lname+"','"+dob+"','"+ssno+"','"+phoneNo+"','"+email+"','"+gender+"')");

    command.bindValue(":id",id);

    return command.exec();


}

int mysqldatabase::insertIntoUsers(QString username, QString password, int role_ID, QVariant emp_ID)
{
   QSqlQuery query;

   query.prepare("INSERT INTO Users(username, Password,Role_ID,Employee_ID) VALUES('"+username+"','"+password+"',:role, :emp)");

   query.bindValue(":role", role_ID);

   query.bindValue(":emp", emp_ID);

   return query.exec();

}

void mysqldatabase::populateCountry(const QString &filename,const QString &file1)
{
    QSqlQuery command;

    command.exec("ALTER TABLE Country_Table AUTO_INCREMENT = 0");

     QProgressBar * bar = new QProgressBar(0);



     bar->setMinimum(0);
     bar->setMaximum(0);

    QSqlQuery query;

    QFile file(filename);

    QStringList words;

    QStringList country;

    QFile codes(file1);

    bar->setToolTip("Loading");


    bar->setFocus();


           if (!file.open(QFile::ReadOnly))
           {
               qDebug()<<"failed to open file";
           }

        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            if (!line.isEmpty())
                words << line.trimmed();
        }

        if(!codes.open(QFile::ReadOnly))
        {
            qDebug()<<"failed to open country codes";
        }


        while(!codes.atEnd())
        {
            QByteArray lines = codes.readLine();

            if(!lines.isEmpty())
            {
                country<<lines.trimmed();

            }
        }


    for(int i = 0; i < words.size();i++)
    {
        query.exec("INSERT INTO Country_Table(Country_Code,Country_Name) VALUES('"+ country[i] + "','"+words[i]+"')");

    }

}

int mysqldatabase::insertintoItems(QVariant barcode, QString itemname, QString itembrand, QString itemNO, double price, double bulkprice, int itemsonstock, int reorderLevel)
{
    QSqlQuery query;

    query.prepare("INSERT INTO Items(barcode,Item_Name, Item_Brand,Item_NO, Unit_Price,bulk_Price,Units_On_Stock,Reorder_Level) VALUES("
                  ":barcode,'"+itemname+"','"+itembrand+"','"+itemNO+"',:price,:price2,:items,:level)");

    query.bindValue(":barcode",barcode);

    query.bindValue(":price", price);

    query.bindValue(":price2",bulkprice);

    query.bindValue(":items",itemsonstock);

    query.bindValue(":level", reorderLevel);

    return query.exec();
}

void mysqldatabase::removeCountry()
{
    QSqlQuery command;
    command.exec("SET FOREIGN_KEY_CHECKS = 0");

    command.prepare("DELETE FROM Country_Table");

    command.exec();

    command.exec("SET FOREIGN_KEY_CHECKS = 1");
}

int mysqldatabase::insertIntoCustomers(QVariant id, QString fname, QString secName, QString lName, QString DOB, QString SSNO, QString phone, QString email, QString gender, QString member)
{
    QSqlQuery query;

    query.prepare("INSERT INTO Customers(Customer_ID, Customer_FName, Customer_Mid_Name, Customer_LName, Customer_DOB, SSNO,Customer_Phone,Customer_Email,gender,member)"
                  "VALUES(:id, :fname, :mname, :lname,:dob, :ssno, :phone, :email, :gender, :member)");

    query.bindValue(":id",id);
    query.bindValue(":fname",fname);
    query.bindValue(":mname",secName);
    query.bindValue(":lname",lName);
    query.bindValue(":dob",DOB);
    query.bindValue(":ssno",SSNO);
    query.bindValue(":phone",phone);
    query.bindValue(":email",email);
    query.bindValue(":gender",gender);
    query.bindValue(":member",member);

    return query.exec();
}

QStringList mysqldatabase::populateComboBox()
{
    QSqlQuery query;

    QStringList countries;

    query.exec("Select Country_Name From Country_Table");

    while(query.next())
    {
       countries<<query.value(0).toString();
    }

    return countries;

}

int mysqldatabase::countryId(QString country)
{
    QSqlQuery query;

    int id;

    query.prepare("SELECT Country_ID FROM Country_Table WHERE Country_Name = ? ");

    query.bindValue(0,country);

    query.exec();
  while(query.next())
  {
    id = query.value(0).toInt();
  }


    return id;
}

int mysqldatabase::insertIntoSuppliers(QString name, QString address, QString office, QString mobile, QString email, QString city, int country)
{
    QSqlQuery query;

    query.prepare("INSERT INTO Suppliers_Table(supplier_Name , supplier_Address,supplier_office_phone,supplier_mobile_phone,supplier_EMail,supplier_City,Country_ID)"
                  " VALUES('"+name+"','"+address+"','"+office+"','"+mobile+"','"+email+"','"+city+"',:country)");

    query.bindValue(":country",country);


    return query.exec();
}

void mysqldatabase::insertIntoRoles()
{
QSqlQuery query;

    query.exec("INSERT INTO Role_Table(role_Type,role_Description) VALUES('Administrator','Control Overall System')");
    query.exec("INSERT INTO Role_Table(role_Type,role_Description) VALUES('Teller','Sales Point Clerk')");
    query.exec("INSERT INTO Role_Table(role_Type,role_Description) VALUES('Ware House Manager','Input and Control Product Records')");

}

QStringList mysqldatabase::populateRoles()
{
    QSqlQuery query;
    QStringList roles;

    query.exec("SELECT role_Type FROM Role_Table");

    while(query.next())
    {
        roles<<query.value(0).toString();
    }

    return roles;
}

int mysqldatabase::getRoleID(QString role)
{
    QSqlQuery query;

    int id;

    query.prepare("SELECT Role_ID FROM Role_Table WHERE role_Type = ? ");

   query.bindValue(0,role);

   query.exec();

   while(query.next())
   {
       id = query.value(0).toInt();
   }

   return id;
}

int mysqldatabase::deleteEmployee(QVariant id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Employees WHERE Employee_ID = ? ");

    query.bindValue(0,id);

    return query.exec();
}

int mysqldatabase::deleteCustomer(QVariant id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Customers WHERE Customer_ID = ? ");

    query.bindValue(0,id);

    return query.exec();
}

int mysqldatabase::deleteItem(QVariant barcode)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Items WHERE Barcode = ? ");

    query.bindValue(0,barcode);

    return query.exec();

}

QString mysqldatabase::searchEmployee(QVariant empID)
{
    bool found = false;

    QString role;


    QSqlQuery query;

    query.prepare("SELECT Role_ID FROM Users WHERE Employee_ID = :id");

    query.bindValue(":id",empID);

     query.exec();

    while(query.next())
    {
      found = true;
      role = findRole(query.value(0).toInt());
    }

    return role;
}

QString mysqldatabase::findRole(int id)
{
    QSqlQuery query;

    query.prepare("SELECT role_Type FROM Role_Table WHERE Role_ID = :id");

    query.bindValue(":id",id);

    query.exec();

    QString roleName;
    while(query.next())
    {
       roleName = query.value(0).toString();

    }

    return roleName;

}

QString mysqldatabase::userName(QVariant empId)
{
    QSqlQuery query;
    QString username;
    query.prepare("SELECT username FROM Users WHERE Employee_ID = :id");

    query.bindValue(":id",empId);

    query.exec();

    while(query.next())
    {
        username = query.value(0).toString();
    }

    return username;
}

QString mysqldatabase::validateLogin(QString username, QString password)
{
    QSqlQuery query, command;
    QString role;
    int roleID;

    int user = 0, pass = 0;
    command.prepare("SELECT username FROM Users WHERE username = :username");
    command.bindValue(":username",username);
    query.prepare("SELECT Password FROM Users WHERE Password = :password");
    query.bindValue(":password",password);

    command.exec();
    query.exec();

    while(command.next())
    {
        if(command.value(0).toString() == username)
        {
            user = 1;
        }
    }

    while(query.next())
    {
        if(query.value(0).toString() == password)
        {
            pass = 1;
        }
    }

    if((pass == 1)&&(user == 1))
    {
        QMessageBox::information(0 , "Welcome to the System ",query.value(0).toString() +" Successfully Logged in");

        QSqlQuery cmd;
        cmd.prepare("SELECT Role_ID FROM Users Where username = :username");
        cmd.bindValue(":username",username);
        cmd.exec();

        while(cmd.next())
        {
            roleID = cmd.value(0).toInt();
        }

        role = getRoleName(roleID);

        return role;
    }
    else
    {
        QMessageBox::warning(0,"Login Error", "Username or Password may be incorrect or not Registered in Users.\nContact System Administrator.");

    }

}

QStringList mysqldatabase::searchBarcode(QVariant barcode)
{
    QSqlQuery query,command;
    QStringList informationRequested;


    query.prepare("SELECT Item_Name FROM Items WHERE barcode = :barcode ");

    query.bindValue(":barcode",barcode);

    query.exec();

    command.prepare("SELECT Unit_Price FROM Items WHERE barcode = :barcode ");

    command.bindValue(":barcode",barcode);

    command.exec();

    while(query.next())
    {
        informationRequested << query.value(0).toString();
    }

    while(command.next())
    {
        informationRequested << command.value(0).toString();
    }

    return informationRequested;

}

QStringList mysqldatabase::findEmployeeToEdit(QVariant id)
{
    QSqlQuery query;
    QStringList tmp;

    query.prepare("SELECT * FROM Employees where Employee_ID = :id");

    query.bindValue(":id",id);

    query.exec();

    while(query.next())
    {
        tmp<<query.value(0).toString();
        tmp<<query.value(1).toString();
        tmp<<query.value(2).toString();
        tmp<<query.value(3).toString();
        tmp<<query.value(4).toString();
        tmp<<query.value(5).toString();
        tmp<<query.value(6).toString();
        tmp<<query.value(7).toString();
        tmp<<query.value(8).toString();
    }

    return tmp;

}

QStringList mysqldatabase::findCustomerToEdit(QVariant id)
{
    QSqlQuery query;
    QStringList tmp;

    query.prepare("SELECT * FROM Customers where Customer_ID = :id");

    query.bindValue(":id",id);

    query.exec();

    while(query.next())
    {
        tmp<<query.value(0).toString();
        tmp<<query.value(1).toString();
        tmp<<query.value(2).toString();
        tmp<<query.value(3).toString();
        tmp<<query.value(4).toString();
        tmp<<query.value(5).toString();
        tmp<<query.value(6).toString();
        tmp<<query.value(7).toString();
        tmp<<query.value(8).toString();
    }

    return tmp;
}

QStringList mysqldatabase::findItemtoEdit(QVariant barcode)
{
    QSqlQuery query;

    QStringList tmp;

    query.prepare("SELECT * FROM Items where barcode = :barcode");

    query.bindValue(":barcode",barcode);

    query.exec();

    while(query.next())
    {
        tmp<<query.value(0).toString();
        tmp<<query.value(1).toString();
        tmp<<query.value(2).toString();
        tmp<<query.value(3).toString();
        tmp<<query.value(4).toString();
        tmp<<query.value(5).toString();
        tmp<<query.value(6).toString();
        tmp<<query.value(7).toString();
    }
    return tmp;

}

QStringList mysqldatabase::findSupplierToEdit(int id)
{
    QSqlQuery query;
    QStringList list;

    query.prepare("SELECT * FROM Suppliers_Table WHERE Supplier_ID = :id");

    query.bindValue(":id",id);

    query.exec();

    while(query.next())
    {
        list<<query.value(0).toString();
        list<<query.value(1).toString();
        list<<query.value(2).toString();
        list<<query.value(3).toString();
        list<<query.value(4).toString();
        list<<query.value(5).toString();
        list<<query.value(6).toString();
        list<<query.value(7).toString();
    }

    return list;
}

int mysqldatabase::updateEmployee(QVariant id, QString fname, QString sname, QString lname, QString dob, QString ssno, QString phoneNo, QString email, QString gender)
{
    QSqlQuery query;

    query.prepare("UPDATE Employees SET First_Name = :fname, Second_Name= :sname, Last_Name = :lname, Date_of_Birth = :dob,"
                  " SS_number= :ssno, Phone_Number= :phone, E_Mail = :email, Gender= :geneder WHERE Employee_ID = :id");

    query.bindValue(":id",id);
    query.bindValue(":fname",fname);
    query.bindValue(":sname",sname);
    query.bindValue(":lname",lname);
    query.bindValue(":dob",dob);
    query.bindValue(":ssno",ssno);
    query.bindValue(":phone",phoneNo);
    query.bindValue(":email",email);
    query.bindValue(":gender",gender);

    return query.exec();
}
int mysqldatabase::updateSuppliers(QVariant id,QString name, QString address, QString office, QString mobile, QString email, QString city, int country)
{
    QSqlQuery query;

    query.prepare("UPDATE Suppliers_Table SET supplier_Name = :name, supplier_Address = :address, supplier_office_phone = :office,"
                  "supplier_mobile_phone = :mobile, supplier_EMail = :email, supplier_City = :city, Country_ID = :country WHERE Supplier_ID = :id ");

    query.bindValue(":name",name);
    query.bindValue(":address",address);
    query.bindValue(":office",office);
    query.bindValue(":mobile",mobile);
    query.bindValue(":email",email);
    query.bindValue(":city",city);
    query.bindValue(":country",country);
    query.bindValue(":id",id);

    return query.exec();
}

int mysqldatabase::updateCustomer(QVariant id, QString fname, QString sname, QString lname, QString dob, QString ssno, QString phoneNo, QString email, QString gender)
{
    QSqlQuery query;

    query.prepare("UPDATE Customers SET Customer_FName = :fname,Customer_Mid_Name  = :sname, Customer_LName  = :lname, Customer_DOB = :dob,"
                  " SSNO = :ssno, Customer_Phone= :phone, Customer_Email = :email, gender= :geneder WHERE Customer_ID = :id");

    query.bindValue(":id",id);
    query.bindValue(":fname",fname);
    query.bindValue(":sname",sname);
    query.bindValue(":lname",lname);
    query.bindValue(":dob",dob);
    query.bindValue(":ssno",ssno);
    query.bindValue(":phone",phoneNo);
    query.bindValue(":email",email);
    query.bindValue(":gender",gender);

    return query.exec();
}
int mysqldatabase::updateUser(int id,QString username, QString password, int role_ID, QVariant emp_ID)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET username = :username, Password = :pass, Role_ID = :role, Employee_ID = :emp WHERE User_ID = :id");

    query.bindValue(":username",username);
    query.bindValue(":pass",password);
    query.bindValue(":role",role_ID);
    query.bindValue(":emp",emp_ID);

    query.bindValue(":id",id);

    return query.exec();
}

int mysqldatabase::deleteUser(QVariant id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Users WHERE User_ID = ? ");

    query.bindValue(0,id);

    return query.exec();
}

int mysqldatabase::deleteSale(QVariant id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM TOTAL_SALES WHERE SALE_ID = ? ");

    query.bindValue(0,id);

    return query.exec();
}

int mysqldatabase::deletesupplier(QVariant id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM Suppliers_Table WHERE supplier_ID = ? ");

    query.bindValue(0,id);

    return query.exec();
}

QStringList mysqldatabase::findusertoEdit(QVariant id)
{
    QSqlQuery query;
    QStringList tmp;

    query.prepare("SELECT * FROM Users where User_ID = :id");

    query.bindValue(":id",id);

    query.exec();

    while(query.next())
    {
        tmp<<query.value(0).toString();
        tmp<<query.value(1).toString();
        tmp<<query.value(2).toString();
        tmp<<query.value(3).toString();
        tmp<<query.value(4).toString();
    }

    return tmp;
}

int mysqldatabase::updateItem(QVariant barcode, QString itemname, QString itembrand, QString itemNO, double price, double bulkprice, int itemsonstock, int reorderLevel)
{
    QSqlQuery query;

    query.prepare("UPDATE Items SET Item_Name = :itemname, Item_Brand = :itembrand, Item_NO = :itemno,"
                  "Unit_Price= :price, bulk_Price = :bulk, Units_On_Stock=:items, Reorder_Level = :reorder WHERE barcode = :barcode");

    query.bindValue(":itemname",itemname);
    query.bindValue(":itembrand",itembrand);
    query.bindValue(":itemno",itemNO);
    query.bindValue(":price",price);
    query.bindValue(":bulk",bulkprice);
    query.bindValue(":items",itemsonstock);
    query.bindValue(":reorder",reorderLevel);
    query.bindValue(":barcode",barcode);

    return query.exec();
}
QString mysqldatabase::receipts()
{
    QSqlQuery query;
   QString total;

    query.prepare("SELECT COUNT(*) FROM Receipt");

    query.exec();

    while (query.next())
    {
        total = query.value(0).toString();
    }

    return total;
}

void mysqldatabase::removeRoles()
{
    QSqlQuery query;
    query.exec("SET FOREIGN_KEY_CHECKS = 0");


    query.exec("DELETE FROM Role_Table WHERE Role_ID > 3");

    query.exec("SET FOREIGN_KEY_CHECKS = 1");
}

QString mysqldatabase::getRoleName(int id)
{
    QSqlQuery query;
    QString roleName;

    query.prepare("SELECT role_Type FROM Role_Table where Role_ID = :id");
    query.bindValue(":id",id);
    query.exec();

    while(query.next())
    {
        roleName = query.value(0).toString();
    }
    return roleName;
}

QString mysqldatabase::findCountry(int id)
{
    QSqlQuery query;

    QString country;

    query.prepare("SELECT Country_Name FROM Country_Table WHERE Country_ID = :id ");

    query.bindValue(":id",id);

    query.exec();

    query.next();

    country = query.value(0).toString();

    return country;
}

int mysqldatabase::InsertIntoReceipt(QString time, QString date, QString Teller, QString receiptno, QString nettotal, QString taxable, QString GrossTotal, QString cashPaid, QString YourChange)
{
    QSqlQuery query;

    query.prepare("INSERT INTO Receipt(Time,Date,Teller,ReceiptNo,NetTotal,Taxable,GrossTotal,CashPaid,YourChange ) VALUES("
                  ":time,:date,:teller,:receipt,:net,:tax, :gross,:cash,:change)");

    query.bindValue(":time",time);
    query.bindValue(":date",date);
    query.bindValue(":teller",Teller);
    query.bindValue(":receipt",receiptno);
    query.bindValue(":net",nettotal);
    query.bindValue(":tax",taxable);
    query.bindValue(":gross",GrossTotal);
    query.bindValue(":cash",cashPaid);
    query.bindValue(":change",YourChange);

    return query.exec();
}

void mysqldatabase::subtractItems(QString name, int amount)
{
    int quantity;
    QSqlQuery query,command;
    command.prepare("SELECT * FROM Items WHERE Item_Name = :name");
    command.bindValue(":name",name);
    command.exec();

    while(command.next())
     {
        quantity = command.value(6).toInt();
     }
     quantity = quantity - amount;

    query.prepare("UPDATE Items SET Units_On_Stock = :items WHERE Item_Name = :itemName");

    query.bindValue(":items",quantity);

    query.bindValue(":itemName",name);

     query.exec();
}
