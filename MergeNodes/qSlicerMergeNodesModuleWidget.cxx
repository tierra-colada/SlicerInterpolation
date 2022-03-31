/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// Slicer includes
#include "qSlicerMergeNodesModuleWidget.h"
#include "ui_qSlicerMergeNodesModuleWidget.h"
#include "vtkSlicerMergeNodesLogic.h"
#include "qSlicerApplication.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLDisplayableNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLVolumeNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLMarkupsNode.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerMergeNodesModuleWidgetPrivate: public Ui_qSlicerMergeNodesModuleWidget
{
public:
  qSlicerMergeNodesModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerMergeNodesModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerMergeNodesModuleWidgetPrivate::qSlicerMergeNodesModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerMergeNodesModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerMergeNodesModuleWidget::qSlicerMergeNodesModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerMergeNodesModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerMergeNodesModuleWidget::~qSlicerMergeNodesModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesModuleWidget::setup()
{
  Q_D(qSlicerMergeNodesModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  d->AddSelectorWidget->setNodeTypes({"vtkMRMLModelNode", "vtkMRMLMarkupsNode"});
  d->AddSelectorWidget->setShowChildNodeType(true);
  d->AddSelectorWidget->setAddEnabled(false);
  d->AddSelectorWidget->setRemoveEnabled(false);
  d->AddSelectorWidget->setEditEnabled(false);
  d->AddSelectorWidget->setRenameEnabled(false);

  d->appendImageSettingsGroupBox->setHidden(true);
  d->appendAnySettingsGroupBox->setHidden(true);

  connect(this, &qSlicerWidget::mrmlSceneChanged,
          d->AddSelectorWidget, &qSlicerWidget::setMRMLScene);

  connect(d->pdataRadioBtn, &QRadioButton::toggled,
          this, &qSlicerMergeNodesModuleWidget::onRadioButtonToggled);
  connect(d->imageRadioBtn, &QRadioButton::toggled,
          this, &qSlicerMergeNodesModuleWidget::onRadioButtonToggled);
  connect(d->anyRadioBtn, &QRadioButton::toggled,
          this, &qSlicerMergeNodesModuleWidget::onRadioButtonToggled);

  connect(d->applyBtn, &QPushButton::clicked,
          this, &qSlicerMergeNodesModuleWidget::onApplyBtnClicked);
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesModuleWidget::onRadioButtonToggled(bool val)
{
  Q_D(qSlicerMergeNodesModuleWidget);
  QRadioButton* radio = qobject_cast<QRadioButton*>(sender());
  if (!radio)
    return;

  QStringList nodeTypesIn, nodeTypesOut;
  if (radio == d->pdataRadioBtn){
    nodeTypesIn = QStringList({"vtkMRMLModelNode", "vtkMRMLMarkupsNode"});
    nodeTypesOut = QStringList({"vtkMRMLModelNode"});
    d->appendImageSettingsGroupBox->setHidden(true);
    d->appendAnySettingsGroupBox->setHidden(true);
  } else if (radio == d->imageRadioBtn){
    nodeTypesIn = QStringList({"vtkMRMLVolumeNode"});
    // node selector is unable to create vtkMRMLVolumeNode
    nodeTypesOut = QStringList({"vtkMRMLScalarVolumeNode"});
    d->appendImageSettingsGroupBox->setHidden(false);
    d->appendAnySettingsGroupBox->setHidden(true);
  } else if (radio == d->anyRadioBtn){
    nodeTypesIn = QStringList({"vtkMRMLModelNode", "vtkMRMLMarkupsNode", "vtkMRMLVolumeNode"});
    nodeTypesOut = QStringList({"vtkMRMLModelNode"});
    d->appendImageSettingsGroupBox->setHidden(true);
    d->appendAnySettingsGroupBox->setHidden(false);
  } else {
    return;
  }
  d->AddSelectorWidget->setNodeTypes(nodeTypesIn);
  d->outNodeSelector->setNodeTypes(nodeTypesOut);
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesModuleWidget::onApplyBtnClicked()
{
  Q_D(qSlicerMergeNodesModuleWidget);
  vtkSlicerMergeNodesLogic* mergeNodesLogic =
      vtkSlicerMergeNodesLogic::SafeDownCast(logic());
  if (!mergeNodesLogic){
    qCritical() << Q_FUNC_INFO << "Unable to get MergeNodes logic";
    return;
  }

  vtkMRMLNode* outNode = d->outNodeSelector->currentNode();
  if (!outNode){
    qCritical() << Q_FUNC_INFO << "Output node is NULL";
    return;
  }

  int imageAppendAxis = d->appendAxisSpinBox->value()-1;
  bool mergeCoincidentalPoints = d->mergePointsCheckBox->isChecked();
  double mergeCoincidentalPointsTol = d->mergePointsTolDoubleSpinBox->value();

  std::vector<vtkMRMLNode*> nodes = d->AddSelectorWidget->getSelectedNodes();
  std::vector<vtkMRMLDisplayableNode*> dispNodes;
  for (vtkMRMLNode* node : nodes){
    if (vtkMRMLDisplayableNode::SafeDownCast(node))
      dispNodes.push_back(vtkMRMLDisplayableNode::SafeDownCast(node));
  }

  qSlicerApplication::setOverrideCursor(Qt::BusyCursor);
  if (d->pdataRadioBtn->isChecked()){
    mergeNodesLogic->AppendPolyData(dispNodes, vtkMRMLModelNode::SafeDownCast(outNode));
  } else if (d->imageRadioBtn->isChecked()){
    std::vector<vtkMRMLVolumeNode*> volumeNodes;
    for (vtkMRMLNode* node : nodes){
      if (vtkMRMLVolumeNode::SafeDownCast(node))
        volumeNodes.push_back(vtkMRMLVolumeNode::SafeDownCast(node));
    }
    mergeNodesLogic->AppendImageData(
          volumeNodes,
          vtkMRMLVolumeNode::SafeDownCast(outNode),
          imageAppendAxis);
  } else if (d->anyRadioBtn->isChecked()){
    mergeNodesLogic->AppendAny(
          dispNodes,
          vtkMRMLModelNode::SafeDownCast(outNode),
          mergeCoincidentalPoints,
          mergeCoincidentalPointsTol);
  }
  qSlicerApplication::restoreOverrideCursor();
}
