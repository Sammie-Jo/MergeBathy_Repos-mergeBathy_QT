/***************************************************************************
 *   Copyright (C) 2006 by Tim Sutton   *
 *   tim@linfiniti.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

// Qt Includes
#include <QApplication>
#include <mbguiapp.h>

// QGIS Includes
#include <qgsapplication.h>
//#include <QTranslator>

//#ifdef _MSC_VER
//#undef APP_EXPORT
//#define APP_EXPORT __declspec(dllexport)
//#endif

//#ifdef _MSC_VER
//#undef CORE_EXPORT
//#define CORE_EXPORT __declspec(dllexport)
//#endif


int main(int argc, char ** argv)
{
    // 1. Initialize App
    QApplication app(argc, argv);
    #ifdef Qt5
        QgsApplication::setPrefixPath("C:/OSGEO4W/apps/qgis-dev", TRUE);
    #else
        QgsApplication::setPrefixPath("C:/OSGEO4W/apps/qgis-ltr", TRUE);
    #endif
    QgsApplication::initQgis();
    // Show setting of parameters
    //qDebug() << "showSettings? " << QgsApplication::showSettings();

    //2. App start
    MBGuiApp * mbguiapp = new MBGuiApp();
    mbguiapp->show();
    int result = app.exec();
    delete mbguiapp;

    //3. Qgis free libs
    QgsApplication::exitQgis();

    //4. And say goodby
    return result;
}



