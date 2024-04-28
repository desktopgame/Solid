import json

def gen_XZ(y: float, minX: int, maxX: int, minZ: int, maxZ: int):
    li = []
    for ix in range(minX, maxX):
        for iz in range(minZ, maxZ):
            x = ix * 5
            z = iz * 5
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
        'tiles': [
            *gen_XZ(-1, -10, 10, -20, 100)
        ]
    }, fp)