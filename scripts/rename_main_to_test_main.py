import click
import fileinput


@click.command()
@click.argument('source_file', type=click.Path(exists=True))
def rename_main_to_test_main_py(source_file):
    print('Renaming main() to test_main() in file: {}'.format(source_file))
    for line in fileinput.input([source_file], inplace=True):
        print(line.replace('int main(void)', 'int test_main(void)'), end='')


if __name__ == '__main__':
    rename_main_to_test_main_py()
