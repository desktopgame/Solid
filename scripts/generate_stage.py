import json

def gen_XZ(tileSize: float, y: float, minX: int, maxX: int, minZ: int, maxZ: int):
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
            })
    return li


with open('./assets/Stage/Stage01.json', 'w') as fp:
    json.dump({
        'tileSize': 100,
        'tiles': [
            *gen_XZ(100, -1, -10, 10, -20, 20)
        ]
    }, fp)