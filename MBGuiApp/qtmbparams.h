#ifndef QTMBPARAMS_H
#define QTMBPARAMS_H

#include <iostream>
#include <string>
#include <sstream>
#include <QString>
#include <QSize>
#include <QPoint>
#include <QStringList>

class QtMBParams
{
public:
    QtMBParams();
   ~QtMBParams();

    //Getters
    QStringList getArgsList();
    QString getParamsStr();
    int getBitStr() const { return bitStr; }
    QString getOutputFilename() const { return outputFilename; }
    QSizeF getGridSpacing() const { return QSizeF(gridSpacingX,gridSpacingY); }
    QSizeF getSmoothingScales() const { return QSizeF(smoothingScaleX,smoothingScaleY); }
    int getSmoothingFilter() const { return smoothingFilter; }
    QString getSmoothingFilterName() const { return enumFilters[smoothingFilter].toLower(); }
    QString getInputFilelist() const { return inputFilelist; }
    QPointF getRefLL() const { return QPointF(refLonX,refLatY); }
    double getRotAngle() const { return rotAngle; }
    double getGridSpacingX() const { return gridSpacingX; }
    double getGridSpacingY() const { return gridSpacingY; }
    double getSmoothingScaleX() const { return smoothingScaleX; }
    double getSmoothingScaleY() const { return smoothingScaleY; }
    bool getInputInMeters() const { return inputInMeters; }
    QString getLogfile() const { return logfile; }
    QString getWrkdir() const { return wrkdir; }
    QString getMBexe() const { return mbexe; }

    QString getPreSplinerOutputFilename() const { return preSplinerOutputFilename; }
    QSizeF getPreSplinerSpacing() const { return QSizeF(preSplinerX,preSplinerY); }
    bool getPreSplinerXYZE() const { return preSplinerXYZE; }
    bool getPreSplinerUseResults() const { return preSplineUseResults; }
    double getPreSplinerTensionFactor() const { return preSplineTensionFactor; }
    double getPreSplinerScaleFactor() const { return preSplineScaleFactor; }
    double getPreSplinerAlpha() const { return preSplineAlpha; }

    int getNThreads() const { return nThreads; }
    bool getModelFlag() const { return modelflag; }
    bool getNoNegDepths() const { return noNegDepths; }
    bool getUseUnscaledAvgInputs() const { return useUnscaledAvgInputs; }
    bool getComputeOffsets() const { return computeOffsets; }
    bool getSmoothInMeters() const { return smoothinmeters; }
    bool getGridInMeters() const { return gridinmeters; }
    bool getUsePreInterpolatedLocs() const { return usePreInterpolatedLocs; }
    QString getPreinterpolatedLocsFile() const { return preInterpolatedLocsFile; }
    bool getKriging() const { return kriging; }
    int getPreSpliner() const { return preSpliner; }
    bool getPreinterpLocsUsage() const { return preinterplocsusage; }

    //Setters
    void setMBexe(const QString& i) { mbexe = i; }
    void setWrkdir(const QString& w) { wrkdir = w; }
    void setBitStr(int b) { bitStr = b; }
    void setOutputFilename(const QString& o) { outputFilename = o; }
    void setGridSpacing(double x, double y) { gridSpacingX = x; gridSpacingY = y; }
    void setSmoothingScales(double x, double y) { smoothingScaleX = x; smoothingScaleY = y; }
    void setSmoothingFilter(int s) { smoothingFilter = s; }
    void setInputFilelist(const QString& i) { inputFilelist = i; }
    void setRefLL(double x, double y) { refLonX = x; refLatY = y; }
    void setRotAngle(double a) { rotAngle = a; }
    void setInputInMeters(bool i) { inputInMeters = i; }
    void setLogfile(const QString& l) { logfile = l; }
    QStringList enumFilters;
    QStringList enumBits;
    QStringList enumPreSpliners;

    void setPreSplinerSpacing(double x, double y) { preSplinerX = x; preSplinerY = y; }
    void setPreSplinerOutputFilename(const QString& o) { preSplinerOutputFilename = o; }
    void setPreSplinerXYZE(bool b) { preSplinerXYZE = b; }
    void setPreSplinerUseResults(bool b) { preSplineUseResults = b; }
    void setPreSplinerTensionFactor(double b) { preSplineTensionFactor = b; }
    void setPreSplinerScaleFactor(double b) { preSplineScaleFactor = b; }
    void setPreSplinerAlpha(double b) { preSplineAlpha = b; }

    void setNThreads(int b) { nThreads = b; }
    void setModelFlag(bool b) { modelflag = b; }
    void setNoNegDepths(bool b) { noNegDepths = b; }
    void setUseUnscaledAvgInputs(double b) { useUnscaledAvgInputs = b; }
    void setComputeOffsets(double b) { computeOffsets = b; }
    void setSmoothInMeters(double b) { smoothinmeters = b; }
    void setGridInMeters(bool b) { gridinmeters = b; }
    void setUsePreInterpolatedLocs(bool b) { usePreInterpolatedLocs = b; }
    void setPreinterpolatedLocsFile(const QString& b) { preInterpolatedLocsFile = b; }
    void setKriging(bool b) { kriging = b; }
    void setPreSpliner(int b) { preSpliner = b; }
    void setPreinterpLocsUsage(bool b) { preinterplocsusage = b; }

private:
    QString mbexe;
    QString logfile;
    QString wrkdir;
    int bitStr;
    QString inputFilelist;
    QString outputFilename;
    double gridSpacingX, gridSpacingY;
    double smoothingScaleX, smoothingScaleY;
    int smoothingFilter;
    double refLonX, refLatY;
    double rotAngle;
    bool inputInMeters;
    char paramsStr [250];
    QStringList argsList;


    double preSplinerX, preSplinerY;
    QString preSplinerOutputFilename;

    bool preSplinerXYZE;
    bool preSplineUseResults;
    double preSplineTensionFactor;
    //GMT Surface only
    double preSplineScaleFactor;
    double preSplineAlpha;

    int nThreads;
    bool modelflag;
    bool noNegDepths;
    bool useUnscaledAvgInputs;
    bool computeOffsets;
    bool smoothinmeters;
    bool gridinmeters;
    bool usePreInterpolatedLocs;
    QString preInterpolatedLocsFile;
    bool kriging;
    int preSpliner;
    bool preinterplocsusage;

};

#endif // QTMBPARAMS_H
