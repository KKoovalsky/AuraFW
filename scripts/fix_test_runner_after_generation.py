import click
import fileinput


@click.command()
@click.argument('source_file', type=click.Path(exists=True))
def main(source_file):
    rename_main_to_test_main_py(source_file)
    remove_dummy_header_inclusion(source_file)


def rename_main_to_test_main_py(source_file):
    print('Renaming main() to test_main() in file: {}'.format(source_file))
    for line in fileinput.input([source_file], inplace=True):
        print(line.replace('int main(void)', 'int test_main(void)'), end='')


def remove_dummy_header_inclusion(source_file):
    dummy_header_include = '#include "dummy_header.hpp"'
    print('Removing line: "{}" in file: {}'.format(dummy_header_include,
                                                   source_file))
    for line in fileinput.input([source_file], inplace=True):
        print(line.replace(dummy_header_include, ''), end='')


if __name__ == '__main__':
    main()
