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

#ifndef __qSlicerInterpolationModuleWidget_h
#define __qSlicerInterpolationModuleWidget_h

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerInterpolationModuleExport.h"

class qSlicerInterpolationModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_INTERPOLATION_EXPORT qSlicerInterpolationModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerInterpolationModuleWidget(QWidget *parent=0);
  virtual ~qSlicerInterpolationModuleWidget();

public slots:
  void onNullPointStrategyComboBoxTextChanged(const QString& text);
  void onKernelComboBoxTextChanged(const QString& text);
  void onKernelFootPrintComboBoxTextChanged(const QString& text);
  void onApplyButtonClicked();

protected:
  QScopedPointer<qSlicerInterpolationModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qSlicerInterpolationModuleWidget);
  Q_DISABLE_COPY(qSlicerInterpolationModuleWidget);
};

#endif
