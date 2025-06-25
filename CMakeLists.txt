cmake_minimum_required(VERSION 3.10)
project(SmartMeterSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Шлях до бібліотеки Gurux
set(GURUX_DIR ../Gurux.DLMS.cpp)  # Шлях до бібліотеки (залишаємо як є)
get_filename_component(GURUX_ABS_DIR "${GURUX_DIR}" ABSOLUTE)
set(GURUX_LIB ${GURUX_ABS_DIR}/development/lib/libgurux_dlms_cpp.a)

# Додаємо інклуди (твій проект + Gurux)
include_directories(
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/tcp
    ${PROJECT_SOURCE_DIR}/data
    ${PROJECT_SOURCE_DIR}/model
    ${PROJECT_SOURCE_DIR}/profile
    ${PROJECT_SOURCE_DIR}/updater
    ${PROJECT_SOURCE_DIR}/cli
    ${PROJECT_SOURCE_DIR}/config
    ${PROJECT_SOURCE_DIR}/logging
    ${PROJECT_SOURCE_DIR}/utils

    ${GURUX_ABS_DIR}/development/include
)

# Файли твого проекту
add_executable(smart_meter_simulator
    main.cpp
    core/DlmsHandler.cpp
    tcp/TcpServer.cpp
    generator/DataGenerator.cpp
    model/ObisObjectModel.cpp
    profile/ProfileGenericManager.cpp
    updater/Updater.cpp
    cli/CLI.cpp
    config/Config.cpp
    logging/Logger.cpp
    utils/Helpers.cpp
)

# Лінкуємо бібліотеку Gurux
target_link_libraries(smart_meter_simulator
    ${GURUX_LIB}
)

# Лінкуємо pthread для Linux
if(UNIX)
    target_link_libraries(smart_meter_simulator pthread)
endif()

# Для Windows: лінкування з Winsock
if(WIN32)
    target_link_libraries(smart_meter_simulator ws2_32)
endif()
