import os
import time

FileList = []

def process_file(file_path):
    
    try:
        file_size = os.path.getsize(file_path)
        FileList.append([file_path, file_size])
    except Exception as e:
        print(f"Error processing file {file_path}: {e}")

def process_directory(directory_path):
    try:
        for root, dirs, files in os.walk(directory_path):
            for file_name in files:
                file_path = os.path.join(root, file_name)
                process_file(file_path)
    except Exception as e:
        print(f"Error processing directory {directory_path}: {e}")

def main():
    root_path = "C:/" 

    if os.path.exists(root_path) and os.path.isdir(root_path):
        process_directory(root_path)
    else:
        print(f"Invalid directory: {root_path}")

if __name__ == "__main__":
    start = time.time()
    main()
    end = time.time()
    print("Indexing all sotrage took: " + str(end-start) + " seconds")
    print(len(FileList))