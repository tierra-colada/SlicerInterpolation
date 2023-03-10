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
#include <vtkMRMLDisplayNode.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

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

  std::string scalarName = "ActiveScalarsMerged";
  vtkNew<vtkAppendPolyData> appendFilter;
  for (vtkMRMLDisplayableNode* node : nodesIn){
    vtkMRMLModelNode* modelNode = vtkMRMLModelNode::SafeDownCast(node);
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    if (modelNode && modelNode->GetPolyData()){
      vtkPointData* pointData = modelNode->GetPolyData()->GetPointData();
      vtkCellData* cellData = modelNode->GetPolyData()->GetCellData();
      if (pointData && pointData->GetScalars()){
        vtkNew<vtkDoubleArray> arr;
        arr->DeepCopy(pointData->GetScalars());
        arr->SetName(scalarName.c_str());
        pointData->SetScalars(arr);
      }
      if (cellData && cellData->GetScalars()){
        vtkNew<vtkDoubleArray> arr;
        arr->DeepCopy(cellData->GetScalars());
        arr->SetName(scalarName.c_str());
        cellData->SetScalars(arr);
      }
      appendFilter->AddInputData(modelNode->GetPolyData());
    } else if (markupsNode && markupsNode->GetCurve()){
      vtkMRMLDisplayNode* displayNode = markupsNode->GetDisplayNode();
      if (displayNode && displayNode->GetActiveScalarArray()){
        vtkNew<vtkDoubleArray> arr;
        arr->DeepCopy(displayNode->GetActiveScalarArray());
        arr->SetName(scalarName.c_str());
        markupsNode->GetCurve()->GetPointData()->AddArray(arr);
      }
      appendFilter->AddInputData(markupsNode->GetCurve());
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
    vtkMRMLVolumeNode* nodeOut,
    int appendAxis)
{
  if (!nodeOut){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendImageData: Output node is NULL");
    return;
  }

  vtkNew<vtkImageAppend> appendFilter;
  appendFilter->SetAppendAxis(appendAxis);
  for (vtkMRMLVolumeNode* node : nodesIn){
    if (!node)
      continue;

    appendFilter->AddInputData(node->GetImageData());
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
    vtkMRMLModelNode* nodeOut,
    bool mergeCoincidentalPoints,
    double tol)
{
  if (!nodeOut){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendAny: Output node is NULL");
    return;
  }

  std::string scalarName = "ActiveScalarsMerged";
  vtkNew<vtkAppendFilter> appendFilter;
  appendFilter->SetMergePoints(mergeCoincidentalPoints);
  appendFilter->SetTolerance(tol);
  for (vtkMRMLDisplayableNode* node : nodesIn){
    vtkMRMLModelNode* modelNode = vtkMRMLModelNode::SafeDownCast(node);
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    vtkMRMLVolumeNode* volumeNode = vtkMRMLVolumeNode::SafeDownCast(node);
    if (modelNode && modelNode->GetMesh()){
      vtkPointData* pointData = modelNode->GetPolyData()->GetPointData();
      vtkCellData* cellData = modelNode->GetPolyData()->GetCellData();
      if (pointData && pointData->GetScalars()){
        vtkNew<vtkDoubleArray> arr;
        arr->DeepCopy(pointData->GetScalars());
        arr->SetName(scalarName.c_str());
        pointData->SetScalars(arr);
      }
      if (cellData && cellData->GetScalars()){
        vtkNew<vtkDoubleArray> arr;
        arr->DeepCopy(cellData->GetScalars());
        arr->SetName(scalarName.c_str());
        cellData->SetScalars(arr);
      }
      appendFilter->AddInputData(modelNode->GetMesh());
    } else if (markupsNode && markupsNode->GetCurve()){
      vtkMRMLDisplayNode* displayNode = markupsNode->GetDisplayNode();
      if (displayNode && displayNode->GetActiveScalarArray()){
        vtkNew<vtkDoubleArray> arr;
        arr->DeepCopy(displayNode->GetActiveScalarArray());
        arr->SetName(scalarName.c_str());
        markupsNode->GetCurve()->GetPointData()->AddArray(arr);
      }
      appendFilter->AddInputData(markupsNode->GetCurve());
    } else if (volumeNode && volumeNode->GetImageData()){
      appendFilter->AddInputData(volumeNode->GetImageData());
    }
  }

  if (appendFilter->GetNumberOfInputConnections(0) < 1){
    vtkErrorMacro("vtkSlicerMergeNodesLogic::AppendImageData: No ImageData found");
    return;
  }

  appendFilter->Update();
  nodeOut->SetAndObserveMesh(appendFilter->GetOutput());
}
