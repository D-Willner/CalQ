cmake_minimum_required(VERSION 3.16)

project(CalQ VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts Sql Network)

qt_standard_project_setup()

include_directories(${CMAKE_SOURCE_DIR})

qt_add_executable(CalQ
  main.cpp
  gui/CalQ.cpp
  gui/AIConfigDialog.cpp
  gui/CalorieBars.cpp
  gui/CalorieDisplay.cpp
  gui/DataBaseTab.cpp
  gui/ExerciseTable.cpp
  gui/FoodTable.cpp
  gui/HistoryTab.cpp
  gui/InitializeDialog.cpp
  gui/MacroChart.cpp
  gui/MainTab.cpp
  gui/RecipeDialog.cpp
  gui/SearchField.cpp
  gui/SearchInput.cpp
  gui/SettingsTab.cpp
  gui/SQLConfigDialog.cpp
  gui/ToolTip.cpp
  gui/ToolTipLabel.cpp
  gui/WeightChart.cpp
  database/DataBase.cpp
  database/DataBaseSearcher.cpp
  database/DataCurrent.cpp
  database/Exercise.cpp
  database/ExerciseType.cpp
  database/FoodType.cpp
  database/Food.cpp
  database/Meal.cpp
  database/Recipe.cpp
  database/Settings.cpp
  database/WeightEntry.cpp
  network/AISettings.cpp
  network/Client.cpp
  network/SQLDatabase.cpp
  network/SQLSettings.cpp
)

target_link_libraries(CalQ PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts Qt6::Sql Qt6::Network)

set_target_properties(CalQ PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
