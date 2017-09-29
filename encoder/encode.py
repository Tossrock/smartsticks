from PIL import Image
from argparse import ArgumentParser, FileType
import sys

def getargs():
    """ Get and parse arguments """
    parser = ArgumentParser("A tool to encode images as c byte arrays.")
    parser.add_argument("-i", "--input", nargs="+", help="The input image(s)")
    parser.add_argument("-o", "--output", type=FileType("w"), nargs="?", default=sys.stdout, help="The output stream. Defaults to stdout.")
    return parser.parse_args()

def fmtbyte(b):
    """ Format a number 0-255 => 0x00 - 0xFF """
    return "{0:#0{1}X}".format(b, 4)

def writepixel(p, stream):
    """ Writes a given pixel to the byte array. """
    pixeltmpl = "{r}, {g}, {b},  "
    pixel = pixeltmpl.format(r=fmtbyte(p[0]), g=fmtbyte(p[1]), b=fmtbyte(p[2]))
    stream.write(pixel)

def writeheader(name, stream):
    """ Writes the header for a byte array. """
    headertmpl = """const uint8_t PROGMEM pixels_{name} = """
    stream.write(headertmpl.format(name=name)+"{")

def writeimage(name, stream):
    """ Writes a given image to a byte array. """
    image = Image.open(name)
    image.pixels = image.load()
    imagename = name.split(".")[0]
    writeheader(imagename, stream)
    for x in range(image.size[0]):
        stream.write("\n")
        for y in range(image.size[1]):
            pixel = image.pixels[x,y]
            writepixel(pixel, stream)
    stream.write("};")
            
 
def main():
    args = getargs()
    print args
    for imagepath in args.input:
        writeimage(imagepath, args.output)
    return 0

if __name__=="__main__":
    sys.exit(main())
