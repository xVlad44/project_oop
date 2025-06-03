# Test configuration for Archaeological Artifacts Inventory
cmake_minimum_required(VERSION 3.16)

# Find Google Test
find_package(GTest REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)

# Create test executable
add_executable(artifact_tests
    test_main.cpp
    ../src/domain/artifact.cpp
    ../src/repository/csv_repository.cpp
    ../src/repository/json_repository.cpp
    ../src/controller/artifact_controller.cpp
    ../src/controller/command.cpp
    ../src/controller/filter.cpp
)

# Link libraries
target_link_libraries(artifact_tests
    GTest::GTest
    GTest::Main
    Qt6::Core
)

# Include directories
target_include_directories(artifact_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Enable testing
enable_testing()
add_test(NAME ArtifactTests COMMAND artifact_tests)
