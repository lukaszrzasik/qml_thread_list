set(SRC_LIST main.cpp
             SleepWorker.cpp
             Worker.cpp)

find_package(Qt5 COMPONENTS Quick Core Qml)
qt5_add_resources(RESOURCES resources/images.qrc)

add_executable(threadListQml ${SRC_LIST} ${RESOURCES})
qt5_use_modules(threadListQml Quick Core Qml)
