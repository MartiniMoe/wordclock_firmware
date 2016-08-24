#!/usr/bin/env python
import sys
import PIL.Image

out_name = sys.argv[1].split(".")[0] + ".h"
image = PIL.Image.open(sys.argv[1])
image_data = image.getdata()

with open(out_name, "w") as out:
    out.write("#ifndef __" + sys.argv[1].split(".")[0].upper() + "_H__\n")
    out.write("#define __" + sys.argv[1].split(".")[0].upper() + "_H__\n")
    out.write("\n")
    out.write("char bootLogo_" + sys.argv[1].split(".")[0].upper() + "[10][11][3] = {\n")

    not_ordered = []

    i = 0

    for pixel in image_data:
        if i == 0:
            i = 11
            not_ordered.append([])
            not_ordered[-1] = ""

        i -= 1

        not_ordered[-1] += str(pixel[0]) + ", "
        not_ordered[-1] += str(pixel[1]) + ", "
        not_ordered[-1] += str(pixel[2]) + ", "

    for line in reversed(not_ordered):
        out.write(line + "\n")

    out.write("\n};\n")
    out.write("\n")
    out.write("#endif")
