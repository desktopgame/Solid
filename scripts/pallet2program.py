from PIL import Image, ImageFilter

im = Image.open('assets/pallete.png')
for i in range(0, 64):
    x = (i * 32) + 16
    y = 16
    col = im.getpixel((x, y))
    r = col[0] / 255
    g = col[1] / 255
    b = col[2] / 255
    print(f'Math::Vector4({{{r}f, {g}f, {b}f, 1.0f  }}),')
