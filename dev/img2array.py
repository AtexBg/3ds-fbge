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
    arr, w, h = img2bgrarray(f"gfx/{file}.png") #opens file in "gfx" folder
    formatted = format_array(arr, w, h) #format array
    pyperclip.copy(formatted)  # copy array into clipboard
    print("Array copied in clipboard, press enter to quit...")

if __name__ == "__main__":
    while True:
        main()
