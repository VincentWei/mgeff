file(GLOB mgeff_header_files "*.h" "${PROJECT_BINARY_DIR}/mgeffconfig.h")
install(FILES ${mgeff_header_files} DESTINATION include/mgeff)
