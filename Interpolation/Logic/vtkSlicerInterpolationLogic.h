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

// .NAME vtkSlicerInterpolationLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerInterpolationLogic_h
#define __vtkSlicerInterpolationLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSlicerInterpolationModuleLogicExport.h"

class vtkMRMLDisplayableNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_INTERPOLATION_MODULE_LOGIC_EXPORT vtkSlicerInterpolationLogic :
  public vtkSlicerModuleLogic
{
public:

  enum InterpolationKernel{
    Linear,
    Gaussian,
    EllipsoidalGaussian,
    ProbabilisticVoronoi,
    Shepard
  };

  static vtkSlicerInterpolationLogic *New();
  vtkTypeMacro(vtkSlicerInterpolationLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void InterpolateLinear(
      vtkMRMLDisplayableNode* nodeIn,
      vtkMRMLDisplayableNode* nodeOut,
      double nullValue,
      int nullPointStrategy,
      vtkStdString maskArrayName,
      int kernelFootprint,
      int nClosestPoints,
      double radius);

  void InterpolateGaussian(
      vtkMRMLDisplayableNode* nodeIn,
      vtkMRMLDisplayableNode* nodeOut,
      double nullValue,
      int nullPointStrategy,
      vtkStdString maskArrayName,
      int kernelFootprint,
      int nClosestPoints,
      double radius,
      double sharpness);

  void InterpolateEllipsoidalGaussian(
      vtkMRMLDisplayableNode* nodeIn,
      vtkMRMLDisplayableNode* nodeOut,
      double nullValue,
      int nullPointStrategy,
      vtkStdString maskArrayName,
      int kernelFootprint,
      int nClosestPoints,
      double radius,
      double sharpness,
      double eccentricity);

  void InterpolateProbabilisticVoronoi(
      vtkMRMLDisplayableNode* nodeIn,
      vtkMRMLDisplayableNode* nodeOut,
      double nullValue,
      int nullPointStrategy,
      vtkStdString maskArrayName,
      int kernelFootprint,
      int nClosestPoints,
      double radius);

  void InterpolateShepard(
      vtkMRMLDisplayableNode* nodeIn,
      vtkMRMLDisplayableNode* nodeOut,
      double nullValue,
      int nullPointStrategy,
      vtkStdString maskArrayName,
      int kernelFootprint,
      int nClosestPoints,
      double radius,
      double power);

  /// interpolationKernel defined as enum
  /// vtkSlicerInterpolationLogic::interpolationKernel
  /// IO nodes are either Volume, Model or Markups
  void Interpolate(
      vtkMRMLDisplayableNode* nodeIn,
      vtkMRMLDisplayableNode* nodeOut,
      int interpolationKernel,
      double nullValue,
      int nullPointStrategy,
      vtkStdString maskArrayName,
      int kernelFootprint,
      int nClosestPoints,
      double radius,
      double sharpness,
      double eccentricity,
      double power);

protected:
  vtkSlicerInterpolationLogic();
  ~vtkSlicerInterpolationLogic() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

  vtkSlicerInterpolationLogic(const vtkSlicerInterpolationLogic&); // Not implemented
  void operator=(const vtkSlicerInterpolationLogic&); // Not implemented
};

#endif
