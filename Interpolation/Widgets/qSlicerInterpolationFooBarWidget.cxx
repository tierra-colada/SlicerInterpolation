/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerInterpolationFooBarWidget.h"
#include "ui_qSlicerInterpolationFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_Interpolation
class qSlicerInterpolationFooBarWidgetPrivate
  : public Ui_qSlicerInterpolationFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerInterpolationFooBarWidget);
protected:
  qSlicerInterpolationFooBarWidget* const q_ptr;

public:
  qSlicerInterpolationFooBarWidgetPrivate(
    qSlicerInterpolationFooBarWidget& object);
  virtual void setupUi(qSlicerInterpolationFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerInterpolationFooBarWidgetPrivate
::qSlicerInterpolationFooBarWidgetPrivate(
  qSlicerInterpolationFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerInterpolationFooBarWidgetPrivate
::setupUi(qSlicerInterpolationFooBarWidget* widget)
{
  this->Ui_qSlicerInterpolationFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerInterpolationFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerInterpolationFooBarWidget
::qSlicerInterpolationFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerInterpolationFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerInterpolationFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerInterpolationFooBarWidget
::~qSlicerInterpolationFooBarWidget()
{
}
