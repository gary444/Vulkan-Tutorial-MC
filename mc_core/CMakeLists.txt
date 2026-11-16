
set(MC_CORE_LIB_NAME "mc_core")

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/${MC_CORE_LIB_NAME}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/${MC_CORE_LIB_NAME}/*.hpp
)
add_library(${MC_CORE_LIB_NAME} STATIC ${LIB_SOURCES})

target_include_directories(${MC_CORE_LIB_NAME} PUBLIC include)
target_compile_features(${MC_CORE_LIB_NAME} PUBLIC cxx_std_20)
target_link_libraries(${MC_CORE_LIB_NAME} PUBLIC glm::glm)