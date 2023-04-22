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

// MergeNodes Logic includes
#include <vtkSlicerMergeNodesLogic.h>

// MergeNodes includes
#include "qSlicerMergeNodesModule.h"
#include "qSlicerMergeNodesModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerMergeNodesModulePrivate
{
public:
  qSlicerMergeNodesModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerMergeNodesModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerMergeNodesModulePrivate::qSlicerMergeNodesModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerMergeNodesModule methods

//-----------------------------------------------------------------------------
qSlicerMergeNodesModule::qSlicerMergeNodesModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerMergeNodesModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerMergeNodesModule::~qSlicerMergeNodesModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerMergeNodesModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerMergeNodesModule::acknowledgementText() const
{
  return "";
}

//-----------------------------------------------------------------------------
QStringList qSlicerMergeNodesModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Kerim Khemraev (Tierra Colada Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerMergeNodesModule::icon() const
{
  return QIcon(":/Icons/MergeNodes.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerMergeNodesModule::categories() const
{
  return QStringList() << "Interpolation";
}

//-----------------------------------------------------------------------------
QStringList qSlicerMergeNodesModule::dependencies() const
{
  return QStringList() << "Markups" << "Models" << "Volumes";
}

//-----------------------------------------------------------------------------
void qSlicerMergeNodesModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerMergeNodesModule
::createWidgetRepresentation()
{
  return new qSlicerMergeNodesModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerMergeNodesModule::createLogic()
{
  return vtkSlicerMergeNodesLogic::New();
}
