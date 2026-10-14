import sys 
import time
import random 
import requests

URL_HOME = 'http://127.0.0.1:3490/'
URL_ABOUT = 'http://127.0.0.1:3490/about/'

def log(message):
    print(f"[{sys.argv[1]}]:", message)

def main():
    req_num = random.randint(1, 3)

    start = time.perf_counter()
    if (req_num == 1):
        response = requests.get(random.choice([URL_HOME, URL_ABOUT]), timeout=10) 
    elif (req_num == 2):
        response = requests.head(random.choice([URL_HOME, URL_ABOUT]), timeout=10) 
    else:
        response = requests.options(random.choice([URL_HOME, URL_ABOUT]), timeout=10) 

    elapsed = (time.perf_counter() - start) * 1000
    log(f"Finished in {elapsed:.3f} ms, Status code: {response.status_code}")

if __name__ == '__main__':
    main()