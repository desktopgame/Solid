import mylib

def main():
    for i, o in mylib.resolve_import_files('./rawdata/Models', './assets/Models', '.json', '.json'):
        with open(i, 'rb') as ifp:
            with open(o, 'w', encoding='UTF-8') as ofp:
                ofp.write(ifp.read().decode())


if __name__ == '__main__':
    main()