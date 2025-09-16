set(MCU_VARIANT mspm0g511x)
set(MCU_SPECIFIC mspm0g5117)
set(JLINK_DEVICE ${MCU_VARIANT})

function(update_board TARGET)
  target_compile_definitions(${TARGET} PUBLIC
    __MSPM0G511x__
    )
endfunction()
