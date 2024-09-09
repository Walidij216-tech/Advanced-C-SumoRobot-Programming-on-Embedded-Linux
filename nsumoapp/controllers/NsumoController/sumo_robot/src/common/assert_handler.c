#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/assert_handler.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/common/defines.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/src/drivers/uart.h"
#include "/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/controllers/NsumoController/sumo_robot/external/printf/printf.h"
#include "/home/walid/Desktop/msp430-gcc/lib/gcc/msp430-elf/9.3.1/plugin/include/config/msp430/msp430.h"
#include <stdio.h>

/* The TI compiler provides intrinsic support for calling a specific opcode, which means
 * you can write __op_code(0x4343) to trigger a software breakpoint (when LAUNCHPAD FET
 * debugger is attached). MSP430-GCC does not have this intrinsic, but 0x4343 corresponds
 * to assembly instruction "CLR.B R3". */
#define BREAKPOINT __asm volatile("CLR.B R3");

// Text + Program counter + Null termination
#define ASSERT_STRING_MAX_SIZE (15u + 6u + 1u)

#define GPIO_OUTPUT_LOW(port, bit) \
    do { \
        if ((port) == 1) { \
            P1SEL &= ~(BIT##bit); \
            P1SEL2 &= ~(BIT##bit); \
            P1DIR |= BIT##bit; \
            P1REN &= ~(BIT##bit); \
            P1OUT &= ~(BIT##bit); \
        } else if ((port) == 2) { \
            P2SEL &= ~(BIT##bit); \
            P2SEL2 &= ~(BIT##bit); \
            P2DIR |= BIT##bit; \
            P2REN &= ~(BIT##bit); \
            P2OUT &= ~(BIT##bit); \
        } else { \
            /* Port non valide */ \
        } \
    } while (0)

static void assert_trace(uint16_t program_counter)
{
    // UART Tx
    P1SEL |= BIT2;
    P1SEL2 |= BIT2;
    uart_init_assert();
    char assert_string[ASSERT_STRING_MAX_SIZE];
    snprintf(assert_string, sizeof(assert_string), "ASSERT 0x%x\n", program_counter);
    uart_trace_assert(assert_string);
}

static void assert_blink_led(void)
{
    GPIO_OUTPUT_LOW(1, 0); // Test LED (Launchpad)
    GPIO_OUTPUT_LOW(2, 6); // Test LED (Nsumo)
    while (1) {
        // Blink LED on both targets in case the wrong target was flashed
        P1OUT ^= BIT0;
        P2OUT ^= BIT6;
        BUSY_WAIT_ms(250);
    };
}

// Stop the motors when something unexpected happens (assert) to prevent damage
static void assert_stop_motors(void)
{
    GPIO_OUTPUT_LOW(2, 6); // Left PWM (Launchpad)
    GPIO_OUTPUT_LOW(2, 1); // Left CC1 (Launchpad)
    GPIO_OUTPUT_LOW(2, 2); // Left CC2 (Launchpad)
    GPIO_OUTPUT_LOW(2, 4); // Left CC2 (Nsumo)
    GPIO_OUTPUT_LOW(2, 5); // Left CC1 (Nsumo)
    GPIO_OUTPUT_LOW(2, 7); // Right CC2 (Nsumo)
    GPIO_OUTPUT_LOW(3, 7); // Right CC1 (Nsumo)
    GPIO_OUTPUT_LOW(3, 5); // Left PWM (Nsumo)
    GPIO_OUTPUT_LOW(3, 6); // Right PWM (Nsumo)
}

/* Minimize code dependency in this function to reduce the risk of accidently calling
 * a function with an assert in it, which would cause the assert_handler to be called
 * recursively until stack overflow. */
void assert_handler(uint16_t program_counter)
{
    assert_stop_motors();
    BREAKPOINT
    assert_trace(program_counter);
    assert_blink_led();
}
