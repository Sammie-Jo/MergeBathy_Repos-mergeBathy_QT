/***************************************************************************
qgsmaptoolselectutils.cpp  -  Utility methods to help with select map tools
---------------------
begin                : May 2010
copyright            : (C) 2010 by Jeremy Palmer
email                : jpalmer at linz dot govt dot nz
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <limits>

#include "qgsmaptoolselectutils.h"
//#include "qgisapp.h"
#include "mbguiapp.h"
#include "qgsmessagebar.h"
#include "qgsmapcanvas.h"
#include "qgsvectorlayer.h"
#include "qgsfeature.h"
#include "qgsgeometry.h"
#include "qgsrendererv2.h"
#include "qgsrubberband.h"
#include "qgscsexception.h"
#include "qgslogger.h"
#include "qgis.h"

#include <QMouseEvent>
#include <QApplication>

QgsVectorLayer* QgsMapToolSelectUtils::getCurrentVectorLayer( QgsMapCanvas* canvas )
{
  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( canvas->currentLayer() );
  if ( !vlayer )
  {
    MBGuiApp::instance()->messageBar()->pushMessage(
      QObject::tr( "No active vector layer" ),
      QObject::tr( "To select features, choose a vector layer in the legend" ),
      QgsMessageBar::INFO,
      MBGuiApp::instance()->messageTimeout() );
  }
  return vlayer;
}

void QgsMapToolSelectUtils::setRubberBand( QgsMapCanvas* canvas, QRect& selectRect, QgsRubberBand* rubberBand )
{
  const QgsMapToPixel* transform = canvas->getCoordinateTransform();
  QgsPoint ll = transform->toMapCoordinates( selectRect.left(), selectRect.bottom() );
  QgsPoint lr = transform->toMapCoordinates( selectRect.right(), selectRect.bottom() );
  QgsPoint ul = transform->toMapCoordinates( selectRect.left(), selectRect.top() );
  QgsPoint ur = transform->toMapCoordinates( selectRect.right(), selectRect.top() );

  if ( rubberBand )
  {
    rubberBand->reset( QGis::Polygon );
    rubberBand->addPoint( ll, false );
    rubberBand->addPoint( lr, false );
    rubberBand->addPoint( ur, false );
    rubberBand->addPoint( ul, true );
  }
}

void QgsMapToolSelectUtils::expandSelectRectangle( QRect& selectRect,
    QgsVectorLayer* vlayer,
    QPoint point )
{
  int boxSize = 0;
  if ( vlayer->geometryType() != QGis::Polygon )
  {
    //if point or line use an artificial bounding box of 10x10 pixels
    //to aid the user to click on a feature accurately
    boxSize = 5;
  }
  else
  {
    //otherwise just use the click point for polys
    boxSize = 1;
  }
  selectRect.setLeft( point.x() - boxSize );
  selectRect.setRight( point.x() + boxSize );
  selectRect.setTop( point.y() - boxSize );
  selectRect.setBottom( point.y() + boxSize );
}

void QgsMapToolSelectUtils::setSelectFeatures( QgsMapCanvas* canvas,
    QgsGeometry* selectGeometry,
    bool doContains,
    bool doDifference,
    bool singleSelect )
{
  if ( selectGeometry->type() != QGis::Polygon )
    return;

  QgsVectorLayer* vlayer = QgsMapToolSelectUtils::getCurrentVectorLayer( canvas );
  if ( !vlayer )
    return;

  // toLayerCoordinates will throw an exception for any 'invalid' points in
  // the rubber band.
  // For example, if you project a world map onto a globe using EPSG 2163
  // and then click somewhere off the globe, an exception will be thrown.
  QScopedPointer<QgsGeometry> selectGeomTrans( new QgsGeometry( *selectGeometry ) );

  if ( canvas->mapSettings().hasCrsTransformEnabled() )
  {
    try
    {
      QgsCoordinateTransform ct( canvas->mapSettings().destinationCrs(), vlayer->crs() );

      if ( !ct.isShortCircuited() && selectGeomTrans->type() == QGis::Polygon )
      {
        // convert add more points to the edges of the rectangle
        // improve transformation result
        QgsPolygon poly( selectGeomTrans->asPolygon() );
        if ( poly.size() == 1 && poly.at( 0 ).size() == 5 )
        {
          const QgsPolyline &ringIn = poly.at( 0 );

          QgsPolygon newpoly( 1 );
          newpoly[0].resize( 41 );
          QgsPolyline &ringOut = newpoly[0];

          ringOut[ 0 ] = ringIn.at( 0 );

          int i = 1;
          for ( int j = 1; j < 5; j++ )
          {
            QgsVector v(( ringIn.at( j ) - ringIn.at( j - 1 ) ) / 10.0 );
            for ( int k = 0; k < 9; k++ )
            {
              ringOut[ i ] = ringOut[ i - 1 ] + v;
              i++;
            }
            ringOut[ i++ ] = ringIn.at( j );
          }
          selectGeomTrans.reset( QgsGeometry::fromPolygon( newpoly ) );
        }
      }

      selectGeomTrans->transform( ct );
    }
    catch ( QgsCsException &cse )
    {
      Q_UNUSED( cse );
      // catch exception for 'invalid' point and leave existing selection unchanged
      QgsDebugMsg( "Caught CRS exception " );
      MBGuiApp::instance()->messageBar()->pushMessage(
        QObject::tr( "CRS Exception" ),
        QObject::tr( "Selection extends beyond layer's coordinate system" ),
        QgsMessageBar::WARNING,
        MBGuiApp::instance()->messageTimeout() );
      return;
    }
  }

  QApplication::setOverrideCursor( Qt::WaitCursor );

  QgsDebugMsg( "Selection layer: " + vlayer->name() );
  QgsDebugMsg( "Selection polygon: " + selectGeomTrans->exportToWkt() );
  QgsDebugMsg( "doContains: " + QString( doContains ? "T" : "F" ) );
  QgsDebugMsg( "doDifference: " + QString( doDifference ? "T" : "F" ) );

  QgsRenderContext context = QgsRenderContext::fromMapSettings( canvas->mapSettings() );
  context.expressionContext() << QgsExpressionContextUtils::layerScope( vlayer );
  QgsFeatureRendererV2* r = vlayer->rendererV2();
  if ( r )
    r->startRender( context, vlayer->fields() );

  QgsFeatureRequest request;
  request.setFilterRect( selectGeomTrans->boundingBox() );
  request.setFlags( QgsFeatureRequest::ExactIntersect );
  if ( r )
    request.setSubsetOfAttributes( r->usedAttributes(), vlayer->fields() );
  else
    request.setSubsetOfAttributes( QgsAttributeList() );

  QgsFeatureIterator fit = vlayer->getFeatures( request );

  QgsFeatureIds newSelectedFeatures;
  QgsFeature f;
  QgsFeatureId closestFeatureId = 0;
  bool foundSingleFeature = false;
  double closestFeatureDist = std::numeric_limits<double>::max();
  while ( fit.nextFeature( f ) )
  {
    context.expressionContext().setFeature( f );
    // make sure to only use features that are visible
    if ( r && !r->willRenderFeature( f, context ) )
      continue;

    const QgsGeometry* g = f.constGeometry();
    if ( doContains )
    {
      if ( !selectGeomTrans->contains( g ) )
        continue;
    }
    else
    {
      if ( !selectGeomTrans->intersects( g ) )
        continue;
    }
    if ( singleSelect )
    {
      foundSingleFeature = true;
      double distance = g->distance( *selectGeomTrans );
      if ( distance <= closestFeatureDist )
      {
        closestFeatureDist = distance;
        closestFeatureId = f.id();
      }
    }
    else
    {
      newSelectedFeatures.insert( f.id() );
    }
  }
  if ( singleSelect && foundSingleFeature )
  {
    newSelectedFeatures.insert( closestFeatureId );
  }

  if ( r )
    r->stopRender( context );

  QgsDebugMsg( "Number of new selected features: " + QString::number( newSelectedFeatures.size() ) );

  if ( doDifference )
  {
    QgsFeatureIds layerSelectedFeatures = vlayer->selectedFeaturesIds();

    QgsFeatureIds selectedFeatures;
    QgsFeatureIds deselectedFeatures;

    QgsFeatureIds::const_iterator i = newSelectedFeatures.constEnd();
    while ( i != newSelectedFeatures.constBegin() )
    {
      --i;
      if ( layerSelectedFeatures.contains( *i ) )
      {
        deselectedFeatures.insert( *i );
      }
      else
      {
        selectedFeatures.insert( *i );
      }
    }

    vlayer->modifySelection( selectedFeatures, deselectedFeatures );
  }
  else
  {
    vlayer->setSelectedFeatures( newSelectedFeatures );
  }
//  MBGuiApp::toggleExcludes();
  QApplication::restoreOverrideCursor();
}

void QgsMapToolSelectUtils::setSelectFeatures( QgsMapCanvas* canvas, QgsGeometry* selectGeometry, QMouseEvent * e )
{
  bool doContains = e->modifiers() & Qt::ShiftModifier;
  bool doDifference = e->modifiers() & Qt::ControlModifier;
  setSelectFeatures( canvas, selectGeometry, doContains, doDifference );
}
