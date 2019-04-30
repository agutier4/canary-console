cmake_minimum_required (VERSION 3.0)
project (canary-console)
include_directories(sdk/sdk/include sdk/sdk/src)
link_directories(.)

find_package(lcm REQUIRED)

# find opengl and qt4 libraries and headers
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
message("OPENGL_INCLUDE_DIR:${OPENGL_INCLUDE_DIR}")
message("QT_INCLUDE_DIR:${QT_INCLUDE_DIR}")

qt_wrap_cpp(viewerWidget MOC_SRCS viewerWidget.h)
add_library(viewerWidget viewerWidget.cpp ${MOC_SRCS})

# generate viewer executable
add_executable(viewer viewer.cpp)
target_link_libraries(viewer viewerWidget ${QT_LIBRARIES} ${OPENGL_LIBRARIES} lcm)

# generate simulatedata executable
add_executable(simulateData simulateData.cpp virtualLidar.cpp)
target_link_libraries(simulateData lcm)

# generate lidarScan add_executable
add_executable(lidarScan lidarScan.cpp lidar.cpp)
target_link_libraries(lidarScan rplidar_sdk pthread)
target_link_libraries(lidarScan lcm)

