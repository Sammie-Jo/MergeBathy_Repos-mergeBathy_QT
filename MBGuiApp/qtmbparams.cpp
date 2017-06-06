#include "qtmbparams.h"
//#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <QDebug>
#include <QString>
#include "mbguiapp.h"
#include <qgsfeaturerequest.h>
#include <QDir>

using namespace Qt;
QtMBParams::~QtMBParams()
{

}
QtMBParams::QtMBParams()
    : gridSpacingX(0), gridSpacingY(0), smoothingScaleX(0), smoothingScaleY(0), refLonX(0), refLatY(0), preSplinerX(0), preSplinerY(0)
{
    enumBits << "x86" << "x64" ;
//    bitStr = 0;
    outputFilename = "";
    enumFilters << "Boxcar" << "Hann" << "Loess" << "Quadloess" << "Kalman";
    smoothingFilter = 3;
    inputFilelist = "";
    rotAngle = 0;
    wrkdir = QDir::temp().path().append("/MBGuiApp");
    inputInMeters = false;

    enumPreSpliners << "" << "GMT Surface" << "MB-System ZGrid";
    preSpliner = 0;
    preSplinerOutputFilename = "";
    preSplinerXYZE = true;
    preSplineUseResults = true;
    preSplineTensionFactor = 0.1;
    //GMT Surface only
    preSplineScaleFactor = 1.96;
    preSplineAlpha = 2.0;

    nThreads = 0;
    modelflag = false;
    noNegDepths = false;
    useUnscaledAvgInputs = false;
    computeOffsets = true;
    smoothinmeters = true;
    gridinmeters = true;
    usePreInterpolatedLocs = false;
    preInterpolatedLocsFile = "";
    kriging = false;
    mbexe = "";

}


QStringList QtMBParams::getArgsList()
{
    //move code for this from MBGuiApp to here!

    return argsList;
}
