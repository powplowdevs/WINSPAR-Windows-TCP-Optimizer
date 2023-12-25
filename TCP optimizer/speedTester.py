import speedtest

def speed_test():
    st = speedtest.Speedtest(secure=True)
    download_speed = st.download() / 1_000000
    return download_speed

if __name__ == "__main__":
    result = speed_test()
    print(f"Download speed: {result:.2f} Mbps")