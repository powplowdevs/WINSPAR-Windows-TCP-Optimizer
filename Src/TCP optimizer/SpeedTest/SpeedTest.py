import requests
import time

def download_speed(url):
    start_time = time.time()
    response = requests.get(url)
    end_time = time.time()
    download_time = end_time - start_time
    download_speed = len(response.content) / download_time / 1024 / 1024
    return download_speed

def upload_speed(url, data):
    start_time = time.time()
    response = requests.post(url, data=data)
    end_time = time.time()
    upload_time = end_time - start_time
    upload_speed = len(data) / upload_time / 1024 / 1024
    return upload_speed

def main():
    download_url = "https://ash-speed.hetzner.com/100MB.bin"#Replace with our site
    upload_url = "https://example.com/upload" #Replace with an actual upload URL
    upload_data = b'0' * 100 * 1024 * 1024  #100MB 

    download_speed_result = download_speed(download_url)
    # upload_speed_result = upload_speed(upload_url, upload_data)

    print(f"Download Speed: {download_speed_result:.2f} Mbps")
    # print(f"Upload Speed: {upload_speed_result:.2f} Mbps")

if __name__ == "__main__":
    main()
