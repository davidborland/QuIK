/*=========================================================================

  Name:        QuIKVis.cpp

  Author:      David Borland

  Description: Contains the main function for QuIKVis program.  Just 
               instantiates a QApplication object and the application 
               main window using Qt.

=========================================================================*/


#include "qvMainWindow.h"

#include <QApplication>


int main(int argc, char** argv) {
    // Initialize Qt
    QApplication app(argc, argv);

    qvMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

