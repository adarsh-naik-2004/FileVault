import os
import random
import string

def generate_random_text(size=256):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))

def create_test_files(folder="test", num_files=50, file_size=256):
    os.makedirs(folder, exist_ok=True)
    
    for i in range(1, num_files + 1):
        file_path = os.path.join(folder, f"test{i}.txt")
        with open(file_path, "w") as f:
            f.write(generate_random_text(file_size))
    
    print(f"{num_files} files created in '{folder}' folder.")

if __name__ == "__main__":
    create_test_files()
