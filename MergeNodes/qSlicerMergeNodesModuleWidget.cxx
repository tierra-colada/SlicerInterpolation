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

// MRML includes
#include <vtkMRMLScene.h>

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

  connect(this, &qSlicerWidget::mrmlSceneChanged,
          d->AddSelectorWidget, &qSlicerWidget::setMRMLScene);

  connect(d->pdataRadioBtn, &QRadioButton::toggled,
          this, &qSlicerMergeNodesModuleWidget::onRadioButtonToggled);
  connect(d->ugridRadioBtn, &QRadioButton::toggled,
          this, &qSlicerMergeNodesModuleWidget::onRadioButtonToggled);
  connect(d->imageRadioBtn, &QRadioButton::toggled,
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

  QStringList nodeTypes;
  if (radio == d->pdataRadioBtn){
    nodeTypes = QStringList({"vtkMRMLModelNode", "vtkMRMLMarkupsNode"});
  } else if (radio == d->ugridRadioBtn){
    nodeTypes = QStringList({"vtkMRMLModelNode"});
  } else if (radio == d->imageRadioBtn){
    nodeTypes = QStringList({"vtkMRMLScalarVolumeNode"});
  } else {
    return;
  }
  d->AddSelectorWidget->setNodeTypes(nodeTypes);
  d->outNodeSelector->setNodeTypes(nodeTypes);
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesModuleWidget::onApplyBtnClicked()
{

}
