import mylib
import shutil

def main():
    for i, o in mylib.resolve_import_files('./rawdata/Fonts', './assets/Fonts', '.ttf', '.ttf'):
        shutil.copyfile(i, o)


if __name__ == '__main__':
    main()