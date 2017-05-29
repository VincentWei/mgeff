project(easing)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources "*.c" "src/*.c")

add_executable(easing ${sources})
install(DIRECTORY res DESTINATION ${PROJECT_BINARY_DIR}/)

