include(FetchContent)
FetchContent_Declare(
        kissat
        GIT_REPOSITORY https://github.com/arminbiere/kissat.git
        GIT_TAG rel-3.1.1
)
FetchContent_MakeAvailable(kissat)

add_custom_command(
        COMMAND "./configure"
        COMMAND "make"
        COMMAND "cp" "build/kissat" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY "${kissat_SOURCE_DIR}"
        OUTPUT "${CMAKE_BINARY_DIR}/kissat"
)
