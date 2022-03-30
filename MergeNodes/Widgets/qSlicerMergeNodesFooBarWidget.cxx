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
#include "qSlicerMergeNodesFooBarWidget.h"
#include "ui_qSlicerMergeNodesFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_MergeNodes
class qSlicerMergeNodesFooBarWidgetPrivate
  : public Ui_qSlicerMergeNodesFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerMergeNodesFooBarWidget);
protected:
  qSlicerMergeNodesFooBarWidget* const q_ptr;

public:
  qSlicerMergeNodesFooBarWidgetPrivate(
    qSlicerMergeNodesFooBarWidget& object);
  virtual void setupUi(qSlicerMergeNodesFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerMergeNodesFooBarWidgetPrivate
::qSlicerMergeNodesFooBarWidgetPrivate(
  qSlicerMergeNodesFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerMergeNodesFooBarWidgetPrivate
::setupUi(qSlicerMergeNodesFooBarWidget* widget)
{
  this->Ui_qSlicerMergeNodesFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerMergeNodesFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerMergeNodesFooBarWidget
::qSlicerMergeNodesFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerMergeNodesFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerMergeNodesFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerMergeNodesFooBarWidget
::~qSlicerMergeNodesFooBarWidget()
{
}
