import mylib
import shutil

def main():
    for i, o in mylib.resolve_import_files('./rawdata/Audios', './assets/Audios', '.wav', '.wav'):
        shutil.copyfile(i, o)


if __name__ == '__main__':
    main()