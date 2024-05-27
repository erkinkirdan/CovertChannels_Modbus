#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <unistd.h>

int main() {
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int socket;
    int rc;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int bit_index = 0;
    char *bit_sequence = "11011101";

    // Create a new TCP context
    ctx = modbus_new_tcp("127.0.0.1", 502);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    // Create a modbus mapping with 10 holding registers
    mb_mapping = modbus_mapping_new(0, 0, 10, 0);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Initialize holding registers with some values (optional)
    for (int i = 0; i < 10; i++) {
        mb_mapping->tab_registers[i] = i;
    }

    // Listen for incoming connections
    socket = modbus_tcp_listen(ctx, 1);
    if (socket == -1) {
        fprintf(stderr, "Unable to listen for connections\n");
        modbus_mapping_free(mb_mapping);
        modbus_free(ctx);
        return -1;
    }

    while (1) {
        // Accept an incoming connection
        modbus_tcp_accept(ctx, &socket);

        while (1) {
            // Receive the request
            rc = modbus_receive(ctx, query);
            if (rc == -1) {
                // Connection closed or error
                break;
            }

            // Print the unit identifier
            printf("Unit Identifier: 0x%X\n", query[6]);

            // Encode the delay based on the current bit in the sequence
            if (bit_sequence[bit_index] == '1') {
                usleep(300000); // 300 ms delay for '1'
            } else if (bit_sequence[bit_index] == '0') {
                usleep(100000); // 100 ms delay for '0'
            }

            // Increment bit index and wrap around if necessary
            bit_index = (bit_index + 1) % 8;

            // Reply to the request
            modbus_reply(ctx, query, rc, mb_mapping);
        }
    }

    // Clean up
    close(socket);
    modbus_mapping_free(mb_mapping);
    modbus_free(ctx);

    return 0;
}
