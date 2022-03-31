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

// .NAME vtkSlicerMergeNodesLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerMergeNodesLogic_h
#define __vtkSlicerMergeNodesLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSlicerMergeNodesModuleLogicExport.h"

class vtkMRMLDisplayableNode;
class vtkMRMLModelNode;
class vtkMRMLVolumeNode;
class vtkMRMLScalarVolumeNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_MERGENODES_MODULE_LOGIC_EXPORT vtkSlicerMergeNodesLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerMergeNodesLogic *New();
  vtkTypeMacro(vtkSlicerMergeNodesLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Model (with polydata) nodes or Markups
  void AppendPolyData(
      const std::vector<vtkMRMLDisplayableNode*>& nodesIn,
      vtkMRMLModelNode* nodeOut);

  /// appendAxis: 0 - X; 1 - Y; 2 - Z
  void AppendImageData(
      const std::vector<vtkMRMLVolumeNode*>& nodesIn,
      vtkMRMLVolumeNode* nodeOut,
      int appendAxis);

  /// The output is Model node with unstructured grid
  void AppendAny(
      const std::vector<vtkMRMLDisplayableNode*>& nodesIn,
      vtkMRMLModelNode* nodeOut,
      bool mergeCoincidentalPoints,
      double tol);

protected:
  vtkSlicerMergeNodesLogic();
  ~vtkSlicerMergeNodesLogic() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

  vtkSlicerMergeNodesLogic(const vtkSlicerMergeNodesLogic&); // Not implemented
  void operator=(const vtkSlicerMergeNodesLogic&); // Not implemented
};

#endif
