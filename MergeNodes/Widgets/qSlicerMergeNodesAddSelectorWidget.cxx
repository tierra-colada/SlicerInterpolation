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

// AddSelector Widgets includes
#include "qSlicerMergeNodesAddSelectorWidget.h"
#include "ui_qSlicerMergeNodesAddSelectorWidget.h"

// MRML includes
#include <vtkMRMLScene.h>

// qMRML includes
#include "qMRMLNodeComboBox.h"

// Qt includes
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_MergeNodes
class qSlicerMergeNodesAddSelectorWidgetPrivate
  : public Ui_qSlicerMergeNodesAddSelectorWidget
{
  Q_DECLARE_PUBLIC(qSlicerMergeNodesAddSelectorWidget);
protected:
  qSlicerMergeNodesAddSelectorWidget* const q_ptr;

public:
  qSlicerMergeNodesAddSelectorWidgetPrivate(
    qSlicerMergeNodesAddSelectorWidget& object);
  virtual void setupUi(qSlicerMergeNodesAddSelectorWidget*);

  QVBoxLayout* mainVLayout;
  QStringList nodeTypes;
  bool showChildNodeTypes = true;
  bool showHidden = false;
  bool noneEnabled = false;
  bool addEnabled = true;
  bool removeEnabled = true;
  bool editEnabled = false;
  bool renameEnabled = false;
};

// --------------------------------------------------------------------------
qSlicerMergeNodesAddSelectorWidgetPrivate
::qSlicerMergeNodesAddSelectorWidgetPrivate(
  qSlicerMergeNodesAddSelectorWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidgetPrivate
::setupUi(qSlicerMergeNodesAddSelectorWidget* widget)
{
  this->Ui_qSlicerMergeNodesAddSelectorWidget::setupUi(widget);

  this->mainVLayout = qobject_cast<QVBoxLayout*>(widget->layout());
  if (!this->mainVLayout){
    qCritical() << Q_FUNC_INFO << "Unable to get main Vertical Layout";
  }

  this->addSelectorBtn->setIcon(QIcon(":/Icons/Add.png"));
  this->removeSelectorBtn->setIcon(QIcon(":/Icons/Remove.png"));

  // init first input
  widget->onAddSelectorBtnClicked();

  QObject::connect(addSelectorBtn, &QToolButton::clicked,
                   widget, &qSlicerMergeNodesAddSelectorWidget::onAddSelectorBtnClicked);
  QObject::connect(removeSelectorBtn, &QToolButton::clicked,
                   widget, &qSlicerMergeNodesAddSelectorWidget::onRemoveSelectorBtnClicked);

  QObject::connect(widget, &qSlicerWidget::mrmlSceneChanged,
                   widget, &qSlicerMergeNodesAddSelectorWidget::onMRMLSceneChanged);
}

//-----------------------------------------------------------------------------
// qSlicerMergeNodesAddSelectorWidget methods

//-----------------------------------------------------------------------------
qSlicerMergeNodesAddSelectorWidget
::qSlicerMergeNodesAddSelectorWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerMergeNodesAddSelectorWidgetPrivate(*this) )
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerMergeNodesAddSelectorWidget
::~qSlicerMergeNodesAddSelectorWidget()
{
}

//-----------------------------------------------------------------------------
QVector<vtkMRMLNode*> qSlicerMergeNodesAddSelectorWidget::getSelectedNodes()
{
  QVector<vtkMRMLNode*> nodes;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    nodes.append(combo->currentNode());

  return nodes;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setNodeTypes(const QStringList& nodeTypes)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->nodeTypes = nodeTypes;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setNodeTypes(nodeTypes);
}

//-----------------------------------------------------------------------------
QStringList qSlicerMergeNodesAddSelectorWidget::getNodeTypes()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->nodeTypes;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setShowChildNodeType(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->showChildNodeTypes = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setShowChildNodeTypes(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getShowChildNodeType()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->showChildNodeTypes;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setShowHidden(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->showHidden = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setShowHidden(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getShowHidden()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->showHidden;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setNoneEnabled(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->noneEnabled = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setNoneEnabled(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getNoneEnabled()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->noneEnabled;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setAddEnabled(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->addEnabled = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setAddEnabled(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getAddEnabled()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->addEnabled;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setRemoveEnabled(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->removeEnabled = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setRemoveEnabled(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getRemoveEnabled()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->removeEnabled;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setEditEnabled(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->editEnabled = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setEditEnabled(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getEditEnabled()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->editEnabled;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::setRenameEnabled(bool val)
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  d->renameEnabled = val;
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setRenameEnabled(val);
}

//-----------------------------------------------------------------------------
bool qSlicerMergeNodesAddSelectorWidget::getRenameEnabled()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  return d->renameEnabled;
}


//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::onAddSelectorBtnClicked()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  QWidget* widget = createWidgetPair();
  if (!widget)
    return;

  d->mainVLayout->insertWidget(d->mainVLayout->count()-1, widget);
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::onRemoveSelectorBtnClicked()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  if (!d->mainVLayout){
    qCritical() << Q_FUNC_INFO << "Unable to get main Vertical Layout";
    return;
  }

  if (d->mainVLayout->count() == 2)
    return;

  auto item = d->mainVLayout->itemAt(d->mainVLayout->count()-2);
  if (!item)
    return;

  delete item->widget();
}

//-----------------------------------------------------------------------------
QWidget* qSlicerMergeNodesAddSelectorWidget::createWidgetPair()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  if (!d->mainVLayout){
    qCritical() << Q_FUNC_INFO << "Unable to get main Vertical Layout";
    return nullptr;
  }

  QWidget* widget = new QWidget;
  QLabel* label = new QLabel;
  label->setObjectName("Label");
  label->setText("Input " + QString::number(d->mainVLayout->count()) + ":");
  qMRMLNodeComboBox* combo = new qMRMLNodeComboBox;
  combo->setObjectName("NodeComboBox");
  combo->setNodeTypes(d->nodeTypes);
  combo->setShowChildNodeTypes(d->showChildNodeTypes);
  combo->setShowHidden(d->showHidden);
  combo->setNoneEnabled(d->noneEnabled);
  combo->setAddEnabled(d->addEnabled);
  combo->setRemoveEnabled(d->removeEnabled);
  combo->setEditEnabled(d->editEnabled);
  combo->setRenameEnabled(d->renameEnabled);
  combo->setMRMLScene(this->mrmlScene());

  QHBoxLayout* hLayout = new QHBoxLayout(widget);
  hLayout->setMargin(0);
  hLayout->addWidget(label);
  hLayout->addWidget(combo);

  return widget;
}

//-----------------------------------------------------------------------------
QVector<qMRMLNodeComboBox*> qSlicerMergeNodesAddSelectorWidget::getNodeComboBoxes()
{
  Q_D(qSlicerMergeNodesAddSelectorWidget);
  if (!d->mainVLayout){
    qCritical() << Q_FUNC_INFO << "Unable to get main Vertical Layout";
    return QVector<qMRMLNodeComboBox*>();
  }

  QVector<qMRMLNodeComboBox*> nodeCombos;
  for (int i = 0; i < d->mainVLayout->count(); i++){
    auto item = d->mainVLayout->itemAt(i);
    if (!item)
      continue;

    qMRMLNodeComboBox* combo =
        item->widget()->findChild<qMRMLNodeComboBox*>();
    if (!combo)
      continue;

    nodeCombos.append(combo);
  }
  return nodeCombos;
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesAddSelectorWidget::onMRMLSceneChanged(vtkMRMLScene* scene)
{
  QVector<qMRMLNodeComboBox*> combos = getNodeComboBoxes();
  for (qMRMLNodeComboBox* combo : combos)
    combo->setMRMLScene(scene);
}
