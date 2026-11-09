
set(MC_CORE_LIB_NAME "mc_core")

add_library(${MC_CORE_LIB_NAME} STATIC
	src/volume.cpp)

target_include_directories(${MC_CORE_LIB_NAME} PUBLIC include)
target_compile_features(${MC_CORE_LIB_NAME} PUBLIC cxx_std_20)
target_link_libraries(${MC_CORE_LIB_NAME} PUBLIC glm::glm)