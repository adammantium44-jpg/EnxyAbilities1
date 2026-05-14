cmake_minimum_required(VERSION 3.21)

# =====================================================
# PROJECT NAME (DLL NAME)
# =====================================================
project(AbilitieMod VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =====================================================
# OUTPUT SYSTEM (JANGAN DIHAPUS - TEMPLATE)
# =====================================================
if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

# =====================================================
# 🔥 ADD COMMONLIBSSE (FIX UTAMA)
# =====================================================
include(FetchContent)

FetchContent_Declare(
    CommonLibSSE
    GIT_REPOSITORY https://github.com/CharmedBaryon/CommonLibSSE.git
    GIT_TAG main
)

FetchContent_MakeAvailable(CommonLibSSE)

# =====================================================
# SKSE PLUGIN BUILD
# =====================================================
add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES
        src/plugin.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

# =====================================================
# AUTO COPY DLL (TEMPLATE SYSTEM - JANGAN DIHAPUS)
# =====================================================
if(DEFINED OUTPUT_FOLDER)

    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<TARGET_FILE:${PROJECT_NAME}>"
        "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_PDB_FILE:${PROJECT_NAME}>"
            "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()

endif()
