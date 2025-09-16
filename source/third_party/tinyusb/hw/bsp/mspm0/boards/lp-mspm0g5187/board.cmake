set(MCU_VARIANT mspm0g518x)
set(MCU_SPECIFIC mspm0g5187)
set(JLINK_DEVICE ${MCU_VARIANT})

function(update_board TARGET)
  target_compile_definitions(${TARGET} PUBLIC
    __MSPM0G518x__
    )
endfunction()
