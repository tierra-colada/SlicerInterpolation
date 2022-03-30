cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)

project(SlicerInterpolation)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://www.slicer.org/wiki/Documentation/Nightly/Extensions/SlicerInterpolation")
set(EXTENSION_CATEGORY "Interpolation")
set(EXTENSION_CONTRIBUTORS "Kerim Khemraev (Tierra Colada)")
set(EXTENSION_DESCRIPTION "The extension provides means to merge and interpolate displayable nodes")
set(EXTENSION_ICONURL "http://www.example.com/Slicer/Extensions/SlicerInterpolation.png")
set(EXTENSION_SCREENSHOTURLS "http://www.example.com/Slicer/Extensions/SlicerInterpolation/Screenshots/1.png")
set(EXTENSION_DEPENDS "NA") # Specified as a list or "NA" if no dependencies

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(MergeNodes)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_GENERATE_CONFIG})
include(${Slicer_EXTENSION_CPACK})
