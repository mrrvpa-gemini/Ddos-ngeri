# ⚠️ PERINGATAN HUKUM DAN ETIKA

**Alat ini dibuat untuk tujuan edukasi dan pengujian keamanan sistem yang Anda miliki atau memiliki izin tertulis untuk diuji. Penggunaan terhadap sistem tanpa izin merupakan tindak pidana cyber di hampir semua yurisdiksi.**

---

## 📛 DISCLAIMER

Pencipta alat ini (@Rvpa.official) **tidak bertanggung jawab** atas:
- Tindakan kriminal pengguna
- Kerugian materiil atau immateriil
- Tuntutan hukum pidana atau perdata
- Suspensi akun, pemblokiran IP, atau sanksi penyedia layanan
- Konsekuensi apapun yang timbul dari penyalahgunaan

**Pengguna 100% bertanggung jawab penuh atas setiap eksekusi.**

---

## 🎯 TUJUAN

- Pembelajaran protokol HTTP/HTTPS
- Pemahaman mekanisme DDoS mitigation
- Pengujian beban pada infrastruktur milik sendiri
- Demonstrasi teoretis serangan L7

---

## ⚖️ LEGALITAS

| Yurisdiksi | Hukuman |
|------------|---------|
| Indonesia (UU ITE No. 11/2008) | 8 tahun penjara / Rp 800 juta |
| Amerika Serikat (CFAA) | 10 tahun penjara |
| Inggris (Computer Misuse Act) | 10 tahun penjara |
| Uni Eropa (NIS Directive) | Sanksi administratif + pidana |
| Singapura (Cybersecurity Act) | 10 tahun penjara / SGD 100.000 |

---

## 🔒 SYARAT PENGGUNAAN

1. Target harus **milik sendiri** atau **izin tertulis** dari pemilik
2. Dokumentasi izin harus tersedia jika diminta
3. Tidak untuk target komersial, pemerintah, atau infrastruktur kritis
4. Tidak untuk target dengan proteksi Cloudflare/AWS/Google tanpa kontrak penetration testing
5. Pengguna harus berusia 18+ dan memahami risiko hukum

---

## 🚫 LARANGAN MUTLAK

- Menggunakan terhadap situs pemerintah
- Menggunakan terhadap fasilitas kesehatan
- Menggunakan terhadap infrastruktur keuangan
- Menggunakan terhadap sekolah/universitas tanpa izin
- Menggunakan untuk pemerasan (ransom DDoS)
- Menggunakan untuk kompetisi ilegal (stresser/booter)
- Menjual, menyewakan, atau mendistribusikan sebagai layanan serangan

---

## 🛡️ BAGAIMANA PROTEKSI BEKERJA

Alat ini mengirimkan request HTTP/HTTPS dengan header acak. Target modern dengan:
- CDN (Cloudflare, Akamai, Fastly)
- WAF (AWS WAF, ModSecurity)
- Rate limiting
- Anycast networking

**akan menahan serangan dari node tunggal.** Serangan efektif memerlukan:
- Distributed network (botnet / IoT)
- Amplification (DNS/NTP/SSDP)
- L7 precision (request berat, slowloris)
- Proxy rotation (residential IP)

Node tunggal = noise. Bukan threat.

---

## 📊 TEKNIS

| Parameter | Nilai |
|-----------|-------|
| Bahasa | Python 3 |
| Dependensi | Standard library only |
| Threading | `threading.Thread` |
| Delay | 1 detik per thread (configurable) |
| Protokol | TCP HTTP/HTTPS |
| SSL | `CERT_NONE` (self-signed bypass) |
| Output | Terminal ASCII art + counter |

---

## 🧪 CONTOH PENGGUNAAN LEGAL

```bash
# Test pada server lokal milik sendiri
python3 rvpa.py
# Thread : 10
# Target : http://localhost:8080
# Delay  : 1

# Test pada VPS sendiri
python3 rvpa.py
# Thread : 50
# Target : https://vps-anda.com
# Delay  : 1
