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
#include <vtkMRMLMarkupsDisplayNode.h>
#include <vtkMRMLStaticMeasurement.h>

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
#include <vtkTransform.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformFilter.h>
#include <vtkPolyLine.h>
#include <vtkAssignAttribute.h>

// STD includes
#include <cassert>
#include <vector>

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

  vtkMRMLModelNode* modelNodeIn = vtkMRMLModelNode::SafeDownCast(nodeIn);
  vtkMRMLModelNode* modelNodeOut = vtkMRMLModelNode::SafeDownCast(nodeOut);
  vtkMRMLVolumeNode* volumeNodeOut = vtkMRMLVolumeNode::SafeDownCast(nodeOut);
  vtkMRMLMarkupsNode* markupsNodeOut = vtkMRMLMarkupsNode::SafeDownCast(nodeOut);
  if (modelNodeIn && volumeNodeOut) {
    if (!volumeNodeOut->GetImageData()) {
      vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Output volume doesn't contain ImageData");
      return;
    }
    // We can't trasnform imageData so that interpolator works correctly.
    // But instead we can apply inverse transform to the model node.
    double origin[3] = { 0 };
    volumeNodeOut->GetOrigin(origin);
    double spacings[3] = { 0 };
    volumeNodeOut->GetSpacing(spacings);

    vtkNew<vtkImageData> imageData;
    imageData->ShallowCopy(volumeNodeOut->GetImageData());
    imageData->SetOrigin(0,0,0);
    imageData->SetSpacing(spacings);

    vtkNew<vtkMatrix4x4> m;
    // get directions
    volumeNodeOut->GetIJKToRASDirectionMatrix(m.GetPointer());
    // add translation
    m->SetElement(0,3,origin[0]);
    m->SetElement(1,3,origin[1]);
    m->SetElement(2,3,origin[2]);

    vtkNew<vtkTransform> transform;
    transform->SetMatrix(m);
    transform->Inverse();

    vtkNew<vtkTransformFilter> transformFilter;
    transformFilter->SetInputData(modelNodeIn->GetMesh());
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    interpolator->SetSourceData(transformFilter->GetOutput());
    interpolator->SetInputData(imageData);
    interpolator->GetImageDataOutput()->SetOrigin(0,0,0);
    interpolator->GetImageDataOutput()->SetSpacing(1,1,1);
    interpolator->Update();

    volumeNodeOut->SetAndObserveImageData(interpolator->GetImageDataOutput());
  } else if (modelNodeIn && markupsNodeOut) {
    if (!markupsNodeOut->GetCurve() || !markupsNodeOut->GetCurve()->GetPointData()) {
      vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Markups doesn't contain pointData");
      return;
    }

    // Create a vtkPoints object and store the points in it
    vtkNew<vtkPoints> points;
    for (size_t i = 0; i < markupsNodeOut->GetNumberOfControlPoints(); i++) {
      double p[3] = { 0 };
      markupsNodeOut->GetNthControlPointPositionWorld(i, p);
      points->InsertNextPoint(p);
    }

    vtkNew<vtkPolyLine> polyLine;
    polyLine->GetPointIds()->SetNumberOfIds(points->GetNumberOfPoints());
    for (size_t i = 0; i < points->GetNumberOfPoints(); i++){
      polyLine->GetPointIds()->SetId(i, i);
    }

    vtkNew<vtkDoubleArray> arr;
    arr->SetName("data");
    arr->SetNumberOfValues(points->GetNumberOfPoints());

    // Create a cell array to store the lines in and add the lines to it
    vtkNew<vtkCellArray> cells;
    cells->InsertNextCell(polyLine);

    // Create a polydata to store everything in
    vtkNew<vtkPolyData> polyData;

    // Add the points to the dataset
    polyData->SetPoints(points);

    // Add the lines to the dataset
    polyData->SetLines(cells);
    polyData->GetPointData()->SetScalars(arr);

    interpolator->SetSourceData(modelNodeIn->GetMesh());
    interpolator->SetInputData(polyData);
    interpolator->Update();

    vtkPointSet* interpolatedPointSet = vtkPointSet::SafeDownCast(interpolator->GetOutput());
    if (!interpolatedPointSet || 
      !interpolatedPointSet->GetPointData() || 
      !interpolatedPointSet->GetPointData()->GetScalars() ||
      interpolatedPointSet->GetPointData()->GetScalars()->GetNumberOfValues() != markupsNodeOut->GetNumberOfControlPoints()) {
      vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Unable to interpolate model on markups");
      return;
    }

    vtkNew<vtkDoubleArray> arrInterpolated;
    arrInterpolated->DeepCopy(interpolatedPointSet->GetPointData()->GetScalars());
    std::string measurementName = "interpolatedValues";
    if (markupsNodeOut->GetMeasurement(measurementName.c_str())) {
      vtkMRMLMeasurement* measurement = markupsNodeOut->GetMeasurement(measurementName.c_str());
      measurement->SetControlPointValues(arrInterpolated);
      measurement->Modified();
    } else {
      vtkNew<vtkMRMLStaticMeasurement> measurement;
      measurement->SetName(measurementName);
      measurement->SetUnits("mm");
      measurement->SetPrintFormat("");  // Prevent from showing up in SH Description
      measurement->SetControlPointValues(arrInterpolated);
      markupsNodeOut->AddMeasurement(measurement);
    }

    vtkMRMLMarkupsDisplayNode* dispNode =
      vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNodeOut->GetDisplayNode());
    if (dispNode){
      double valRange[2] = {0,0};
      arrInterpolated->GetFiniteRange(valRange);
      dispNode->SetScalarRange(valRange[0], valRange[1]);
      dispNode->SetActiveScalar(measurementName.c_str(), vtkAssignAttribute::POINT_DATA);
      dispNode->SetScalarVisibility(true);
    }
  } else if (modelNodeIn && modelNodeOut) {
    interpolator->SetSourceData(modelNodeIn->GetMesh());
    interpolator->SetInputData(modelNodeOut->GetMesh());
    interpolator->Update();

    modelNodeOut->SetAndObserveMesh(
      vtkPointSet::SafeDownCast(interpolator->GetOutput()));
  } else {
    vtkErrorMacro("vtkSlicerInterpolationLogic::Interpolate: Input node is not a Model and output is neither Model, Volume or Markups");
    return;
  }
}
