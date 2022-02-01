import pypag

def write_bmp(width, height, data):
    header = bytearray(
        [0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0,
            0,    0, 0, 0, 0, 0, 0, 0, 1, 0, 32, 0, 0, 0, 0,  0, 0, 0,
            0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0
        ])
    filesize = width * height * 4 + 54;
    header[2] = (filesize & 0xFF)
    header[3] = ((filesize >> 8) & 0xFF)
    header[4] = ((filesize >> 16) & 0xFF)
    header[5] = ((filesize >> 24) & 0xFF)

    header[18] = (width & 0xFF)
    header[19] = ((width >> 8) & 0xFF)
    header[20] = ((width >> 16) & 0xFF)
    header[21] = ((width >> 24) & 0xFF)

    h = -height
    header[22] = (h & 0xFF)
    header[23] = ((h >> 8) & 0xFF)
    header[24] = ((h >> 16) & 0xFF)
    header[25] = ((h >> 24) & 0xFF)

    with open("result.bmp", "wb") as f:
        f.write(header)
        f.write(data)


f = pypag.PAGFile.load("../test2.pag")
font = pypag.PAGFont.register_font("../NotoSerifSC-Regular.otf", 0)
textDocumentHandle = f.get_text_data(0)
textDocumentHandle.font_family = font.font_family
textDocumentHandle.font_style = font.font_style
f.replace_text(0, textDocumentHandle)
surface = pypag.PAGSurface.make_offscreen(f.width(), f.height())
player = pypag.PAGPlayer()
player.surface = surface
player.composition = f
player.progress = 0.9
player.flush()
buf = bytearray(f.width() * f.height() * 4)
surface.read_pixels(pypag.ColorType.BGRA_8888, pypag.AlphaType.Premultiplied, buf)
write_bmp(f.width(), f.height(), buf)


