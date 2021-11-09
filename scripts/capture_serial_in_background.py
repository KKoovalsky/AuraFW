#!/usr/bin/env python3
import serial
import click
from queue import Queue
from threading import Thread
import asyncio
import logging


@click.command()
@click.argument('action',
                type=click.Choice(['start', 'stop', 'restart', 'status']))
@click.option('--pid_file',
              type=click.Path(),
              default='/var/log/capture_serial_in_background.pid',
              help=('Path to PID file for the background process. '
                    'Use it when you want to run multiple instances, or'
                    'gain control over PID file location.'))
@click.option('--input_port',
              required=True,
              type=str,
              help=('Serial port from which the data will be read from. '
                    'Can be "/dev/ttyUSB0", or "COM12"'
                    '8 data bits, no parity and 1 stop bit is assumed (8N1)'))
@click.option('--output_port',
              required=True,
              type=int,
              help=('Network socket, created on localhost, '
                    'to which the data will be written to.'))
@click.option('--baud_rate',
              type=int,
              default=115200,
              help='Baud rate for the output read from OUTPUT_PORT.')
def capture_serial_in_background(
        action, pid_file, input_port, output_port, baud_rate):
    """Captures serial port in the background which can be read over TCP.

    This will capture the output from INPUT_PORT and copy it to the OUTPUT_PORT
    in the background. If there is output collected before client connected
    to the server, they will be written to the OUTPUT_PORT after connection.

    EXAMPLE:

    \b
        ./capture_serial_in_background.py       \\
            start                               \\
            --output_port 9199                  \\
            --input_port /dev/ttyUSB0

    Then read the file over the socket (blocking):

    \b
        ./read_file_over_socket.py --port 9199

    After you are finished the server can be closed with:

    \b
        ./capture_serial_in_background.py stop


    To run multiple instances, use PID file:

    \b
        ./capture_serial_in_background.py       \\
            start                               \\
            --pid_file /some/path/1             \\
            --output_port 9199                  \\
            --input_port /dev/ttyUSB0
        ./capture_serial_in_background.py       \\
            stop                                \\
            --pid_file /some/path/1

    """
    forwarder = SerialToDataStreamServerForwarder(
        input_port, output_port, baud_rate)
    # daemon = Daemon(worker=forwarder.run,
    #                 pid_file=pid_file)
    # daemon.do_action(action)
    forwarder.run()


class SerialToDataStreamServerForwarder:

    def __init__(self, input_port, output_port, baud_rate):
        self._input_port = input_port
        self._output_port = output_port
        self._baud_rate = baud_rate

    def run(self):

        init_logging()

        logging.info('Hello!')

        logging.info('Creating data stream server.')
        background_server = DataStreamServer(self._output_port)

        def _run_background_server():
            nonlocal background_server
            background_server.run()

        logging.info('Creating background server thread.')
        background_server_thread = Thread(target=_run_background_server)

        logging.info('Going to start background server ...')
        background_server_thread.start()
        logging.info('Started background server!')

        logging.info('Going to open serial connection ...')
        with serial.Serial(self._input_port, self._baud_rate, timeout=10) \
                as input_:
            logging.info('Opened serial connection!')
            while True:
                line = input_.readline()
                if line:
                    background_server.send(line)


def init_logging():
    logging.basicConfig(filename='capture_serial_in_background.log',
                        level=logging.DEBUG)


class DataStreamServer:

    def __init__(self, port: int):
        self._port = port
        self._queue = Queue()

    def run(self):
        asyncio.run(self._run_server())

    async def _run_server(self):
        server = await asyncio.start_server(self._handle_client_connection,
                                            host='127.0.0.1',
                                            port=self._port,
                                            limit=1)

        async with server:
            await server.serve_forever()

    async def _handle_client_connection(self,
                                        reader: asyncio.StreamReader,
                                        writer: asyncio.StreamWriter):
        while True:
            message = self._queue.get()
            if isinstance(message, _TerminateMessage):
                writer.close()
                return

            data = message
            writer.write(data)
            await writer.drain()

    def send(self, data: bytes):
        self._queue.put(data)


class _TerminateMessage:
    pass


if __name__ == '__main__':
    capture_serial_in_background()
