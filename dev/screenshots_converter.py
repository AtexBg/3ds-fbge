import os
from PIL import Image

WIDTH = 240
HEIGHT = 400

def bin_to_png(bin_path, png_path):
    with open(bin_path, "rb") as f:
        data = f.read()

    expected_size = WIDTH * HEIGHT * 3
    if len(data) != expected_size:
        print(f"[size of {bin_path} = {len(data)}, expected {expected_size}")
    
    img = Image.new("RGB", (WIDTH, HEIGHT))
    pixels = img.load()

    for y in range(HEIGHT):
        for x in range(WIDTH):
            i = (y * WIDTH + x) * 3
            b = data[i]
            g = data[i+1]
            r = data[i+2]
            pixels[x, y] = (r, g, b)
    img = img.rotate(90, expand=True)
    img.save(png_path)
    print(f"[+] Converti {bin_path} → {png_path}")

def process_folder(input, output):
    if not os.path.exists(output):
        os.makedirs(output)

    for fname in os.listdir(input):
        if fname.endswith(".bin"):
            bin_path = os.path.join(input, fname)
            png_path = os.path.join(output, f"{os.path.splitext(fname)[0]}.png")
            bin_to_png(bin_path, png_path)

if __name__ == "__main__":
    input = "screenshots" #input folder with .bin files
    output = "screenshots_png"#output folder
    process_folder(input, output)
