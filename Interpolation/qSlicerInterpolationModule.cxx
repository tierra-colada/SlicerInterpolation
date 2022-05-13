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

// Interpolation Logic includes
#include <vtkSlicerInterpolationLogic.h>

// Interpolation includes
#include "qSlicerInterpolationModule.h"
#include "qSlicerInterpolationModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerInterpolationModulePrivate
{
public:
  qSlicerInterpolationModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerInterpolationModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerInterpolationModulePrivate::qSlicerInterpolationModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerInterpolationModule methods

//-----------------------------------------------------------------------------
qSlicerInterpolationModule::qSlicerInterpolationModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerInterpolationModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerInterpolationModule::~qSlicerInterpolationModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerInterpolationModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerInterpolationModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerInterpolationModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerInterpolationModule::icon() const
{
  return QIcon(":/Icons/Interpolation.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerInterpolationModule::categories() const
{
  return QStringList() << "Interpolation";
}

//-----------------------------------------------------------------------------
QStringList qSlicerInterpolationModule::dependencies() const
{
  return QStringList() << "Markups" << "Models" << "Volumes";
}

//-----------------------------------------------------------------------------
void qSlicerInterpolationModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerInterpolationModule
::createWidgetRepresentation()
{
  return new qSlicerInterpolationModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerInterpolationModule::createLogic()
{
  return vtkSlicerInterpolationLogic::New();
}
