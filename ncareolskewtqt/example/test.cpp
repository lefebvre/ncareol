/*
-------------------------------------------------------------------------
OBJECT NAME:	test.cc

FULL NAME:	Main launcher for SkewtQtTest programs

TYPE:		Qt

COPYRIGHT:	University Corporation for Atmospheric Research, 2003-2004
-------------------------------------------------------------------------
*/

#include <QDialog>
#include <QApplication>
#include "SkewtQtTest.h"

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);

  QDialog d;
  d.resize(1000, 1000);
  d.show();

  SkewtQtTest skewtTest(&d, "skewtTest");

  app.exec();

  return(0);

}	// END MAIN

