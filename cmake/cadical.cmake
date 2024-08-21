include(FetchContent)
FetchContent_Declare(
        cadical
        GIT_REPOSITORY https://github.com/arminbiere/cadical.git
        GIT_TAG rel-2.0.0
)
FetchContent_MakeAvailable(cadical)

add_custom_command(
        COMMAND "./configure"
        COMMAND "make"
        COMMAND "cp" "build/cadical" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY "${cadical_SOURCE_DIR}"
        OUTPUT "${CMAKE_BINARY_DIR}/cadical"
)
