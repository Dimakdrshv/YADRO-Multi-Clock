set(TOOLCHAIN_PREFIX $ENV{MIK32_TOOLCHAIN_DIR}/riscv-none-elf)

set(CMAKE_C_COMPILER            ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_ASM_COMPILER          ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER          ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_C_COMPILER_AR         ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_C_COMPILER_RANLIB     ${TOOLCHAIN_PREFIX}-ranlib)
set(CMAKE_CXX_COMPILER_AR       ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_CXX_COMPILER_RANLIB   ${TOOLCHAIN_PREFIX}-ranlib)
set(CMAKE_AR                    ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_RANLIB                ${TOOLCHAIN_PREFIX}-ranlib)

string(CONCAT COMMON_FLAGS
    " -march=rv32imc_zicsr_zifencei"
    " -mabi=ilp32"
    " -mcmodel=medlow"
    " -g3"
    " -Wall"
    " -fsigned-char -ffunction-sections"
    " -DMIK32V2"
)


string(CONCAT C_FLAGS
    ${COMMON_FLAGS}
    " -std=gnu11"
)

string(CONCAT CPP_FLAGS
    ${COMMON_FLAGS}
    " -std=gnu++17"
    " -fabi-version=0"
    " -fno-rtti -fno-exceptions"
    " -fno-use-cxa-atexit -fno-threadsafe-statics"
)

string(CONCAT ASM_FLAGS
    ${COMMON_FLAGS}
    " -x assembler-with-cpp"
)

set(CMAKE_C_FLAGS_INIT   ${C_FLAGS})
set(CMAKE_CPP_FLAGS_INIT ${CPP_FLAGS})
set(CMAKE_ASM_FLAGS_INIT ${ASM_FLAGS})

string(CONCAT LINK_FLAGS
    " -Wl,-Map,base_project.map"
    " -Tram.ld"
    " -Xlinker --gc-sections"
    " -nostartfiles"
)

set(CMAKE_EXE_LINKER_FLAGS ${LINK_FLAGS})

set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)