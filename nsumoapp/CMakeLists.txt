cmake_minimum_required(VERSION 3.8)
project(nsumoapp VERSION 1.0)

# Modify to where the bots2d folder is stored
set (BOTS2D_RELATIVE_PATH ../)

set (NSUMOAPP_ASSETS
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/RectObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/QuadObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/CircleObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/LineObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/actuators/WheelMotor.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/bodies/SimpleBotBody.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/BaseBot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/Sumobot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/sensors/RangeSensorObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/sensors/LineDetectorObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/playgrounds/Dohyo.cpp
)

set (NSUMOAPP_SCENES
    scenes/NsumoScene.cpp
)

set (NSUMOAPP_CONTROLLERS
    controllers/NsumoController/main.c
    controllers/NsumoController/NsumoMicrocontroller.cpp
    controllers/NsumoController/sumo_robot/src/common/enum_to_string.c
    controllers/NsumoController/sumo_robot/src/common/ring_buffer.c
    controllers/NsumoController/sumo_robot/src/app/line.c
    controllers/NsumoController/sumo_robot/src/app/drive.c
    controllers/NsumoController/sumo_robot/src/app/enemy.c
    controllers/NsumoController/sumo_robot/src/app/timer.c
    controllers/NsumoController/sumo_robot/src/app/input_history.c
    controllers/NsumoController/sumo_robot/src/app/state_machine.c
    controllers/NsumoController/sumo_robot/src/app/state_retreat.c
    controllers/NsumoController/sumo_robot/src/app/state_attack.c
    controllers/NsumoController/sumo_robot/src/app/state_search.c
    controllers/NsumoController/sumo_robot/src/app/state_wait.c
    controllers/NsumoController/sumo_robot/src/app/state_manual.c
    controllers/NsumoController/nsumo_simulated/tb6612fng.c
    controllers/NsumoController/nsumo_simulated/qre1113.c
    controllers/NsumoController/nsumo_simulated/vl53l0x.c
    controllers/NsumoController/nsumo_simulated/trace.c
    controllers/NsumoController/nsumo_simulated/assert_handler.c
    controllers/NsumoController/nsumo_simulated/ir_remote.c
)

set (NSUMOAPP_FILES
    ${NSUMOAPP_ASSETS}
    ${NSUMOAPP_SCENES}
    ${NSUMOAPP_CONTROLLERS}
    NsumoApp.cpp
    main.cpp
)

add_executable(nsumoapp
    ${NSUMOAPP_FILES}
)

target_include_directories(nsumoapp PRIVATE scenes)
target_include_directories(nsumoapp PRIVATE controllers/NsumoController/sumo_robot/src)
target_include_directories(nsumoapp PRIVATE controllers)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/assets/objects)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/core)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/renderer)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/transforms)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/scene)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/physics)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/controllers)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/controllers/components)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/external/glm)

# Include bots2d and build it as a static lib
add_subdirectory(${BOTS2D_RELATIVE_PATH} build)
target_link_libraries(nsumoapp PRIVATE bots2d)

target_compile_definitions(nsumoapp PRIVATE BUILD_TEST)

set_target_properties(nsumoapp PROPERTIES
       CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(nsumoapp PRIVATE /W4 /WX)
else()
  target_compile_options(nsumoapp PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
