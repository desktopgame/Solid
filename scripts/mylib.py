import pathlib
from typing import Tuple, List


def resolve_import_files(input_dir: str, output_dir: str, ext: str, new_ext: str) -> List[Tuple[pathlib.Path, pathlib.Path]]:
    assert ext is not None or ext[0] == '.'
    assert new_ext is not None or new_ext[0] == '.'
    rawdata = pathlib.Path(input_dir)
    assets = pathlib.Path(output_dir)
    ret: List[Tuple[pathlib.Path, pathlib.Path]] = []

    if not assets.exists():
        assets.mkdir(parents=True)

    for file in rawdata.glob(f'**/*{ext}'):
        rel = file.relative_to(rawdata)
        asset = assets.joinpath(rel)
        asset = pathlib.Path(asset.parent).joinpath(f'{asset.stem}{new_ext}')
        if not asset.parent.exists():
            asset.parent.mkdir(parents=True)
        ret.append((file, asset))
    return ret