from PIL import Image

# --- Définis ta palette (valeurs RGB -> code int) ---
palette = {
    (255, 255, 255): 0,  # White
    (0, 0, 0): 1,        # Black
    (255, 0, 0): 3,      # Red
    (0, 0, 255): 7,      # Blue
    (0, 255, 0): 8,      # Green
    (126, 253, 0): 4,    # Fluo green
    (127, 0, 0): 5,      # Brown
    (127, 0, 125): 6,    # Magenta
    (255, 127, 0): 9,    # Orange
    (254, 128, 129): 10, # Pink
    (127, 128, 0): 11,   # Olive
    (255, 255, 0): 12,   # Yellow
    (234, 234, 234): 13, # Gray
    (126, 126, 126): 14, # Darker gray
    (80, 80, 80): 15,    # Silver
}

# Transparence gérée à part (alpha = 0 -> code 2)
TRANSPARENT = 2

def image_to_array(path):
    img = Image.open(path).convert("RGBA")
    img = img.rotate(270, expand=True)
    width, height = img.size
    pixels = img.load()

    result = []
    for y in range(height):
        row = []
        for x in range(width):
            r, g, b, a = pixels[x, y]

            if a == 0:
                code = TRANSPARENT
            else:
                code = palette.get((r, g, b), 1)  # défaut = noir si inconnu
            row.append(code)
        result.append(row)

    return result

def format_c_array(array):
    lines = []
    for row in array:
        line = "{" + ",".join(str(x) for x in row) + "},"
        lines.append(line)
    return "\n".join(lines)

if __name__ == "__main__":
    arr = image_to_array("image.png")
    print(format_c_array(arr))
