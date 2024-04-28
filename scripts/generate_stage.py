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

def gen_WallX(tileSize: float, n: str, x: float, minY: int, maxY: int, minZ: int, maxZ: int):
    li = []
    for iy in range(minY, maxY):
        for iz in range(minZ, maxZ):
            y = iy * tileSize
            z = iz * tileSize
            li.append({
                "x": x,
                "y": y,
                "z": z,
                "r": (y - minY) / (maxY - minY),
                "g": 0.5,
                "b": (z - minZ) / (maxZ - minZ),
                "n": n
            })
    return li

def gen_WallZ(tileSize: float, n: str, z: float, minY: int, maxY: int, minX: int, maxX: int):
    li = []
    for iy in range(minY, maxY):
        for ix in range(minX, maxX):
            y = iy * tileSize
            x = ix * tileSize
            li.append({
                "x": x,
                "y": y,
                "z": z,
                "r": (y - minY) / (maxY - minY),
                "g": 0.5,
                "b": (z - minX) / (maxX - minX),
                "n": n
            })
    return li


with open('./assets/Stage/Stage01.json', 'w') as fp:
    json.dump({
        'tileSize': 10,
        'tiles': [
            *gen_WallZ(10, "Z+", -200, -1, 11, -20, 20),
            *gen_WallZ(10, "Z-", 200, -1, 11, -20, 20),
            *gen_WallX(10, "X+", -100, -1, 11, -20, 20),
            *gen_WallX(10, "X-", 100, -1, 11, -20, 20),
            *gen_FloorRoof(10, "Y-", 10 * 10, -10, 10, -20, 20),
            *gen_FloorRoof(10, "Y+", -1, -10, 10, -20, 20)
        ]
    }, fp)