#!/usr/bin/env python3
import re
import click
from serial import Serial
import subprocess


@click.command()
@click.option('--port',
              required=True,
              type=str,
              help=('Serial port from which the data will be read from. '
                    'Can be e.g. "/dev/ttyUSB0", or "COM12", or other ...'
                    '8 data bits, no parity and 1 stop bit is assumed (8N1)'))
@click.option('--baud_rate',
              type=int,
              default=115200,
              help='Baud rate for the output read from the PORT.')
@click.option('--pass_regex',
              type=str,
              help=('Regex, which will make this script to return 0, '
                    'if one of the line, received over serial, matches.'))
def drive_device_test(port, baud_rate, pass_regex):
    """Runs device test and checks for pass/fail criteria.

    It will run the device, using openocd, and capture output over serial
    PORT, with specified BAUD_RATE. 8 data bits, no parity and one stop bit
    is assumed (8N1).

    If PASS_REGEX is specified then the logs will be matched against that
    PASS_REGEX. When regex matches then this script returns 0, otherwise it
    returns non-zero exit code.

    If PASS_REGEX is NOT specified, then the script will pass/fail
    according to Unity library output. So the output:

    \b
            10 Tests 0 Failures 1 Ignored

    will make the script return 0 error code, but output:

    \b
            10 Tests 2 Failures 1 Ignored

    will make the script return non-zero error code.

    """
    print(
        'drive_device_test.py: lines read from serial will be echoed to stdout')
    with Serial(port, baud_rate, timeout=10) as serial_input:
        start_device()
        while True:
            line = serial_input.readline().decode()
            if not line:
                raise RuntimeError('Timeout waiting for a line from serial!')
            echo(line.rstrip())
            if pass_regex is None:
                exit_if_tests_finished(line)
            else:
                exit_if_matches(pass_regex, line)


def start_device():
    cmd = [
        'openocd', '-f', 'board/stm32l4discovery.cfg', '-c',
        'init ; reset run ; exit']
    r = subprocess.run(cmd, capture_output=True)
    if r.returncode != 0:
        raise RuntimeError(
            ('Failed to reset (start) device.\r\n'
             'Openocd STDOUT: \r\n{}\r\n'
             'Openocd STDERR: \r\n{}\r\n').format(
                r.stdout.decode(), r.stderr.decode()))


def echo(data):
    print(data)


def exit_if_matches(regex: str, line: str):
    prog = re.compile(regex)
    match = prog.search(line)
    if match:
        exit(0)


def exit_if_tests_finished(line: str):
    unity_finisher_regex = r'\d+ Tests (\d+) Failures \d+ Ignored'
    prog = re.compile(unity_finisher_regex)
    match = prog.search(line)
    if match:
        failures = int(match.group(1))
        if failures != 0:
            raise RuntimeError('ERROR: Found {} failed tests!'.format(failures))
        else:
            exit(0)


if __name__ == '__main__':
    drive_device_test()
