set(MCU_DEFINITIONS
    STM32G474xx
)

set(MCU_COMPILE_OPTIONS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

set(MCU_LINK_OPTIONS
    ${MCU_COMPILE_OPTIONS}

    -nostartfiles
    -specs=nano.specs
    -specs=nosys.specs
)

set(MCU_LINKER_SCRIPT
    "${CMAKE_SOURCE_DIR}/linker/STM32G474RETX_FLASH.ld"
)

set(MCU_STARTUP_FILE
    "${CMAKE_SOURCE_DIR}/startup/startup_stm32g474xx.s"
)