from PIL import Image
import pyperclip

def img2bitarray(path):
    img = Image.open(path).convert("RGB")
    img = img.rotate(270, expand=True)  # -90° for 3DS screen
    width, height = img.size
    pixels = img.load()

    result = []
    for y in range(height):
        row = []
        for x in range(width):
            r, g, b = pixels[x, y]
            bit = 1 if (r or g or b) else 0
            row.append(str(bit))
        result.append(row)

    return result, width, height

def format_array(array):
    lines = []
    for row in array:
        lines.append("    {" + ",".join(row) + "},")
    lines.append("};")
    return "\n".join(lines)

def main():
    file = input("enter file name: ")
    arr, w, h = img2bitarray(f"gfx/font/{file}.png")

    output = f"uint8_t {file}[{h}][{w}] = {{\n"
    output += format_array(arr)

    pyperclip.copy(output)
    print("\x1b[32m1-bit font array copied to clipboard.\x1b[0m")

if __name__ == "__main__":
    while True:
        main()
