#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

void wait(int ticks)
{
    for (int i = 0; i < ticks; i++);
}

/* Keypad: PM0-PM3 = X1-X4 output, PM4-PM7 = Y1-Y4 input */
void configure_keypad()
{
    SYSCTL_RCGCGPIO_R |= (1 << 11);          // Port M clock
    while (!(SYSCTL_PRGPIO_R & (1 << 11)));

    GPIO_PORTM_DEN_R |= 0xFF;
    GPIO_PORTM_DIR_R |= 0x0F;                // PM0-PM3 output
    GPIO_PORTM_DIR_R &= ~0xF0;               // PM4-PM7 input
    GPIO_PORTM_PUR_R |= 0xF0;                // pull-up rows
    GPIO_PORTM_DATA_R |= 0x0F;               // all columns high
}

/* Display: PK0-PK3 = D0-D3 output */
void configure_display()
{
    SYSCTL_RCGCGPIO_R |= (1 << 9);           // Port K clock
    while (!(SYSCTL_PRGPIO_R & (1 << 9)));

    GPIO_PORTK_DEN_R |= 0x0F;                // PK0-PK3 digital enable
    GPIO_PORTK_DIR_R |= 0x0F;                // PK0-PK3 output
    GPIO_PORTK_DATA_R &= ~0x0F;              // initially display 0
}

char read_keypad()
{
    char keymap[4][4] =
    {
        {'1', '2', '3', 'F'},
        {'4', '5', '6', 'E'},
        {'7', '8', '9', 'D'},
        {'A', '0', 'B', 'C'}
    };

    int count = 0;
    char key = 0;

    for (int col = 0; col < 4; col++)
    {
        GPIO_PORTM_DATA_R |= 0x0F;           // all columns HIGH
        GPIO_PORTM_DATA_R &= ~(1 << col);    // selected column LOW

        wait(1000);

        int rows = (GPIO_PORTM_DATA_R & 0xF0) >> 4;

        for (int row = 0; row < 4; row++)
        {
            if ((rows & (1 << row)) == 0)
            {
                count++;
                key = keymap[row][col];
            }
        }
    }

    GPIO_PORTM_DATA_R |= 0x0F;

    if (count == 0)
        return 0;

    if (count > 1)
        return 'X';

    return key;
}

int hex_value(char key)
{
    if (key >= '0' && key <= '9')
        return key - '0';

    if (key >= 'A' && key <= 'F')
        return key - 'A' + 10;

    return 0;
}

void display_key(char key)
{
    int value = hex_value(key);              // A=10, B=11, ..., F=15

    GPIO_PORTK_DATA_R &= ~0x0F;              // clear old display value
    GPIO_PORTK_DATA_R |= value;              // send new 4-bit value
}

int main(void)
{
    configure_keypad();
    configure_display();

    char last_key = 0;

    while (1)
    {
        char current_key = read_keypad();

        if (current_key != 0 && last_key == 0)
        {
            if (current_key == 'X')
            {
                printf("Error: Multiple keys pressed\n");
            }
            else
            {
                printf("Pressed key: %c\n", current_key);
                display_key(current_key);
            }

            last_key = current_key;
        }

        if (current_key == 0)
        {
            last_key = 0;
        }

        wait(50000);
    }
}
