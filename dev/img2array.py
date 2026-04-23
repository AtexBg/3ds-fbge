# HOW TO ADD NEW SPRITE ON THE DE:
# - Copy array to include/textures.txt
# - Declare it into src/charset_data.c and include/TOP_render.h

from PIL import Image
import pyperclip

def img2bgrarray(path):
    img = Image.open(path).convert("RGB")
    img = img.rotate(270, expand=True) #Rotate image -90° because the 3DS screen is hardwarely rotated 90°
    width, height = img.size
    pixels = img.load()

    result = []
    for y in range(height):
        row = []
        for x in range(width):
            r, g, b = pixels[x, y]
            row.append(f"{{{b},{g},{r}}}")
        result.append(row)
    return result, width, height

def format_array(array, width, height):
    lines = []
    for row in array:
        line = "    {" + ",".join(row) + "},"
        lines.append(line)
    lines.append("};")
    return "\n".join(lines)
    
def main():
    file = input("enter file name:")
    arr, w, h = img2bgrarray(f"gfx/{file}.png") #opens file in "gfx/font" folder
    formatted = format_array(arr, w, h) #format array
    pyperclip.copy(formatted)  # copy array into clipboard
    print("\x1b[32mArray copied in clipboard.\x1b[0m")

if __name__ == "__main__":
    while True:
        main()
