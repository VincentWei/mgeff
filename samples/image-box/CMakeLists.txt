file(GLOB sources "*.c")
add_executable(image_box ${sources})

add_subdirectory(res)
#install(DIRECTORY res DESTINATION ${PROJECT_BINARY_DIR}/)
