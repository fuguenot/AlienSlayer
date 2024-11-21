cmake_minimum_required(VERSION 3.28)

project(
  AlienSlayer
  VERSION 0.1.0
  DESCRIPTION "Kill the aliens."
  LANGUAGES CXX C)

find_package(SDL2 REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)
find_package(SDL2_image REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)
find_package(SDL2_ttf REQUIRED CONFIG PATHS "." NO_DEFAULT_PATH)

file(COPY Resources DESTINATION alienslayer.app/Contents)

set(APPLE_GUI_BUNDLE_NAME AlienSlayer)
set_source_files_properties("Resources/alienslayer.icns"
                            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set(APPLE_GUI_ICON alienslayer)

add_executable(
  AlienSlayer MACOSX_BUNDLE "src/main.cpp" "src/error.h" "src/error.cpp"
                            "Resources/alienslayer.icns")

set_target_properties(
  AlienSlayer
  PROPERTIES BUNDLE ON
             MACOSX_BUNDLE_GUI_IDENTIFIER "com.fuguenot.alienslayer"
             MACOSX_BUNDLE_BUNDLE_NAME "AlienSlayer"
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/AppleInfo.plist)

target_link_libraries(AlienSlayer PRIVATE SDL2::SDL2 SDL2_image::SDL2_image
                                          SDL2_ttf::SDL2_ttf)
