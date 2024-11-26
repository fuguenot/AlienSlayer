cmake_minimum_required(VERSION 3.28)

project(
  AlienSlayer
  VERSION 1.0.0
  DESCRIPTION "Kill the aliens."
  LANGUAGES CXX C)

find_package(SDL2 REQUIRED CONFIG PATHS "Frameworks" NO_DEFAULT_PATH)
find_package(SDL2_image REQUIRED CONFIG PATHS "Frameworks" NO_DEFAULT_PATH)
find_package(SDL2_ttf REQUIRED CONFIG PATHS "Frameworks" NO_DEFAULT_PATH)

file(COPY "Resources" DESTINATION "alienslayer.app/Contents")
file(COPY "Frameworks" DESTINATION "alienslayer.app/Contents")

set(APPLE_GUI_BUNDLE_NAME AlienSlayer)
set_source_files_properties("Resources/alienslayer.icns"
                            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set(APPLE_GUI_ICON alienslayer)

add_executable(
  AlienSlayer MACOSX_BUNDLE
  "src/main.cpp"
  "src/error.h"
  "src/error.cpp"
  "src/game.h"
  "src/game.cpp"
  "src/spritesheet.h"
  "src/spritesheet.cpp"
  "src/util.h"
  "src/util.cpp"
  "src/alien.h"
  "src/alien.cpp"
  "src/text.h"
  "src/text.cpp"
  "src/update.cpp"
  "src/button.h"
  "src/button.cpp"
  "Resources/alienslayer.icns")

set_target_properties(
  AlienSlayer
  PROPERTIES BUNDLE ON
             MACOSX_BUNDLE_GUI_IDENTIFIER "com.fuguenot.alienslayer"
             MACOSX_BUNDLE_BUNDLE_NAME "AlienSlayer"
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_INFO_PLIST
             "${CMAKE_SOURCE_DIR}/cmake/AppleInfo.plist"
             BUILD_RPATH
             "${CMAKE_SOURCE_DIR}/build/alienslayer.app/Contents/Frameworks")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/Frameworks/SDL2.framework"
        DESTINATION "alienslayer.app/Contents/Frameworks")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Frameworks/SDL2_image.framework"
        DESTINATION "alienslayer.app/Contents/Frameworks")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Frameworks/SDL2_ttf.framework"
        DESTINATION "alienslayer.app/Contents/Frameworks")

install(TARGETS AlienSlayer DESTINATION "${CMAKE_SOURCE_DIR}/build")

target_link_libraries(AlienSlayer PRIVATE SDL2::SDL2 SDL2_image::SDL2_image
                                          SDL2_ttf::SDL2_ttf)
