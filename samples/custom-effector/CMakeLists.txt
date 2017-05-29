project(custom-effector)
#file(GLOB sources "*.c")

set(sources customeffector.c)

add_subdirectory(res)

add_executable(customeffector ${sources})

install(DIRECTORY res DESTINATION ${PROJECT_BINARY_DIR}/)
