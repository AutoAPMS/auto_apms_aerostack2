cmake_minimum_required(VERSION 3.8)
project(auto_apms_aerostack2)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(auto_apms_behavior_tree_core REQUIRED)
find_package(as2_msgs REQUIRED)

# Theoretically we don't need to find auto_apms_behavior_tree here since we link dynamically (not statically) 
# to the shared lib that contains the builtin nodes because we reuse some of them in our node manifest.
# For better clarity and to avoid confusion, we will explicitly find it here nevertheless.
find_package(auto_apms_behavior_tree REQUIRED)

set(_AUTO_APMS_AEROSTACK2__PLATFORM_NAMESPACE platform)
set(_AUTO_APMS_AEROSTACK2__PLATFORM_SET_CONTROL_MODE_SRV set_platform_control_mode)
set(_AUTO_APMS_AEROSTACK2__PLATFORM_ARMING_STATE_SRV set_arming_state)
set(_AUTO_APMS_AEROSTACK2__PLATFORM_OFFBOARD_MODE_SRV set_offboard_mode)
set(_AUTO_APMS_AEROSTACK2__PLATFORM_TAKEOFF_SRV platform_takeoff)
set(_AUTO_APMS_AEROSTACK2__PLATFORM_LAND_SRV platform_land)
set(_AUTO_APMS_AEROSTACK2__PLATFORM_INFO_TOPIC platform_land)

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE
    auto_apms_behavior_tree_core::auto_apms_behavior_tree_core # Adding this for the generated node model header
)

# Behavior Tree Nodes
add_library(behavior_tree_nodes SHARED
    "src/set_control_mode.cpp"
)
target_link_libraries(behavior_tree_nodes PUBLIC
    auto_apms_behavior_tree_core::auto_apms_behavior_tree_core
    ${as2_msgs_TARGETS}
)
auto_apms_behavior_tree_register_nodes(behavior_tree_nodes 
    auto_apms_aerostack2::SetControlMode
    NODE_MANIFEST
    config/node_manifest.yaml
    NODE_MODEL_HEADER_TARGET 
    ${PROJECT_NAME}
)

# Export library
install(
    TARGETS
    ${PROJECT_NAME}
    EXPORT export_${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(
    auto_apms_behavior_tree_core # Adding this for the generated node model header
)

# Plugin library
install(
    TARGETS
    behavior_tree_nodes
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

ament_package()
