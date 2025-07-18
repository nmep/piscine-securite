import sys
import exif
from pathlib import Path

def check_mime(file_mime, file_name):
    mimes = [".jpg", ".jpeg", ".png", ".gif", ".bmp"]
    for mime in mimes:
        if (file_mime == mime):
            return True
    raise ValueError(f"{file_name} has not an accepted mime")

def parsing(valid_files):
    file = Path(__file__)    
    args = sys.argv

    if len(args) == 1:
        print("\nthis script need a file")
        exit(1)
    for arg in args:
        if arg == file.name:
            continue
        mime = arg[arg.find('.'):]
        try:
            check_mime(mime, arg)
        except Exception as e:
            print(f"Error: {e}")
            continue
        valid_files.append(arg)

def scorpion(valid_files):
    print("==========exif==========")
    print("==========view==========\n\n")
    for file in valid_files:
        try:
            with open(file, 'rb') as img_file:
                
                print(f"\nImage: {file}\n")
                image = exif.Image(img_file)
                if not image.has_exif:
                    img_file.close()
                    print("The image has no exif data")
                    exit(2)
                list_all = sorted(image.list_all())
                for tag in list_all:
                    print(f"{tag}: {image.get(tag)}")
                img_file.close()
        except Exception as e:
            print(f"Error: {e}")

def get_input(text):
    try:
        inp = input(text)
    except KeyboardInterrupt:
        print("\nError: KeyboardInterrupt detected")
        exit(130)
    except EOFError:
        print("\nExit")
        exit(0)
    return inp

if __name__ == "__main__":
    valid_files = []
    parsing(valid_files)
    scorpion(valid_files)