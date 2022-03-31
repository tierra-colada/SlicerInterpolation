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
#include "vtkSlicerInterpolationLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLDisplayableNode.h>
#include <vtkMRMLVolumeNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLMarkupsNode.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPointInterpolator.h>
#include <vtkLinearKernel.h>
#include <vtkGaussianKernel.h>
#include <vtkEllipsoidalGaussianKernel.h>
#include <vtkProbabilisticVoronoiKernel.h>
#include <vtkShepardKernel.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include<vtkMergePoints.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerInterpolationLogic);

//----------------------------------------------------------------------------
vtkSlicerInterpolationLogic::vtkSlicerInterpolationLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerInterpolationLogic::~vtkSlicerInterpolationLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::InterpolateLinear(
    vtkMRMLDisplayableNode* nodeIn,
    vtkMRMLDisplayableNode* nodeOut,
    double nullValue,
    int nullPointStrategy,
    int kernelFootprint,
    int nClosestPoints,
    double radius)
{
  this->Interpolate(nodeIn,
                    nodeOut,
                    InterpolationKernel::Linear,
                    nullValue,
                    nullPointStrategy,
                    kernelFootprint,
                    nClosestPoints, radius,
                    0, 0, 0);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::InterpolateGaussian(
    vtkMRMLDisplayableNode* nodeIn,
    vtkMRMLDisplayableNode* nodeOut,
    double nullValue,
    int nullPointStrategy,
    int kernelFootprint,
    int nClosestPoints,
    double radius,
    double sharpness)
{
  this->Interpolate(nodeIn,
                    nodeOut,
                    InterpolationKernel::Linear,
                    nullValue,
                    nullPointStrategy,
                    kernelFootprint,
                    nClosestPoints, radius,
                    sharpness, 0, 0);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::InterpolateEllipsoidalGaussian(
    vtkMRMLDisplayableNode* nodeIn,
    vtkMRMLDisplayableNode* nodeOut,
    double nullValue,
    int nullPointStrategy,
    int kernelFootprint,
    int nClosestPoints,
    double radius,
    double sharpness,
    double eccentricity)
{
  this->Interpolate(nodeIn,
                    nodeOut,
                    InterpolationKernel::Linear,
                    nullValue,
                    nullPointStrategy,
                    kernelFootprint,
                    nClosestPoints, radius,
                    sharpness, eccentricity, 0);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::InterpolateProbabilisticVoronoi(
    vtkMRMLDisplayableNode* nodeIn,
    vtkMRMLDisplayableNode* nodeOut,
    double nullValue,
    int nullPointStrategy,
    int kernelFootprint,
    int nClosestPoints,
    double radius)
{
  this->Interpolate(nodeIn,
                    nodeOut,
                    InterpolationKernel::Linear,
                    nullValue,
                    nullPointStrategy,
                    kernelFootprint,
                    nClosestPoints, radius,
                    0, 0, 0);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::InterpolateShepard(
    vtkMRMLDisplayableNode* nodeIn,
    vtkMRMLDisplayableNode* nodeOut,
    double nullValue,
    int nullPointStrategy,
    int kernelFootprint,
    int nClosestPoints,
    double radius,
    double power)
{
  this->Interpolate(nodeIn,
                    nodeOut,
                    InterpolationKernel::Linear,
                    nullValue,
                    nullPointStrategy,
                    kernelFootprint,
                    nClosestPoints, radius,
                    0, 0, power);
}

//---------------------------------------------------------------------------
void vtkSlicerInterpolationLogic::Interpolate(
    vtkMRMLDisplayableNode* nodeIn,
    vtkMRMLDisplayableNode* nodeOut,
    int interpolationKernel,
    double nullValue,
    int nullPointStrategy,
    int kernelFootprint,
    int nClosestPoints,
    double radius,
    double sharpness,
    double eccentricity,
    double power)
{
  if (!nodeIn){
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Input node is NULL");
    return;
  }

  if (!nodeOut){
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Output node is NULL");
    return;
  }

  if (nullPointStrategy != vtkPointInterpolator::Strategy::MASK_POINTS &&
      nullPointStrategy != vtkPointInterpolator::Strategy::NULL_VALUE  &&
      nullPointStrategy != vtkPointInterpolator::Strategy::CLOSEST_POINT){
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Invalid Null Point Strategy");
    return;
  }

  if (kernelFootprint != vtkGeneralizedKernel::KernelStyle::RADIUS &&
      kernelFootprint != vtkGeneralizedKernel::KernelStyle::N_CLOSEST){
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Invalid Kernel Footprint (neither RADIUS nor N_CLOSEST)");
    return;
  }

  if (kernelFootprint == vtkGeneralizedKernel::KernelStyle::RADIUS &&
      radius < 0){
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Kernel Footprint is RADIUS with radius < 0");
    return;
  }

  if (kernelFootprint == vtkGeneralizedKernel::KernelStyle::N_CLOSEST &&
      nClosestPoints < 0){
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Kernel Footprint is N_CLOSEST with NPoints < 0");
    return;
  }

  vtkSmartPointer<vtkGeneralizedKernel> kernel  = nullptr;
  if (interpolationKernel == InterpolationKernel::Linear){
    kernel = vtkSmartPointer<vtkLinearKernel>::New();
  } else if (interpolationKernel == InterpolationKernel::Gaussian) {
    if (sharpness < 0){
      vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Gaussian kernel with sharpness < 0");
      return;
    }
    kernel = vtkSmartPointer<vtkGaussianKernel>::New();
    vtkGaussianKernel::SafeDownCast(kernel)->SetSharpness(sharpness);
  } else if (interpolationKernel == InterpolationKernel::EllipsoidalGaussian) {
    if (sharpness < 0){
      vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: EllipsoidalGaussian kernel with sharpness < 0");
      return;
    } else if (eccentricity < 0){
      vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: EllipsoidalGaussian kernel with eccentricity < 0");
      return;
    }
    kernel = vtkSmartPointer<vtkEllipsoidalGaussianKernel>::New();
    vtkEllipsoidalGaussianKernel::SafeDownCast(kernel)->SetSharpness(sharpness);
    vtkEllipsoidalGaussianKernel::SafeDownCast(kernel)->SetEccentricity(eccentricity);
  } else if (interpolationKernel == InterpolationKernel::ProbabilisticVoronoi) {
    kernel = vtkSmartPointer<vtkProbabilisticVoronoiKernel>::New();
  } else if (interpolationKernel == InterpolationKernel::Shepard) {
    kernel = vtkSmartPointer<vtkShepardKernel>::New();
    vtkShepardKernel::SafeDownCast(kernel)->SetPowerParameter(power);
  } else {
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Invalid Interpolation Kernel");
    return;
  }

  kernel->SetKernelFootprint(kernelFootprint);
  kernel->SetNumberOfPoints(nClosestPoints);
  kernel->SetRadius(radius);

  vtkNew<vtkPointInterpolator> interpolator;
  interpolator->SetKernel(kernel);
  interpolator->SetNullValue(nullValue);
  interpolator->SetNullPointsStrategy(nullPointStrategy);
  // PassPointArraysOff is important
  interpolator->PassPointArraysOff();

  if (vtkMRMLVolumeNode::SafeDownCast(nodeOut)){
    interpolator->SetInputData(vtkMRMLVolumeNode::SafeDownCast(nodeOut)->GetImageData());
  } else if (vtkMRMLModelNode::SafeDownCast(nodeOut)) {
    interpolator->SetInputData(vtkMRMLModelNode::SafeDownCast(nodeOut)->GetMesh());
  } else if (vtkMRMLMarkupsNode::SafeDownCast(nodeOut)) {
    interpolator->SetInputData(vtkMRMLMarkupsNode::SafeDownCast(nodeOut)->GetCurve());
  } else {
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Output node is neither Volume, Model or Markups");
    return;
  }

  if (vtkMRMLVolumeNode::SafeDownCast(nodeIn)){
    interpolator->SetSourceData(vtkMRMLVolumeNode::SafeDownCast(nodeIn)->GetImageData());
  } else if (vtkMRMLModelNode::SafeDownCast(nodeIn)) {
    interpolator->SetSourceData(vtkMRMLModelNode::SafeDownCast(nodeIn)->GetMesh());
  } else if (vtkMRMLMarkupsNode::SafeDownCast(nodeIn)) {
    interpolator->SetSourceData(vtkMRMLMarkupsNode::SafeDownCast(nodeIn)->GetCurve());
  } else {
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Input node is neither Volume, Model or Markups");
    return;
  }

  interpolator->Update();

  if (vtkMRMLVolumeNode::SafeDownCast(nodeOut)){
    vtkMRMLVolumeNode::SafeDownCast(nodeOut)->SetAndObserveImageData(
          interpolator->GetImageDataOutput());
  } else if (vtkMRMLModelNode::SafeDownCast(nodeOut)) {
    vtkMRMLModelNode::SafeDownCast(nodeOut)->SetAndObserveMesh(
          vtkPointSet::SafeDownCast(interpolator->GetOutput()));
  } else if (vtkMRMLMarkupsNode::SafeDownCast(nodeOut)) {
    // for markups we only can try to set scalars
    vtkMRMLMarkupsNode::SafeDownCast(nodeOut)->GetCurve()->GetPointData()->SetScalars(
          vtkPointSet::SafeDownCast(interpolator->GetOutput())->GetPointData()->GetScalars());
  } else {
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Output node is neither Volume, Model or Markups");
    return;
  }
}
