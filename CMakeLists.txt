cmake_minimum_required(VERSION 3.16)

# ============================
#  Variables
# ============================
include(.cmake.conf)
# ============================

project(${MODULE_NAME}
        VERSION ${MODULE_VERSION}
        LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ============================
# Qt Setup
# ============================
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# QML Install destinition
set(${MODULE_NAME}_QML_INSTALL_DEST ${CMAKE_INSTALL_PREFIX}/qml)

# Share Install destinition
set(${MODULE_NAME}_SHARE_INSTALL_DEST ${CMAKE_INSTALL_PREFIX}/share)

# Optional QML support
option(${MODULE_NAME}_WITH_QML "Enable QML modules" OFF)
if(${MODULE_NAME}_WITH_QML)
    # Importing base dependencies
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Qml Quick)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Qml Quick)

    # Generating config.h form config.h.in (WeaQuick_QML_IMPORT_PATH) must be assign from CMake.
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${MODULE_NAME}_config.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}/${MODULE_NAME}_config.h)

    # Adding QML files into the QML_IMPORT_PATH automatically
    # Directories where Qt Creator can find QML files.
    # (Not needed for builds, but makes Qt Creator code completion happy.)
    list(APPEND ${MODULE_NAME}_QML_IMPORTS "${${MODULE_NAME}_QML_INSTALL_DEST}")
    list(APPEND ${MODULE_NAME}_QML_IMPORTS "${CMAKE_CURRENT_SOURCE_DIR}/qml/")

    # Prevent adding duplicate values at each run of CMake.
    list(REMOVE_DUPLICATES ${MODULE_NAME}_QML_IMPORTS)

    # The variable is cached in ${BUILD_DIR}/CMakeCache.txt. We need FORCE to
    # change it there immediately. Also, add a comment to the cache file.
    set(QML_IMPORT_PATH "${${MODULE_NAME}_QML_IMPORTS}"
        CACHE STRING "${MODULE_NAME} QML Imports Paths for Qt Creator"
        FORCE)
endif()

# ============================
# Files
# ============================
file(GLOB_RECURSE MODULE_HEADERS CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE MODULE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
file(GLOB_RECURSE MODULE_SCRIPTS CONFIGURE_DEPENDS "scripts/*.js")
file(GLOB_RECURSE MODULE_RESOURCES CONFIGURE_DEPENDS "*.qrc" "qml/*.qrc" "share/*.qrc" "shaders/*.qrc" "scripts/*.qrc")
file(GLOB_RECURSE MODULE_SHADERS CONFIGURE_DEPENDS "shaders/*.glsl" "shaders/*.vert" "shaders/*.frag")

add_library(${MODULE_NAME} SHARED
    ${MODULE_HEADERS}
    ${MODULE_SOURCES}
    ${MODULE_SCRIPTS}
    ${MODULE_SHADERS}
    ${MODULE_RESOURCES}
)

# This ALIAS Helps using Module as Subdirectory and linking it by:
#   `target_link_libraries(Project PRIVATE ${MODULE_NAMESPACE}::${MODULE_NAME})`
add_library(
    ${MODULE_NAMESPACE}::${MODULE_NAME}
    ALIAS
    ${MODULE_NAME}
)

target_include_directories(${MODULE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${MODULE_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)

if(${MODULE_NAME}_WITH_QML)
    target_link_libraries(${MODULE_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick)
endif()

# Export definition
target_compile_definitions(${MODULE_NAME} PRIVATE ${MODULE_NAME}_EXPORT)

# ============================
# Meta & BUILD Automation
# ============================
include(cmake/CopyShare.cmake)
copy_share(${MODULE_NAME})

# ============================
# Install
# ============================
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${MODULE_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MODULE_NAME}
)

install(TARGETS ${MODULE_NAME}
    EXPORT ${MODULE_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MODULE_NAME}
)

install(EXPORT ${MODULE_NAME}Targets
    FILE ${MODULE_NAME}Targets.cmake
    NAMESPACE ${MODULE_NAMESPACE}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MODULE_NAME}
)

# Install QML files (if exists)
if(${MODULE_NAME}_WITH_QML)
    set(${MODULE_NAME}_QML_DEST "${${MODULE_NAME}_QML_INSTALL_DEST}/com/wearily/${MODULE_NAME}")
    install(DIRECTORY qml/com/wearily/${MODULE_NAME}/
        DESTINATION ${${MODULE_NAME}_QML_DEST}
        FILES_MATCHING PATTERN "*.qml" PATTERN "qmldir"
    )
endif()

# Install shares
set(${MODULE_NAME}_SHARE_DEST "${${MODULE_NAME}_SHARE_INSTALL_DEST}")
install(DIRECTORY share/
    DESTINATION ${${MODULE_NAME}_SHARE_DEST}
    FILES_MATCHING PATTERN "*"
)

# Removing Duplicate WeaChart directories.
install(DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR}/${MODULE_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ============================
# Examples & Tests
# ============================
option(${MODULE_NAME}_BUILD_EXAMPLES "Build examples" OFF)
if(${MODULE_NAME}_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

option(${MODULE_NAME}_BUILD_TESTS "Build tests" OFF)
if(${MODULE_NAME}_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()


# Also Important flags
message(STATUS
   "CMakeAttributes: "
   "\nCMAKE_INSTALL_DATADIR: ${CMAKE_INSTALL_DATADIR}"
   "\nCMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}"
   "\nCMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}"
   "\nCMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}"
   "\nCMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}"
   "\nCMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}"
   "\nCMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}"
   "\nPROJECT_VERSION: ${PROJECT_VERSION}"
   "\nTemplate Attributes: "
   "\nMODULE_NAME: ${MODULE_NAME}"
   "\nMODULE_NAMESPACE: ${MODULE_NAMESPACE}"
   "\nMODULE_VERSION: ${MODULE_VERSION}"
   "\nMODULE_HEADERS: ${MODULE_HEADERS}"
   "\nMODULE_SOURCES: ${MODULE_SOURCES}"
   "\nMODULE_SCRIPTS: ${MODULE_SCRIPTS}"
   "\nMODULE_SHADERS: ${MODULE_SHADERS}"
   "\nMODULE_RESOURCES: ${MODULE_RESOURCES}"
   "\nMODULE_SHARE_INSTALL_DIR: ${${MODULE_NAME}_SHARE_INSTALL_DEST}"
   "\nWITH_QML: ${${MODULE_NAME}_WITH_QML}"
   "\nQML_DEST: ${${MODULE_NAME}_QML_INSTALL_DEST}"
   "\nQML_DEST_INSTALL: ${${MODULE_NAME}_QML_DEST}"
)
