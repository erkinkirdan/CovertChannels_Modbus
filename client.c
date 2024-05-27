#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <sys/time.h>

int main() {
    modbus_t *ctx;
    uint16_t tab_reg[32];
    int rc;
    uint8_t unit_ids[8] = {0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44};
    struct timeval start, end;
    double elapsed_time;
    char decoded_sequence[9];

    // Create a new TCP context
    ctx = modbus_new_tcp("127.0.0.1", 502);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    // Connect to the server
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Send 8 read requests with specific unit identifier fields and measure response time
    for (int i = 0; i < 8; i++) {
        modbus_set_slave(ctx, unit_ids[i]);
        gettimeofday(&start, NULL);  // Start timing

        rc = modbus_read_registers(ctx, 0, 1, tab_reg);  // Read register at address 0
        if (rc == -1) {
            fprintf(stderr, "Failed to read registers: %s\n", modbus_strerror(errno));
            modbus_free(ctx);
            return -1;
        }

        gettimeofday(&end, NULL);  // End timing

        // Calculate the elapsed time in milliseconds
        elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0;
        elapsed_time += (end.tv_usec - start.tv_usec) / 1000.0;

        // Decode based on the elapsed time
        if (elapsed_time >= 200) {
            decoded_sequence[i] = '1';
        } else {
            decoded_sequence[i] = '0';
        }

        printf("Request %d sent with Unit Identifier: 0x%X, Response time: %.2f ms, Decoded bit: %c\n", i + 1, unit_ids[i], elapsed_time, decoded_sequence[i]);
    }

    // Null-terminate the decoded sequence
    decoded_sequence[8] = '\0';

    printf("Decoded binary sequence: %s\n", decoded_sequence);

    // Clean up
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
