from PIL import Image
import sys

fname = 'espacio.png'
img = Image.open(fname)

if img.width != 128 or img.height != 160:
    print("Error: 128x160 image expected")
    sys.exit(2)

# Convertir la imagen a modo RGB si no está en ese modo
if img.mode != 'RGB':
    img = img.convert('RGB')

print("const uint16_t marlene[][128] = {");

for y in range(0, img.height):
    s = "{"
    for x in range(0, img.width):
        (r, g, b) = img.getpixel((x, y))
        color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)
        color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8)
        s += "0x{:04X},".format(color565)
    s += "},"
    print(s)

print("};")
