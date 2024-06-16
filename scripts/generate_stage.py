import json

def gen_FloorRoof(tileSize: float, n: str, y: float, minX: int, maxX: int, minZ: int, maxZ: int):
    li = []
    for ix in range(minX, maxX):
        for iz in range(minZ, maxZ):
            x = ix * tileSize
            z = iz * tileSize
            li.append({
                "x": x,
                "y": y,
                "z": z,
                "r": (x - minX) / (maxX - minX),
                "g": 0.5,
                "b": (z - minZ) / (maxZ - minZ),
                "n": n
            })
    return li

def gen_WallX(tileSize: float, n: str, x: float, minY: int, maxY: int, minZ: int, maxZ: int, offsetX: float, offsetY: float):
    li = []
    for iy in range(minY, maxY):
        for iz in range(minZ, maxZ):
            y = iy * tileSize
            z = iz * tileSize
            li.append({
                "x": x + offsetX,
                "y": y + offsetY,
                "z": z,
                "r": (y - minY) / (maxY - minY),
                "g": 0.5,
                "b": (z - minZ) / (maxZ - minZ),
                "n": n
            })
    return li

def gen_WallZ(tileSize: float, n: str, z: float, minY: int, maxY: int, minX: int, maxX: int, offsetZ: float, offsetY: float):
    li = []
    for iy in range(minY, maxY):
        for ix in range(minX, maxX):
            y = iy * tileSize
            x = ix * tileSize
            li.append({
                "x": x,
                "y": y + offsetY,
                "z": z + offsetZ,
                "r": (y - minY) / (maxY - minY),
                "g": 0.5,
                "b": (z - minX) / (maxX - minX),
                "n": n
            })
    return li


with open('./assets/Stage/Stage01.json', 'w') as fp:
    json.dump({
        'tileSize': 1,
        'tiles': [
            *gen_WallZ(1, "Z+", -10, -1, 10, -10, 10, -0.5, 0.5),
            *gen_WallZ(1, "Z-", 10, -1, 10, -10, 10, -0.5, 0.5),
            *gen_WallX(1, "X+", -10, -1, 10, -10, 10, -0.5, 0.5),
            *gen_WallX(1, "X-", 10, -1, 10, -10, 10, -0.5, 0.5),
            *gen_FloorRoof(1, "Y-", 10, -10, 10, -10, 10),
            *gen_FloorRoof(1, "Y+", -1, -10, 10, -10, 10)
        ]
    }, fp)