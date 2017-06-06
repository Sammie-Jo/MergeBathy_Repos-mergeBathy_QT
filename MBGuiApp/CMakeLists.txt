#minimum required CMake version to 2.8.11 for automatic linking to the qtmain.lib library on Windows.
cmake_minimum_required(VERSION 2.8.11)

PROJECT(MBGuiApp)
########################################################
# CMake environment settings and necessary calls
########################################################
SET(CMAKE_COLOR_MAKEFILE ON)

# Find includes in corresponding build directories
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

# Create the ui_widget.h associated with the widget.ui and widget.cpp
#set(CMAKE_AUTOUIC ON)

# Create rules to execute rcc at the appropriate time on source files which have the suffix .qrc.
#set(CMAKE_AUTORCC ON)

# Needed if running Qt 5.7 with CMake older than 3.1.0
set(CMAKE_CXX_STANDARD 14)

########################################################
# Find QGIS required modules (Imported Targets)
########################################################
IF (WIN32)
  # expect that classes are being imported by default
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  # import qgis classes
  IF (MSVC)
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
    ADD_DEFINITIONS("-DGUI_EXPORT=__declspec(dllimport)")
    ADD_DEFINITIONS("-DCORE_EXPORT=__declspec(dllimport)")
    ADD_DEFINITIONS("-DAPP_EXPORT=__declspec(dllimport)")
#    ADD_DEFINITIONS("-DAPP_EXPORT=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
    ADD_DEFINITIONS("\"-DGUI_EXPORT=__declspec(dllimport)\"")
    ADD_DEFINITIONS("\"-DCORE_EXPORT=__declspec(dllimport)\"")
    ADD_DEFINITIONS("\"-DAPP_EXPORT=__declspec(dllimport)\"")
#    ADD_DEFINITIONS("\"-DAPP_EXPORT=__declspec(dllexport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DGUI_EXPORT=)
  ADD_DEFINITIONS(-DCORE_EXPORT=)
  ADD_DEFINITIONS(-DAPP_EXPORT=)
ENDIF (WIN32)

# set path to additional CMake modules: QGIS, GDAL, GEOS
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_find_rules ${CMAKE_MODULE_PATH})

FIND_PACKAGE(QGIS REQUIRED)
IF (NOT QGIS_FOUND)
  MESSAGE (SEND_ERROR "QGIS dependency was not found!")
ENDIF (NOT QGIS_FOUND)

FIND_PACKAGE(GDAL REQUIRED)
IF (NOT GDAL_FOUND)
  MESSAGE (SEND_ERROR "GDAL dependency was not found!")
ENDIF (NOT GDAL_FOUND)

FIND_PACKAGE(GEOS REQUIRED)
IF (NOT GEOS_FOUND)
  MESSAGE (SEND_ERROR "GEOS dependency was not found!")
ENDIF (NOT GEOS_FOUND)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DMBGuiAppDEBUG=1)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

########################################################
# MBGuiApp project source files
########################################################
# CPP files
SET (MBGuiApp_SRCS
     main.cpp
     qtmbparams.cpp
     qtcanvas.cpp
     mycanvasview.cpp
     mymenuprovider.cpp
     mbguiapp.cpp
     commands.cpp
     attribute_table/qgsattributetabledialog.cpp
#     attribute_table/gui/attributetable/qgsattributetableview.cpp
     attribute_table/qgsdelattrdialog.cpp
     attribute_table/qgsaddattrdialog.cpp
     attribute_table/qgsfieldcalculator.cpp
     attribute_table/qgsclipboard.cpp
     attribute_table/qgsfeatureaction.cpp
     attribute_table/qgsguivectorlayertools.cpp
     attribute_table/qgsidentifyresultsdialog.cpp
     attribute_table/qgsmaptoolidentifyaction.cpp
     attribute_table/ogr/qgsvectorlayersaveasdialog.cpp
     attribute_table/qgsrasterlayerproperties.cpp
#     attribute_table/qgsvectorlayerproperties.cpp
     attribute_table/qgsmaplayerstyleguiutils.cpp
 attribute_table/qgsstatusbarcoordinateswidget.cpp
    attribute_table/qgsattributetypedialog.cpp
    attribute_table/qgsstatisticalsummarydockwidget.cpp
#    attribute_table/qgsmaptoolpinlabels.cpp
#    attribute_table/qgsmaptoollabel.cpp
#    attribute_table/qgsmaptoolshowhidelabels.cpp
#    attribute_table/qgsmaptoolmovelabel.cpp
#    attribute_table/qgsmaptoolrotatelabel.cpp
#    attribute_table/qgsmaptoolchangelabelproperties.cpp
    attribute_table/qgsundowidget.cpp
#    attribute_table/qgsmaptoolrotatefeature.cpp
    attribute_table/qgsmaptoolrotatepointsymbols.cpp
#    attribute_table/core/raster/qgsrasterrendererregistry.cpp
    attribute_table/qgsmaptoolselectutils.cpp
    attribute_table/qgspointrotationitem.cpp
    attribute_table/qgslabelpropertydialog.cpp
    attribute_table/ogr/qgsopenvectorlayerdialog.cpp
    attribute_table/ogr/qgsnewogrconnection.cpp
    attribute_table/ogr/qgsogrhelperfunctions.cpp
    attribute_table/qgswelcomepageitemsmodel.cpp
    attribute_table/qgswelcomepage.cpp
    attribute_table/qgsversioninfo.cpp
#    attribute_table/qgsabout.cpp
    attribute_table/qgsmaptoolselect.cpp
    attribute_table/qgsmaptoolselectfreehand.cpp
    attribute_table/qgsmaptoolselectpolygon.cpp
    attribute_table/qgsmaptoolselectradius.cpp
    attribute_table/qgsmaptoolselectrectangle.cpp
#    attribute_table/qgsmaptoolsimplify.cpp
    attribute_table/nodetool/qgsmaptoolnodetool.cpp
    attribute_table/nodetool/qgsnodeeditor.cpp
    attribute_table/nodetool/qgsselectedfeature.cpp
    attribute_table/nodetool/qgsvertexentry.cpp
#    attribute_table/qgsapplayertreeviewmenuprovider.cpp
    attribute_table/qgsvisibilitypresets.cpp
#    attribute_table/mbguiappinterface.cpp
    propertybrowserextension/variantmanager.cpp
    propertybrowserextension/variantfactory.cpp
    propertybrowserextension/fileedit.cpp
    propertybrowserextension/fileeditfactory.cpp
    propertybrowserextension/filepathmanager.cpp
    attribute_table/qgshandlebadlayers.cpp
)

# UI files
SET (MBGuiApp_UIS
    ui/mbguiappbase.ui
    ui/qgsattributetabledialog.ui
    ui/qgsdualviewbase.ui
#    ui/qgsfieldconditionalformatwidget.ui
    ui/qgsaddattrdialogbase.ui
    ui/qgsdelattrdialogbase.ui
    ui/qgsfieldcalculatorbase.ui
    ui/qgsopenvectorlayerdialogbase.ui
    ui/qgsextentgroupboxwidget.ui
    ui/qgsdatumtransformdialogbase.ui
    ui/qgsidentifyresultsbase.ui
    ui/qgsvectorlayersaveasdialogbase.ui
#    ui/qgsrasterlayerpropertiesbase.ui
#    ui/qgsvectorlayerpropertiesbase.ui
    ui/qgssymbolv2selectordialogbase.ui
    ui/qgsattributetypeedit.ui
    ui/qgsstatisticalsummarybase.ui
    ui/qgsrasterminmaxwidgetbase.ui
    ui/qgsmultibandcolorrendererwidgetbase.ui
    ui/qgspalettedrendererwidgetbase.ui
    ui/qgsrasterhistogramwidgetbase.ui
    ui/qgsattributeloadfrommap.ui
    ui/qgslabelpropertydialogbase.ui
    ui/qgsnewogrconnectionbase.ui
#    ui/qgsabout.ui
    ui/qgsexpressionselectiondialogbase.ui
    ui/qgshandlebadlayersbase.ui
    ui/qgssinglebandgrayrendererwidgetbase.ui
    ui/qgssinglebandpseudocolorrendererwidgetbase.ui
)
if(QT_USE_QT5)
    SET (MBGuiApp_UIS ${MBGuiApp_UIS} ui/qgsfieldconditionalformatwidget.ui ui/qgsrasterlayerpropertiesbase.ui)

else()
    SET (MBGuiApp_UIS ${MBGuiApp_UIS} ui/qgsfieldconditionalformatwidgetV2.ui ui/qgsrasterlayerpropertiesbaseV2.ui)
endif()


# This tut needs no MOC
SET (MBGuiApp_MOC_HDRS
    qtmbparams.h
    qtcanvas.h
    mycanvasview.h
    mymenuprovider.h
    mbguiapp.h
    commands.h
    attribute_table/qgsattributetabledialog.h
    attribute_table/qgsdelattrdialog.h
    attribute_table/qgsaddattrdialog.h
    attribute_table/qgsfieldcalculator.h
    attribute_table/qgsclipboard.h
    attribute_table/qgsfeatureaction.h
    attribute_table/qgsguivectorlayertools.h
    attribute_table/qgsidentifyresultsdialog.h
    attribute_table/qgsmaptoolidentifyaction.h
    attribute_table/ogr/qgsvectorlayersaveasdialog.h
    attribute_table/qgsrasterlayerproperties.h
#    attribute_table/qgsvectorlayerproperties.h
    attribute_table/qgsmaplayerstyleguiutils.h
    attribute_table/qgsstatusbarcoordinateswidget.h
    attribute_table/qgsattributetypedialog.h
    attribute_table/qgsstatisticalsummarydockwidget.h
#    attribute_table/qgsmaptoolpinlabels.h
#    attribute_table/qgsmaptoollabel.h
#    attribute_table/qgsmaptoolshowhidelabels.h
#    attribute_table/qgsmaptoolmovelabel.h
#    attribute_table/qgsmaptoolrotatefeature.h
#    attribute_table/qgsmaptoolrotatelabel.h
#    attribute_table/qgsmaptoolchangelabelproperties.h
    attribute_table/qgsundowidget.h
    attribute_table/qgsmaptoolrotatepointsymbols.h
#    attribute_table/core/raster/qgsrasterrendererregistry.h
    attribute_table/qgsmaptoolselectutils.h
    attribute_table/qgspointrotationitem.h
    attribute_table/qgslabelpropertydialog.h
    attribute_table/ogr/qgsopenvectorlayerdialog.h
    attribute_table/ogr/qgsnewogrconnection.h
    attribute_table/ogr/qgsogrhelperfunctions.h
    attribute_table/qgswelcomepageitemsmodel.h
    attribute_table/qgswelcomepage.h
    attribute_table/qgsversioninfo.h
#    attribute_table/qgsabout.h
    attribute_table/qgsmaptoolselect.h
    attribute_table/qgsmaptoolselectfreehand.h
    attribute_table/qgsmaptoolselectpolygon.h
    attribute_table/qgsmaptoolselectradius.h
    attribute_table/qgsmaptoolselectrectangle.h
#    attribute_table/qgsmaptoolsimplify.h
    attribute_table/nodetool/qgsmaptoolnodetool.h
    attribute_table/nodetool/qgsnodeeditor.h
    attribute_table/nodetool/qgsselectedfeature.h
    attribute_table/nodetool/qgsvertexentry.h
#    attribute_table/qgsapplayertreeviewmenuprovider.h
    attribute_table/qgsvisibilitypresets.h
#    attribute_table/mbguiappinterface.h
    propertybrowserextension/variantmanager.h
    propertybrowserextension/variantfactory.h
    propertybrowserextension/fileedit.h
    propertybrowserextension/fileeditfactory.h
    propertybrowserextension/filepathmanager.h
    attribute_table/qgshandlebadlayers.h
)

SET(IMAGE_RCCS images/images.qrc)
SET (MBGuiApp_RCCS resources.qrc)

SET (QT_USE_QT3SUPPORT FALSE)
SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTSVG TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTNETWORK TRUE)
SET (QT_USE_QTOPENGL TRUE)

########################################################
# Load Qt version-specific modules (Imported Targets)
########################################################
# The result of a find_package call is that imported targets will be created
# for use with target_link_libraries, some variables will be populated with
# information required to configure the build, and macros will be made available for use.
# Check Qt version
SET(QT_USE_QT5 FALSE)
if (QT_USE_QT5)
#    if (Qt5Widgets_VERSION VERSION_LESS 5.7.0)
#        message(FATAL_ERROR "Minimum supported Qt5 version is 5.70!")
#    endif()
    MESSAGE ("Using Qt 5.8.0: /qgis-dev")
    SET(QGISF qgis-dev)
    SET(qtversion "5.0")
    add_definitions(-DQt5)

    # Imported targets
    # Find the libraries
    find_package(Qt5 REQUIRED
                Widgets
    #                WebEngine
                #Webkit #deprecated in favor of WebEngine
                Xml
    #                Q3DSurface
    #                QtCanvas3D
                )
else()
    # The Qt5Widgets library could not be found!
    MESSAGE ("Using Qt 4.8.6: /qgis-ltr")
    SET(QGISF qgis-ltr)
    SET(qtversion "4.7")
    remove_definitions(-DQt5)

    FIND_PACKAGE(Qt4 REQUIRED)
    INCLUDE( ${QT_USE_FILE} )

endif()

IF (WIN32)
    SET(PLATFORM_LIBRARIES wsock32)
ENDIF (WIN32)
########################################################
# Build
########################################################
if(QT_USE_QT5)
    QT5_WRAP_UI (MBGuiApp_UIS_H          ${MBGuiApp_UIS})
    QT5_WRAP_CPP (MBGuiApp_MOC_SRCS      ${MBGuiApp_MOC_HDRS})
    QT5_ADD_RESOURCES(MBGuiApp_RCC_SRCS  ${MBGuiApp_RCCS})
    QT5_ADD_RESOURCES(IMAGE_RCC_SRCS    ${IMAGE_RCCS})
ELSE()
    QT4_WRAP_UI (MBGuiApp_UIS_H          ${MBGuiApp_UIS})
    QT4_WRAP_CPP (MBGuiApp_MOC_SRCS      ${MBGuiApp_MOC_HDRS})
    QT4_ADD_RESOURCES(MBGuiApp_RCC_SRCS  ${MBGuiApp_RCCS})
    QT4_ADD_RESOURCES(IMAGE_RCC_SRCS     ${IMAGE_RCCS})
ENDIF ()
ADD_EXECUTABLE (MBGuiApp MACOSX_BUNDLE WIN32 ${MBGuiApp_SRCS} ${MBGuiApp_MOC_SRCS} ${MBGuiApp_RCC_SRCS} ${MBGuiApp_UIS_H} ${IMAGE_RCC_SRCS})

# Includes
INCLUDE_DIRECTORIES(${QGIS_INCLUDE_DIR}
                     ${GDAL_INCLUDE_DIR}
                     ${GEOS_INCLUDE_DIR}

                     ${QT_INCLUDE_DIR}
                     F:/Sams_Files/FY_2012/MergeBathy_Repos/mergeBathy_Qt/QtPropertyBrowser-master_Qt${qtversion}/QtPropertyBrowser-master/src

                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} attribute_table
                     ${CMAKE_CURRENT_BINARY_DIR} attribute_table/gui
                     ${CMAKE_CURRENT_BINARY_DIR} attribute_table/core/geometry
                     ${CMAKE_CURRENT_BINARY_DIR} attribute_table/core/raster
#                     ${CMAKE_CURRENT_BINARY_DIR}/../ui

                     ${CMAKE_CURRENT_SOURCE_DIR} propertybrowserextension
                     ${CMAKE_CURRENT_SOURCE_DIR}/../ui

                     C:/OSGeo4W/include/qwt
                     .
                    )
# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
if(QT_USE_QT5)
    INCLUDE_DIRECTORIES(${QT5_INCLUDE_DIR}
                        ${QT5Widgets_INCLUDE_DIR}
    #                        ${QT5WebEngine_INCLUDE_DIR}
                        ${QTXml_INCLUDE_DIR}
    #                        ${QTQ3DSurface_INCLUDE_DIR}
    #                        ${QTQtCanvas3D_INCLUDE_DIR}
                        C:/OSGeo4W/apps/Qt5/include
                        )
     # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
        add_definitions(${Qt5Widgets_DEFINITIONS})
ELSE()
    INCLUDE_DIRECTORIES(${QT4_INCLUDE_DIR}
                        C:/OSGeo4W/include/qt4
                        C:/OSGeo4W/include/qt4/QtCrypto
                        C:/OSGeo4W/include/qt4/QtWebKit
                #     F:/Sams_Files/FY_2012/MergeBathy_Repos/mergeBathy_Qt/qwtplot3d-0.2.7/qwtplot3d/include
                        )
ENDIF ()


#This is probably no longer needed, but I will leave it in for Win machines for the moment
IF(QT_QTSQL_FOUND)
 FIND_LIBRARY(QT_QTSQL_LIBRARY NAMES QtSql QtSql4 PATHS ${QT_LIBRARY_DIR} NO_DEFAULT_PATH)
 SET(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QTSQL_LIBRARY})
ENDIF(QT_QTSQL_FOUND)

########################################################
# Find libraries
########################################################
# Get bit
#IF (WIN32)
#    ## Windows common build here
#    IF (!${CMAKE_TARGET.arch, x86_64))
#        message("x86 build")
#        SET (BITS 32bit)
#        ## Windows x86 (32bit) specific build here
#    ELSE
#        MESSAGE (SEND_MESSAGE "x86_64 build!")
#        SET (BITS 64bit)
#        ## Windows x64 (64bit) specific build here
#    ENDIF
#ENDIF
SET (BITS 32bit)


if(QT_USE_QT5)
    FIND_LIBRARY(QT_QTPROPERTYBROWSER_LIBRARY NAMES QtPropertyBrowser PATHS ${QT_LIBRARY_DIR} "F:/Sams_Files/FY_2012/MergeBathy_Repos/mergeBathy_Qt/QtPropertyBrowser-master_Qt5.0/build-Qt_5_8_0_2015_32bit-RelwDeb/bin/")

ELSE()
    FIND_LIBRARY(QT_QTPROPERTYBROWSER_LIBRARY NAMES QtPropertyBrowser PATHS ${QT_LIBRARY_DIR} "F:/Sams_Files/FY_2012/MergeBathy_Repos/mergeBathy_Qt/QtPropertyBrowser-master_Qt4.7/build-QtPropertyBrowser-master-Desktop_Qt_4_8_6_MSVC2010_32bit-Default/bin/")

    # QwtPlot3D or QWTis not working yet
    FIND_LIBRARY(QT_QWTPLOT3D_LIBRARY NAMES qwtplot3d PATHS ${QT_LIBRARY_DIR} "F:/Sams_Files/FY_2012/MergeBathy_Repos/mergeBathy_Qt/qwtplot3d-0.2.7/build-qwtplot3d-Desktop_Qt_4_8_6_MSVC2010_32bit-Release/lib/")
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QWTPLOT3D_LIBRARY})

    FIND_LIBRARY(QT_WEBKIT_LIBRARY NAMES qtwebkitd4 PATHS ${QT_LIBRARY_DIR} "C:/OSGeo4W/lib/")
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${QT_WEBKIT_LIBRARY})

ENDIF ()
SET(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QTPROPERTYBROWSER_LIBRARY})

FIND_LIBRARY(QT_QWT_LIBRARY NAMES qwt PATHS ${QT_LIBRARY_DIR} "C:/OSGeo4W/lib/")
SET(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QWT_LIBRARY})
########################################################
# Link target to libraries
########################################################
TARGET_LINK_LIBRARIES(MBGuiApp
                    ${QWT_LIBRARY}
                    ${QT_LIBRARIES}

                    ${QGIS_CORE_LIBRARY}
                    ${QGIS_GUI_LIBRARY}
                    ${QGIS_ANALYSIS_LIBRARY}
                    #  ${QGIS_NETWORK_ANALYSIS_LIBRARY}
                    ${QGIS_NETWORK_LIBRARY}
                    ${GEOS_LIBRARY}
                    ${GDAL_LIBRARY}
                    )

if(QT_USE_QT5)
        # Executables fail to build with Qt 5 in the default configuration
        # without -fPIE. We add that here.
        set(CMAKE_CXX_FLAGS
            ${Qt5_EXECUTABLE_COMPILE_FLAGS}
            ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}
    #        ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}
    #        ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}
    #        ${Qt5WebEngine_EXECUTABLE_COMPILE_FLAGS}
            ${Qt5Xml_EXECUTABLE_COMPILE_FLAGS}
    #        ${Qt5Q3DSurface_EXECUTABLE_COMPILE_FLAGS}
    #        ${Qt5QtCanvas3D_EXECUTABLE_COMPILE_FLAGS}
            )

        TARGET_LINK_LIBRARIES( MBGuiApp
                            Qt5::Widgets
    #                        Qt5::Core
    #                        Qt5::Gui
    #                        Qt5::WebEngine
                            Qt5::Xml
    #                        Qt5:Q3DSurface
    #                        Qt5:QtCanvas3D
                        #    ${Qt5Widgets_LIBRARIES}
                            )
ENDIF ()

IF (MSVC)
  # Very important or you get all kinds of odd side
  #effects like app crash on start up saying qtgui.dll
  TARGET_LINK_LIBRARIES( MBGuiApp
                         qtmain
                       )
ENDIF (MSVC)
