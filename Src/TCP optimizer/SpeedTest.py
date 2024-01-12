import requests

url = "http://localhost:3000/speed-test"
try:
    response = requests.get(url)
    data = response.json()

    print("Speed Test Results:")
    print(f"Speed in Bps: {data['speedBps']}")
    print(f"Speed in Kbps: {data['speedKbps']}")
    print(f"Speed in Mbps: {data['speedMbps']}")

except requests.RequestException as e:
    print(f"Error: {e}")
