/***************************************************************************
                          qgslabelpropertydialog.cpp
                          --------------------------
    begin                : 2010-11-12
    copyright            : (C) 2010 by Marco Hugentobler
    email                : marco dot hugentobler at sourcepole dot ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgslabelpropertydialog.h"
#include <qgsdatadefined.h>
#include <qgsfontutils.h>
#include <qgslogger.h>
#include "qgsmaplayerregistry.h"
#include "qgsmaprenderer.h"
#include "qgsvectorlayer.h"
//#include "qgisapp.h"
#include "mbguiapp.h"
#include "qgsmapcanvas.h"

#include <QColorDialog>
#include <QFontDatabase>
#include <QSettings>
#include <QDialogButtonBox>


QgsLabelPropertyDialog::QgsLabelPropertyDialog( const QString& layerId, int featureId, const QFont& labelFont, const QString& labelText, QWidget * parent, Qt::WindowFlags f ):
    QDialog( parent, f ), mLabelFont( labelFont ), mCurLabelField( -1 )
{
  setupUi( this );
  fillHaliComboBox();
  fillValiComboBox();

  init( layerId, featureId, labelText );

  QSettings settings;
  restoreGeometry( settings.value( QString( "/Windows/ChangeLabelProps/geometry" ) ).toByteArray() );
}

QgsLabelPropertyDialog::~QgsLabelPropertyDialog()
{
  QSettings settings;
  settings.setValue( QString( "/Windows/ChangeLabelProps/geometry" ), saveGeometry() );

  qDeleteAll( mDataDefinedProperties );
}

void QgsLabelPropertyDialog::on_buttonBox_clicked( QAbstractButton *button )
{
  if ( buttonBox->buttonRole( button ) == QDialogButtonBox::ApplyRole )
  {
    emit applied();
  }
}

void QgsLabelPropertyDialog::init( const QString& layerId, int featureId, const QString& labelText )
{
  //get feature attributes
  QgsVectorLayer* vlayer = dynamic_cast<QgsVectorLayer*>( QgsMapLayerRegistry::instance()->mapLayer( layerId ) );
  if ( !vlayer )
  {
    return;
  }

  if ( !vlayer->getFeatures( QgsFeatureRequest().setFilterFid( featureId ).setFlags( QgsFeatureRequest::NoGeometry ) ).nextFeature( mCurLabelFeat ) )
  {
    return;
  }
  QgsAttributes attributeValues = mCurLabelFeat.attributes();

  //get layerproperties. Problem: only for pallabeling...

  blockElementSignals( true );

  QgsPalLayerSettings layerSettings = QgsPalLayerSettings::fromLayer( vlayer );

  //get label field and fill line edit
  if ( layerSettings.isExpression && !labelText.isNull() )
  {
    mLabelTextLineEdit->setText( labelText );
    mLabelTextLineEdit->setEnabled( false );
    mLabelTextLabel->setText( tr( "Expression result" ) );
  }
  else
  {
    QString labelFieldName = vlayer->customProperty( "labeling/fieldName" ).toString();
    if ( !labelFieldName.isEmpty() )
    {
      mCurLabelField = vlayer->fieldNameIndex( labelFieldName );
      if ( mCurLabelField >= 0 )
      {
        mLabelTextLineEdit->setText( attributeValues.at( mCurLabelField ).toString() );
        const QgsFields& layerFields = vlayer->fields();
        switch ( layerFields[mCurLabelField].type() )
        {
          case QVariant::Double:
            mLabelTextLineEdit->setValidator( new QDoubleValidator( this ) );
            break;
          case QVariant::Int:
          case QVariant::UInt:
          case QVariant::LongLong:
            mLabelTextLineEdit->setValidator( new QIntValidator( this ) );
            break;
          default:
            break;
        }
      }
      else
      {
        mLabelTextLineEdit->setEnabled( false );
      }
    }
  }

  //get attributes of the feature and fill data defined values

  // font is set directly from QgsLabelPosition
  updateFont( mLabelFont, false );

  //set all the gui elements to the default layer-level values
  mLabelDistanceSpinBox->clear();
  mLabelDistanceSpinBox->setSpecialValueText( tr( "Layer default (%1)" ).arg( QString::number( layerSettings.dist, 'f', mLabelDistanceSpinBox->decimals() ) ) );
  mBufferSizeSpinBox->clear();
  mBufferSizeSpinBox->setSpecialValueText( tr( "Layer default (%1)" ).arg( QString::number( layerSettings.bufferSize, 'f', mBufferSizeSpinBox->decimals() ) ) );
  mRotationSpinBox->clear();
  mXCoordSpinBox->clear();
  mYCoordSpinBox->clear();

  mShowLabelChkbx->setChecked( true );
  mFontColorButton->setColor( layerSettings.textColor );
  mBufferColorButton->setColor( layerSettings.bufferColor );
  mMinScaleSpinBox->setValue( layerSettings.scaleMin );
  mMaxScaleSpinBox->setValue( layerSettings.scaleMax );
  mHaliComboBox->setCurrentIndex( mHaliComboBox->findData( "Left" ) );
  mValiComboBox->setCurrentIndex( mValiComboBox->findData( "Bottom" ) );
  mFontColorButton->setColorDialogTitle( tr( "Font color" ) );
  mBufferColorButton->setColorDialogTitle( tr( "Buffer color" ) );

  disableGuiElements();

  QMap< QgsPalLayerSettings::DataDefinedProperties, QgsDataDefined* >::const_iterator it = layerSettings.dataDefinedProperties.constBegin();
  for ( ; it != layerSettings.dataDefinedProperties.constEnd(); ++it )
  {
    mDataDefinedProperties.insert( it.key(), it.value() ? new QgsDataDefined( *it.value() ) : nullptr );
  }

  //set widget values from data defined results
  setDataDefinedValues( layerSettings, vlayer );
  //enable widgets connected to data defined fields
  enableDataDefinedWidgets( vlayer );

  blockElementSignals( false );
}

void QgsLabelPropertyDialog::disableGuiElements()
{
  mShowLabelChkbx->setEnabled( false );
  mAlwaysShowChkbx->setEnabled( false );
  mMinScaleSpinBox->setEnabled( false );
  mMaxScaleSpinBox->setEnabled( false );
  mFontFamilyCmbBx->setEnabled( false );
  mFontStyleCmbBx->setEnabled( false );
  mFontUnderlineBtn->setEnabled( false );
  mFontStrikethroughBtn->setEnabled( false );
  mFontBoldBtn->setEnabled( false );
  mFontItalicBtn->setEnabled( false );
  mFontSizeSpinBox->setEnabled( false );
  mBufferSizeSpinBox->setEnabled( false );
  mFontColorButton->setEnabled( false );
  mBufferColorButton->setEnabled( false );
  mLabelDistanceSpinBox->setEnabled( false );
  mXCoordSpinBox->setEnabled( false );
  mYCoordSpinBox->setEnabled( false );
  mHaliComboBox->setEnabled( false );
  mValiComboBox->setEnabled( false );
  mRotationSpinBox->setEnabled( false );
}

void QgsLabelPropertyDialog::blockElementSignals( bool block )
{
  mShowLabelChkbx->blockSignals( block );
  mAlwaysShowChkbx->blockSignals( block );
  mMinScaleSpinBox->blockSignals( block );
  mMaxScaleSpinBox->blockSignals( block );
  mFontFamilyCmbBx->blockSignals( block );
  mFontStyleCmbBx->blockSignals( block );
  mFontUnderlineBtn->blockSignals( block );
  mFontStrikethroughBtn->blockSignals( block );
  mFontBoldBtn->blockSignals( block );
  mFontItalicBtn->blockSignals( block );
  mFontSizeSpinBox->blockSignals( block );
  mBufferSizeSpinBox->blockSignals( block );
  mFontColorButton->blockSignals( block );
  mBufferColorButton->blockSignals( block );
  mLabelDistanceSpinBox->blockSignals( block );
  mXCoordSpinBox->blockSignals( block );
  mYCoordSpinBox->blockSignals( block );
  mHaliComboBox->blockSignals( block );
  mValiComboBox->blockSignals( block );
  mRotationSpinBox->blockSignals( block );
}

void QgsLabelPropertyDialog::setDataDefinedValues( const QgsPalLayerSettings &layerSettings, QgsVectorLayer* vlayer )
{
  //loop through data defined properties and set all the GUI widget values. We can do this
  //even if the data defined property is set to an expression, as it's useful to show
  //users what the evaluated property is...

  QgsExpressionContext context;
  context << QgsExpressionContextUtils::globalScope()
  << QgsExpressionContextUtils::projectScope()
  << QgsExpressionContextUtils::atlasScope( nullptr )
  << QgsExpressionContextUtils::mapSettingsScope( MBGuiApp::instance()->mapCanvas()->mapSettings() )
  << QgsExpressionContextUtils::layerScope( vlayer );
  context.setFeature( mCurLabelFeat );

  QMap< QgsPalLayerSettings::DataDefinedProperties, QgsDataDefined* >::const_iterator propIt = mDataDefinedProperties.constBegin();
  for ( ; propIt != mDataDefinedProperties.constEnd(); ++propIt )
  {
    QgsDataDefined* dd = propIt.value();
    if ( !dd || !dd->isActive() )
    {
      continue;
    }

    if ( !dd->expressionIsPrepared() )
    {
      dd->prepareExpression( context );
    }

    //TODO - pass expression context
    QVariant result = layerSettings.dataDefinedValue( propIt.key(), mCurLabelFeat, vlayer->fields(), &context );
    if ( !result.isValid() || result.isNull() )
    {
      //could not evaluate data defined value
      continue;
    }

    bool ok = false;
    switch ( propIt.key() )
    {
      case QgsPalLayerSettings::Show:
      {
        int showLabel = result.toInt( &ok );
        mShowLabelChkbx->setChecked( !ok || showLabel != 0 );
        break;
      }
      case QgsPalLayerSettings::AlwaysShow:
        mAlwaysShowChkbx->setChecked( result.toBool() );
        break;
      case QgsPalLayerSettings::MinScale:
      {
        int minScale = result.toInt( &ok );
        if ( ok )
        {
          mMinScaleSpinBox->setValue( minScale );
        }
        break;
      }
      case QgsPalLayerSettings::MaxScale:
      {
        int maxScale = result.toInt( &ok );
        if ( ok )
        {
          mMaxScaleSpinBox->setValue( maxScale );
        }
        break;
      }
      case QgsPalLayerSettings::BufferSize:
      {
        double bufferSize = result.toDouble( &ok );
        if ( ok )
        {
          mBufferSizeSpinBox->setValue( bufferSize );
        }
        break;
      }
      case QgsPalLayerSettings::PositionX:
      {
        double posX = result.toDouble( &ok );
        if ( ok )
        {
          mXCoordSpinBox->setValue( posX );
        }
        break;
      }
      case QgsPalLayerSettings::PositionY:
      {
        double posY = result.toDouble( &ok );
        if ( ok )
        {
          mYCoordSpinBox->setValue( posY );
        }
        break;
      }
      case QgsPalLayerSettings::LabelDistance:
      {
        double labelDist = result.toDouble( &ok );
        if ( ok )
        {
          mLabelDistanceSpinBox->setValue( labelDist );
        }
        break;
      }
      case QgsPalLayerSettings::Hali:
        mHaliComboBox->setCurrentIndex( mHaliComboBox->findData( result.toString() ) );
        break;
      case QgsPalLayerSettings::Vali:
        mValiComboBox->setCurrentIndex( mValiComboBox->findData( result.toString() ) );
        break;
      case QgsPalLayerSettings::BufferColor:
        mBufferColorButton->setColor( QColor( result.toString() ) );
        break;
      case QgsPalLayerSettings::Color:
        mFontColorButton->setColor( QColor( result.toString() ) );
        break;
      case QgsPalLayerSettings::Rotation:
      {
        double rot = result.toDouble( &ok );
        if ( ok )
        {
          mRotationSpinBox->setValue( rot );
        }
        break;
      }

      case QgsPalLayerSettings::Size:
      {
        double size = result.toDouble( &ok );
        if ( ok )
        {
          mFontSizeSpinBox->setValue( size );
        }
        else
        {
          mFontSizeSpinBox->setValue( 0 );
        }
        break;
      }
      default:
        break;
    }
  }
}

void QgsLabelPropertyDialog::enableDataDefinedWidgets( QgsVectorLayer* vlayer )
{
  //loop through data defined properties, this time setting whether or not the widgets are enabled
  //this can only be done for properties which are assigned to fields
  QMap< QgsPalLayerSettings::DataDefinedProperties, QgsDataDefined* >::const_iterator propIt = mDataDefinedProperties.constBegin();
  for ( ; propIt != mDataDefinedProperties.constEnd(); ++propIt )
  {
    QgsDataDefined* dd = propIt.value();
    if ( !dd )
    {
      continue;
    }
    QString ddField = dd->field();
    if ( !dd->isActive() || dd->useExpression() || ddField.isEmpty() )
    {
      continue; // can only modify attributes with an active data definition of a mapped field
    }

    int ddIndx = vlayer->fieldNameIndex( ddField );
    if ( ddIndx == -1 )
    {
      continue;
    }

    QgsDebugMsg( QString( "ddField: %1" ).arg( ddField ) );

    switch ( propIt.key() )
    {
      case QgsPalLayerSettings::Show:
        mShowLabelChkbx->setEnabled( true );
        break;
      case QgsPalLayerSettings::AlwaysShow:
        mAlwaysShowChkbx->setEnabled( true );
        break;
      case QgsPalLayerSettings::MinScale:
        mMinScaleSpinBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::MaxScale:
        mMaxScaleSpinBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::BufferSize:
        mBufferSizeSpinBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::PositionX:
        mXCoordSpinBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::PositionY:
        mYCoordSpinBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::LabelDistance:
        mLabelDistanceSpinBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::Hali:
        mHaliComboBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::Vali:
        mValiComboBox->setEnabled( true );
        break;
      case QgsPalLayerSettings::BufferColor:
        mBufferColorButton->setEnabled( true );
        break;
      case QgsPalLayerSettings::Color:
        mFontColorButton->setEnabled( true );
        break;
      case QgsPalLayerSettings::Rotation:
        mRotationSpinBox->setEnabled( true );
        break;
        //font related properties
      case QgsPalLayerSettings::Family:
        mFontFamilyCmbBx->setEnabled( true );
        break;
      case QgsPalLayerSettings::FontStyle:
        mFontStyleCmbBx->setEnabled( true );
        break;
      case QgsPalLayerSettings::Underline:
        mFontUnderlineBtn->setEnabled( true );
        break;
      case QgsPalLayerSettings::Strikeout:
        mFontStrikethroughBtn->setEnabled( true );
        break;
      case QgsPalLayerSettings::Bold:
        mFontBoldBtn->setEnabled( true );
        break;
      case QgsPalLayerSettings::Italic:
        mFontItalicBtn->setEnabled( true );
        break;
      case QgsPalLayerSettings::Size:
        mFontSizeSpinBox->setEnabled( true );
        break;
      default:
        break;
    }
  }
}

void QgsLabelPropertyDialog::updateFont( const QFont& font, bool block )
{
  // update background reference font
  if ( font != mLabelFont )
  {
    mLabelFont = font;
  }

  if ( block )
    blockElementSignals( true );

  mFontFamilyCmbBx->setCurrentFont( mLabelFont );
  populateFontStyleComboBox();
  mFontUnderlineBtn->setChecked( mLabelFont.underline() );
  mFontStrikethroughBtn->setChecked( mLabelFont.strikeOut() );
  mFontBoldBtn->setChecked( mLabelFont.bold() );
  mFontItalicBtn->setChecked( mLabelFont.italic() );
  if ( block )
    blockElementSignals( false );
}

void QgsLabelPropertyDialog::populateFontStyleComboBox()
{
  mFontStyleCmbBx->clear();
  Q_FOREACH ( const QString &style, mFontDB.styles( mLabelFont.family() ) )
  {
    mFontStyleCmbBx->addItem( style );
  }

  int curIndx = 0;
  int stylIndx = mFontStyleCmbBx->findText( mFontDB.styleString( mLabelFont ) );
  if ( stylIndx > -1 )
  {
    curIndx = stylIndx;
  }

  mFontStyleCmbBx->setCurrentIndex( curIndx );
}

void QgsLabelPropertyDialog::fillHaliComboBox()
{
  mHaliComboBox->addItem( tr( "Left" ), "Left" );
  mHaliComboBox->addItem( tr( "Center" ), "Center" );
  mHaliComboBox->addItem( tr( "Right" ), "Right" );
}

void QgsLabelPropertyDialog::fillValiComboBox()
{
  mValiComboBox->addItem( tr( "Bottom" ), "Bottom" );
  mValiComboBox->addItem( tr( "Base" ), "Base" );
  mValiComboBox->addItem( tr( "Half" ), "Half" );
  mValiComboBox->addItem( tr( "Cap" ), "Cap" );
  mValiComboBox->addItem( tr( "Top" ), "Top" );
}

void QgsLabelPropertyDialog::on_mShowLabelChkbx_toggled( bool chkd )
{
  insertChangedValue( QgsPalLayerSettings::Show, ( chkd ? 1 : 0 ) );
}

void QgsLabelPropertyDialog::on_mAlwaysShowChkbx_toggled( bool chkd )
{
  insertChangedValue( QgsPalLayerSettings::AlwaysShow, ( chkd ? 1 : 0 ) );
}

void QgsLabelPropertyDialog::on_mMinScaleSpinBox_valueChanged( int i )
{
  insertChangedValue( QgsPalLayerSettings::MinScale, i );
}

void QgsLabelPropertyDialog::on_mMaxScaleSpinBox_valueChanged( int i )
{
  insertChangedValue( QgsPalLayerSettings::MaxScale, i );
}

void QgsLabelPropertyDialog::on_mLabelDistanceSpinBox_valueChanged( double d )
{
  QVariant distance( d );
  if ( d < 0 )
  {
    //null value so that distance is reset to default
    distance.clear();
  }
  insertChangedValue( QgsPalLayerSettings::LabelDistance, distance );
}

void QgsLabelPropertyDialog::on_mXCoordSpinBox_valueChanged( double d )
{
  QVariant x( d );
  if ( d < mXCoordSpinBox->minimum() + mXCoordSpinBox->singleStep() )
  {
    //null value
    x.clear();
  }
  insertChangedValue( QgsPalLayerSettings::PositionX, x );
}

void QgsLabelPropertyDialog::on_mYCoordSpinBox_valueChanged( double d )
{
  QVariant y( d );
  if ( d < mYCoordSpinBox->minimum() + mYCoordSpinBox->singleStep() )
  {
    //null value
    y.clear();
  }
  insertChangedValue( QgsPalLayerSettings::PositionY, y );
}

void QgsLabelPropertyDialog::on_mFontFamilyCmbBx_currentFontChanged( const QFont& f )
{
  mLabelFont.setFamily( f.family() );
  updateFont( mLabelFont );
  insertChangedValue( QgsPalLayerSettings::Family, f.family() );
}

void QgsLabelPropertyDialog::on_mFontStyleCmbBx_currentIndexChanged( const QString & text )
{
  QgsFontUtils::updateFontViaStyle( mLabelFont, text );
  updateFont( mLabelFont );
  insertChangedValue( QgsPalLayerSettings::FontStyle, text );
}

void QgsLabelPropertyDialog::on_mFontUnderlineBtn_toggled( bool ckd )
{
  mLabelFont.setUnderline( ckd );
  updateFont( mLabelFont );
  insertChangedValue( QgsPalLayerSettings::Underline, ckd );
}

void QgsLabelPropertyDialog::on_mFontStrikethroughBtn_toggled( bool ckd )
{
  mLabelFont.setStrikeOut( ckd );
  updateFont( mLabelFont );
  insertChangedValue( QgsPalLayerSettings::Strikeout, ckd );
}

void QgsLabelPropertyDialog::on_mFontBoldBtn_toggled( bool ckd )
{
  mLabelFont.setBold( ckd );
  updateFont( mLabelFont );
  insertChangedValue( QgsPalLayerSettings::Bold, ckd );
}

void QgsLabelPropertyDialog::on_mFontItalicBtn_toggled( bool ckd )
{
  mLabelFont.setItalic( ckd );
  updateFont( mLabelFont );
  insertChangedValue( QgsPalLayerSettings::Italic, ckd );
}

void QgsLabelPropertyDialog::on_mFontSizeSpinBox_valueChanged( double d )
{
  QVariant size( d );
  if ( d <= 0 )
  {
    //null value so that font size is reset to default
    size.clear();
  }
  insertChangedValue( QgsPalLayerSettings::Size, size );
}

void QgsLabelPropertyDialog::on_mBufferSizeSpinBox_valueChanged( double d )
{
  QVariant size( d );
  if ( d < 0 )
  {
    //null value so that size is reset to default
    size.clear();
  }
  insertChangedValue( QgsPalLayerSettings::BufferSize, size );
}

void QgsLabelPropertyDialog::on_mRotationSpinBox_valueChanged( double d )
{
  QVariant rotation( d );
  if ( d < 0 )
  {
    //null value so that size is reset to default
    rotation.clear();
  }
  insertChangedValue( QgsPalLayerSettings::Rotation, rotation );
}

void QgsLabelPropertyDialog::on_mFontColorButton_colorChanged( const QColor &color )
{
  insertChangedValue( QgsPalLayerSettings::Color, color.name() );
}

void QgsLabelPropertyDialog::on_mBufferColorButton_colorChanged( const QColor &color )
{
  insertChangedValue( QgsPalLayerSettings::BufferColor, color.name() );
}

void QgsLabelPropertyDialog::on_mHaliComboBox_currentIndexChanged( const int index )
{
  insertChangedValue( QgsPalLayerSettings::Hali, mHaliComboBox->itemData( index ) );
}

void QgsLabelPropertyDialog::on_mValiComboBox_currentIndexChanged( const int index )
{
  insertChangedValue( QgsPalLayerSettings::Vali, mValiComboBox->itemData( index ) );
}

void QgsLabelPropertyDialog::on_mLabelTextLineEdit_textChanged( const QString& text )
{
  if ( mCurLabelField != -1 )
  {
    mChangedProperties.insert( mCurLabelField, text );
  }
}

void QgsLabelPropertyDialog::insertChangedValue( QgsPalLayerSettings::DataDefinedProperties p, const QVariant& value )
{
  QMap< QgsPalLayerSettings::DataDefinedProperties, QgsDataDefined* >::const_iterator ddIt = mDataDefinedProperties.constFind( p );
  if ( ddIt != mDataDefinedProperties.constEnd() )
  {
    QgsDataDefined* dd = ddIt.value();

    if ( dd && dd->isActive() && !dd->useExpression() && !dd->field().isEmpty() )
    {
      mChangedProperties.insert( mCurLabelFeat.fieldNameIndex( dd->field() ), value );
    }
  }
}
