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
#include "vtkSlicerMergeNodesLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLVolumeNode.h>
#include <vtkMRMLMarkupsNode.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkAppendPolyData.h>
#include <vtkImageAppend.h>
#include <vtkAppendFilter.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerMergeNodesLogic);

//----------------------------------------------------------------------------
vtkSlicerMergeNodesLogic::vtkSlicerMergeNodesLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerMergeNodesLogic::~vtkSlicerMergeNodesLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::AppendPolyData(
    const std::vector<vtkMRMLDisplayableNode*>& nodesIn,
    vtkMRMLModelNode* nodeOut)
{
  if (!nodeOut){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendPolyData: Output node is NULL");
    return;
  }

  vtkNew<vtkAppendPolyData> appendFilter;
  for (vtkMRMLDisplayableNode* node : nodesIn){
    if (vtkMRMLModelNode::SafeDownCast(node) &&
        vtkMRMLModelNode::SafeDownCast(node)->GetPolyData()){
      appendFilter->AddInputData(vtkMRMLModelNode::SafeDownCast(node)->GetPolyData());
    } else if (vtkMRMLMarkupsNode::SafeDownCast(node) &&
               vtkMRMLMarkupsNode::SafeDownCast(node)->GetCurve()){
      appendFilter->AddInputData(vtkMRMLMarkupsNode::SafeDownCast(node)->GetCurve());
    }
  }

  if (appendFilter->GetNumberOfInputConnections(0) < 1){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendPolyData: No PolyData found");
    return;
  }

  appendFilter->Update();
  nodeOut->SetAndObservePolyData(appendFilter->GetOutput());
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::AppendImageData(
    const std::vector<vtkMRMLVolumeNode*>& nodesIn,
    int appendAxis,
    vtkMRMLVolumeNode* nodeOut)
{
  if (!nodeOut){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendImageData: Output node is NULL");
    return;
  }

  int idx = 0;
  vtkNew<vtkImageAppend> appendFilter;
  appendFilter->SetAppendAxis(appendAxis);
  for (vtkMRMLVolumeNode* node : nodesIn){
    if (!node)
      continue;

    appendFilter->SetInputData(idx, node->GetImageData());
    idx ++;
  }

  if (appendFilter->GetNumberOfInputConnections(0) < 1){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendImageData: No ImageData found");
    return;
  }

  appendFilter->Update();
  nodeOut->SetAndObserveImageData(appendFilter->GetOutput());
}

//---------------------------------------------------------------------------
void vtkSlicerMergeNodesLogic::AppendAny(
    const std::vector<vtkMRMLDisplayableNode*>& nodesIn,
    bool mergeCoincidentalPoints,
    double tol,
    vtkMRMLModelNode* nodeOut)
{
  if (!nodeOut){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendAny: Output node is NULL");
    return;
  }

  vtkNew<vtkAppendFilter> appendFilter;
  appendFilter->SetMergePoints(mergeCoincidentalPoints);
  appendFilter->SetTolerance(tol);
  for (vtkMRMLDisplayableNode* node : nodesIn){
    if (vtkMRMLModelNode::SafeDownCast(node) &&
        vtkMRMLModelNode::SafeDownCast(node)->GetMesh()){
      appendFilter->AddInputData(vtkMRMLModelNode::SafeDownCast(node)->GetMesh());
    } else if (vtkMRMLMarkupsNode::SafeDownCast(node) &&
               vtkMRMLMarkupsNode::SafeDownCast(node)->GetCurve()){
      appendFilter->AddInputData(vtkMRMLMarkupsNode::SafeDownCast(node)->GetCurve());
    } else if (vtkMRMLVolumeNode::SafeDownCast(node) &&
               vtkMRMLVolumeNode::SafeDownCast(node)->GetImageData()){
      appendFilter->AddInputData(vtkMRMLVolumeNode::SafeDownCast(node)->GetImageData());
    }
  }

  if (appendFilter->GetNumberOfInputConnections(0) < 1){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendImageData: No ImageData found");
    return;
  }

  appendFilter->Update();
  nodeOut->SetAndObserveMesh(appendFilter->GetOutput());
}
