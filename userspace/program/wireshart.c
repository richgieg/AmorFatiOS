#include <program/wireshart.h>
#include <sys.h>

void wireshart(void) {
    sys_net_subscribe();

    u8 data[2048];
    int count = 0;
    ssize_t bytes_read;

    sys_console_puts("Waiting for frames...\n\n");

    while (true) {
        bytes_read = sys_net_read_frame(data, sizeof(data));

        sys_console_puts("********************************************************************************");

        sys_console_puts("Frame #");
        sys_console_putdw(++count);
        sys_console_putc('\n');

        sys_console_puts("Length: ");
        sys_console_putdw(bytes_read);
        sys_console_puts("\n");

        sys_console_puts("Data: ");

        for (int i = 0; i < bytes_read; i++) {
            sys_console_putb(data[i]);
            sys_console_putc(' ');
        }
        sys_console_puts("\n\n");
    }

    sys_process_exit();
}
