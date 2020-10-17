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
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    interface w; //inerface object

    //w.interface();
    //mysqldatabase database;

   // database.loadDatabase("davidhernandez00", "root", "localhost");//MAY CHANGE DEPEDING ON YOUR MACHNE

    w.show(); //displaying the object

    return a.exec();
}
