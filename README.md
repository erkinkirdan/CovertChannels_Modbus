# Covert Channels in Modbus

This repository contains two C programs that demonstrate covert channels in a simple client-server communication using the Modbus TCP protocol.

## Requirements

- C compiler (e.g., `gcc`)
- libmodbus (a library for Modbus protocol implementation)

## Installation

1. **Install libmodbus**:
    ```sh
    sudo apt-get install libmodbus-dev
    ```

2. **Compile the programs**:
    ```sh
    gcc -o client client.c -lmodbus
    gcc -o server server.c -lmodbus
    ```

## Usage

1. **Start the Modbus TCP server**:
    ```sh
    ./server
    ```

2. **Start the Modbus TCP client**:
    ```sh
    ./client
    ```
