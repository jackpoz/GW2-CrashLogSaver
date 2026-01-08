# Clone the repository with submodules
set(GIT_TAG "0.12.2")
find_program(GIT NAMES git git.cmd)
set(SOURCE_PATH "${CURRENT_BUILDTREES_DIR}/src/${GIT_TAG}")

if(NOT EXISTS "${SOURCE_PATH}")
    file(MAKE_DIRECTORY "${CURRENT_BUILDTREES_DIR}/src")

    vcpkg_execute_required_process(
        COMMAND "${GIT}" clone --branch ${GIT_TAG} --depth 1 "https://github.com/getsentry/sentry-native.git" "${SOURCE_PATH}"
        WORKING_DIRECTORY "${CURRENT_BUILDTREES_DIR}/src"
        LOGNAME "git-clone"
    )

    vcpkg_execute_required_process(
        COMMAND "${GIT}" submodule update --init --recursive
        WORKING_DIRECTORY "${SOURCE_PATH}"
        LOGNAME "git-submodule-update"
    )
endif()

set(VCPKG_BUILD_TYPE release)
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_SHARED_LIBS=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup()
