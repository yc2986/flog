cmake_minimum_required (VERSION 3.0)

function(git_pull PROJECT_NAME PROJECT_INSTALL_DIR GIT_REPO)
    ExternalProject_Add(
        ${PROJECT_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJECT_INSTALL_DIR}
    )
endfunction(git_pull)

function(get_target_platform TARGET)
    if (CMAKE_GENERATOR MATCHES "Unix Makefiles")
        set(TARGET_PLATFORM Makefile/)
    elseif (CMAKE_GENERATOR MATCHES "Xcode")
        set(TARGET_PLATFORM Xcode/)
    elseif (MSVC)
        set(TARGET_PLATFORM MSVC/)
    endif ()
    message(${TARGET_PLATFORM} "in function scope")
    set(${TARGET} ${TARGET_PLATFORM} PARENT_SCOPE)
endfunction(get_target_platform)