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

#ifndef __qSlicerMergeNodesFooBarWidget_h
#define __qSlicerMergeNodesFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qSlicerMergeNodesModuleWidgetsExport.h"

class qSlicerMergeNodesFooBarWidgetPrivate;

/// \ingroup Slicer_QtModules_MergeNodes
class Q_SLICER_MODULE_MERGENODES_WIDGETS_EXPORT qSlicerMergeNodesFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qSlicerMergeNodesFooBarWidget(QWidget *parent=0);
  ~qSlicerMergeNodesFooBarWidget() override;

protected slots:

protected:
  QScopedPointer<qSlicerMergeNodesFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerMergeNodesFooBarWidget);
  Q_DISABLE_COPY(qSlicerMergeNodesFooBarWidget);
};

#endif
