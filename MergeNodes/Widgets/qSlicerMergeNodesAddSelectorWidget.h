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

#ifndef __qSlicerMergeNodesAddSelectorWidget_h
#define __qSlicerMergeNodesAddSelectorWidget_h

// Qt includes
#include <qSlicerWidget.h>

// AddSelector Widgets includes
#include "qSlicerMergeNodesModuleWidgetsExport.h"

class qSlicerMergeNodesAddSelectorWidgetPrivate;
class vtkMRMLNode;
class qMRMLNodeComboBox;

/// \ingroup Slicer_QtModules_MergeNodes
class Q_SLICER_MODULE_MERGENODES_WIDGETS_EXPORT qSlicerMergeNodesAddSelectorWidget
  : public qSlicerWidget
{
  Q_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerMergeNodesAddSelectorWidget(QWidget *parent=0);
  ~qSlicerMergeNodesAddSelectorWidget() override;

public slots:
  QVector<vtkMRMLNode*> getSelectedNodes();
  QVector<qMRMLNodeComboBox*> getNodeComboBoxes();

  void setNodeTypes(const QStringList& nodeTypes);
  QStringList getNodeTypes();

  void setShowHidden(bool val);
  bool getShowHidden();

  void setNoneEnabled(bool val);
  bool getNoneEnabled();

  void setAddEnabled(bool val);
  bool getAddEnabled();

  void setRemoveEnabled(bool val);
  bool getRemoveEnabled();

  void setEditEnabled(bool val);
  bool getEditEnabled();

  void setRenameEnabled(bool val);
  bool getRenameEnabled();

  void setShowChildNodeType(bool val);
  bool getShowChildNodeType();

  void onAddSelectorBtnClicked();
  void onRemoveSelectorBtnClicked();
  void onMRMLSceneChanged(vtkMRMLScene* scene);

protected:
  QScopedPointer<qSlicerMergeNodesAddSelectorWidgetPrivate> d_ptr;

  QWidget* createWidgetPair();

private:
  Q_DECLARE_PRIVATE(qSlicerMergeNodesAddSelectorWidget);
  Q_DISABLE_COPY(qSlicerMergeNodesAddSelectorWidget);
};

#endif
