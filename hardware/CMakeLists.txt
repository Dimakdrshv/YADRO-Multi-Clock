#===================================================================================================
# MIK32 Shared library
#===================================================================================================

add_library(mik32_shared
    mik32v2-shared/runtime/crt0.S
)

target_include_directories(mik32_shared
    PUBLIC
        mik32v2-shared/periphery
        mik32v2-shared/include
        mik32v2-shared/libs
)