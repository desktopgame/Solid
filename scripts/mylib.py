import pathlib
from typing import Tuple, List


def resolve_import_files(ext: str) -> List[Tuple[pathlib.Path, pathlib.Path]]:
    assert ext is not None or ext[0] == '.'
    rawdata = pathlib.Path('rawdata')
    assets = pathlib.Path('assets')
    ret: List[Tuple[pathlib.Path, pathlib.Path]] = []

    for file in rawdata.glob(f'**/*{ext}'):
        rel = file.relative_to(rawdata)
        asset = assets.joinpath(rel)
        if not asset.parent.exists():
            asset.parent.mkdir(parents=True)
        ret.append((file, asset))
    return ret