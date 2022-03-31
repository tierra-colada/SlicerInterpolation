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
#include "qSlicerInterpolationModuleWidget.h"
#include "ui_qSlicerInterpolationModuleWidget.h"
#include "vtkSlicerInterpolationLogic.h"
#include "qSlicerApplication.h"

// VTK includes
#include <vtkPointInterpolator.h>
#include <vtkGeneralizedKernel.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>

// MRML includes
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLMarkupsNode.h>

// MRML includes
#include <vtkMRMLDisplayableNode.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerInterpolationModuleWidgetPrivate: public Ui_qSlicerInterpolationModuleWidget
{
public:
  qSlicerInterpolationModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerInterpolationModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerInterpolationModuleWidgetPrivate::qSlicerInterpolationModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerInterpolationModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerInterpolationModuleWidget::qSlicerInterpolationModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerInterpolationModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerInterpolationModuleWidget::~qSlicerInterpolationModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerInterpolationModuleWidget::setup()
{
  Q_D(qSlicerInterpolationModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  // hide unneded widgets
  this->onNullPointStrategyComboBoxTextChanged(d->nullPointStrategyComboBox->currentText());
  this->onKernelComboBoxTextChanged(d->kernelComboBox->currentText());
  this->onKernelFootPrintComboBoxTextChanged(d->kernelFootPrintComboBox->currentText());

  connect(d->nullPointStrategyComboBox, &QComboBox::currentTextChanged,
          this, &qSlicerInterpolationModuleWidget::onNullPointStrategyComboBoxTextChanged);
  connect(d->kernelComboBox, &QComboBox::currentTextChanged,
          this, &qSlicerInterpolationModuleWidget::onKernelComboBoxTextChanged);
  connect(d->kernelFootPrintComboBox, &QComboBox::currentTextChanged,
          this, &qSlicerInterpolationModuleWidget::onKernelFootPrintComboBoxTextChanged);

  connect(d->applyBtn, &QPushButton::clicked,
          this, &qSlicerInterpolationModuleWidget::onApplyButtonClicked);
}

//-----------------------------------------------------------------------------
void qSlicerInterpolationModuleWidget::onNullPointStrategyComboBoxTextChanged(const QString& text)
{
  Q_D(qSlicerInterpolationModuleWidget);
  if (text == "Null Value"){
    d->nullPointLabel->setHidden(false);
    d->nullPointDoubleSpinBox->setHidden(false);
  } else {
    d->nullPointLabel->setHidden(true);
    d->nullPointDoubleSpinBox->setHidden(true);
  }
}

//-----------------------------------------------------------------------------
void qSlicerInterpolationModuleWidget::onKernelComboBoxTextChanged(const QString& text)
{
  Q_D(qSlicerInterpolationModuleWidget);
  if (text == "Linear"){
    d->sharpnessLabel->setHidden(true);
    d->sharpnessDoubleSpinBox->setHidden(true);
    d->eccentricityLabel->setHidden(true);
    d->eccentricityDoubleSpinBox->setHidden(true);
    d->powerLabel->setHidden(true);
    d->powerDoubleSpinBox->setHidden(true);
  } else if (text == "Gaussian") {
    d->sharpnessLabel->setHidden(false);
    d->sharpnessDoubleSpinBox->setHidden(false);
    d->eccentricityLabel->setHidden(true);
    d->eccentricityDoubleSpinBox->setHidden(true);
    d->powerLabel->setHidden(true);
    d->powerDoubleSpinBox->setHidden(true);
  } else if (text == "Ellipsoidal Gaussian") {
    d->sharpnessLabel->setHidden(false);
    d->sharpnessDoubleSpinBox->setHidden(false);
    d->eccentricityLabel->setHidden(false);
    d->eccentricityDoubleSpinBox->setHidden(false);
    d->powerLabel->setHidden(true);
    d->powerDoubleSpinBox->setHidden(true);
  } else if (text == "Probabilistic Voronoi") {
    d->sharpnessLabel->setHidden(true);
    d->sharpnessDoubleSpinBox->setHidden(true);
    d->eccentricityLabel->setHidden(true);
    d->eccentricityDoubleSpinBox->setHidden(true);
    d->powerLabel->setHidden(true);
    d->powerDoubleSpinBox->setHidden(true);
  } else if (text == "Shepard") {
    d->sharpnessLabel->setHidden(true);
    d->sharpnessDoubleSpinBox->setHidden(true);
    d->eccentricityLabel->setHidden(true);
    d->eccentricityDoubleSpinBox->setHidden(true);
    d->powerLabel->setHidden(false);
    d->powerDoubleSpinBox->setHidden(false);
  }
}

//-----------------------------------------------------------------------------
void qSlicerInterpolationModuleWidget::onKernelFootPrintComboBoxTextChanged(const QString& text)
{
  Q_D(qSlicerInterpolationModuleWidget);
  if (text == "N Closest"){
    d->radiusLabel->setHidden(true);
    d->radiusDoubleSpinBox->setHidden(true);
    d->nClosestLabel->setHidden(false);
    d->nClosestSpinBox->setHidden(false);
  } else if (text == "Radius"){
    d->radiusLabel->setHidden(false);
    d->radiusDoubleSpinBox->setHidden(false);
    d->nClosestLabel->setHidden(true);
    d->nClosestSpinBox->setHidden(true);
  }
}

//-----------------------------------------------------------------------------
void qSlicerInterpolationModuleWidget::onApplyButtonClicked()
{
  Q_D(qSlicerInterpolationModuleWidget);
  vtkSlicerInterpolationLogic* interpolationLogic =
      vtkSlicerInterpolationLogic::SafeDownCast(logic());
  if (!interpolationLogic){
    qCritical() << Q_FUNC_INFO << "Unable to get Interpolation logic";
    return;
  }

  vtkMRMLDisplayableNode* inputNode = vtkMRMLDisplayableNode::SafeDownCast(
        d->inputNodeSelector->currentNode());
  if (!inputNode){
    qCritical() << Q_FUNC_INFO << "Unable to cast Input node to vtkMRMLDisplayableNode";
    return;
  }

  vtkMRMLDisplayableNode* outNode = vtkMRMLDisplayableNode::SafeDownCast(
        d->outputNodeSelector->currentNode());
  if (!outNode){
    qCritical() << Q_FUNC_INFO << "Unable to cast Output node to vtkMRMLDisplayableNode";
    return;
  }

  double nullValue = d->nullPointDoubleSpinBox->value();
  int nullPointStrategy = 0;
  if (d->nullPointStrategyComboBox->currentText() == "Null Value"){
    nullPointStrategy = vtkPointInterpolator::Strategy::NULL_VALUE;
  } else if (d->nullPointStrategyComboBox->currentText() == "Closest Point"){
    nullPointStrategy = vtkPointInterpolator::Strategy::CLOSEST_POINT;
  } else if (d->nullPointStrategyComboBox->currentText() == "Mask Points"){
    nullPointStrategy = vtkPointInterpolator::Strategy::NULL_VALUE;
  }

  int kernel = 0;
  if (d->kernelComboBox->currentText() == "Linear"){
    kernel = vtkSlicerInterpolationLogic::InterpolationKernel::Linear;
  } else if (d->kernelComboBox->currentText() == "Gaussian"){
    kernel = vtkSlicerInterpolationLogic::InterpolationKernel::Gaussian;
  } else if (d->kernelComboBox->currentText() == "Ellipsoidal Gaussian"){
    kernel = vtkSlicerInterpolationLogic::InterpolationKernel::EllipsoidalGaussian;
  } else if (d->kernelComboBox->currentText() == "Probabilistic Voronoi"){
    kernel = vtkSlicerInterpolationLogic::InterpolationKernel::ProbabilisticVoronoi;
  } else if (d->kernelComboBox->currentText() == "Shepard"){
    kernel = vtkSlicerInterpolationLogic::InterpolationKernel::Shepard;
  }

  int kernelFootPrint = 0;
  if (d->kernelFootPrintComboBox->currentText() == "N Closest"){
    kernelFootPrint = vtkGeneralizedKernel::KernelStyle::N_CLOSEST;
  } else if (d->kernelFootPrintComboBox->currentText() == "Radius"){
    kernelFootPrint = vtkGeneralizedKernel::KernelStyle::RADIUS;
  }

  double radius = d->radiusDoubleSpinBox->value();
  int nClosestPoints = d->nClosestSpinBox->value();
  double sharpness = d->sharpnessDoubleSpinBox->value();
  double eccentricity = d->eccentricityDoubleSpinBox->value();
  double power = d->powerDoubleSpinBox->value();

  qSlicerApplication::setOverrideCursor(Qt::BusyCursor);
  interpolationLogic->Interpolate(
        inputNode, outNode,
        kernel, nullValue, nullPointStrategy,
        kernelFootPrint, nClosestPoints, radius,
        sharpness, eccentricity, power);
  qSlicerApplication::restoreOverrideCursor();
}
