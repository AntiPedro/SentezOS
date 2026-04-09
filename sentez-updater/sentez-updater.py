#!/usr/bin/env python3
import requests
import os
import subprocess
import tarfile
import sys

# SentezOS GitHub OTA (Over-The-Air) Güncelleme Sistemi
GITHUB_REPO = "SentezOS/SentezCore" # (Kendi GitHub repo ismini buraya yazacaksın)
CURRENT_VERSION_FILE = "/etc/sentez-version"

def get_current_version():
    if os.path.exists(CURRENT_VERSION_FILE):
        with open(CURRENT_VERSION_FILE, "r") as f:
            return f.read().strip()
    return "v1.0.0"

def perform_update(download_url, version):
    print(f"🚀 SentezOS güncelleniyor: {version} indiriliyor...")
    tar_path = "/tmp/sentez-update.tar.gz"
    
    # 1. Update paketini indir
    response = requests.get(download_url, stream=True)
    with open(tar_path, "wb") as f:
        for chunk in response.iter_content(chunk_size=8192):
            f.write(chunk)
            
    # 2. Paketi aç ve kurulum komutunu (bash script) çalıştır
    print("📦 Paket açılıyor...")
    with tarfile.open(tar_path, "r:gz") as tar:
        tar.extractall(path="/tmp/sentez_update")
        
    print("⚙️ Sistem yamaları (Kernel/UI) uygulanıyor...")
    # Örnek: Update klasörünün içindeki install.sh'yi çalıştır
    update_script = "/tmp/sentez_update/apply_update.sh"
    if os.path.exists(update_script):
        subprocess.run(["bash", update_script])
    
    # Yeni versiyonu kaydet
    with open(CURRENT_VERSION_FILE, "w") as f:
        f.write(version)
    print("✅ SentezOS Başarıyla Güncellendi! Sisteminizi yeniden başlatın.")

def check_for_updates():
    current_ver = get_current_version()
    print(f"SentezOS Mevcut Sürüm: {current_ver}")
    print(f"📡 GitHub ({GITHUB_REPO}) sunucusuna bağlanılıyor...")
    
    url = f"https://api.github.com/repos/{GITHUB_REPO}/releases/latest"
    try:
        req = requests.get(url, timeout=10)
        if req.status_code == 200:
            data = req.json()
            latest_ver = data.get("tag_name")
            
            if latest_ver != current_ver:
                print(f"🔔 Yeni bir SentezOS sürümü bulundu: {latest_ver}")
                if "assets" in data and len(data["assets"]) > 0:
                    asset_url = data["assets"][0]["browser_download_url"]
                    # Kullanıcı onayı yerine otomatik OTA patch
                    perform_update(asset_url, latest_ver)
                else:
                    print("Güncelleme dosyası (asset) Github'da bulunamadı.")
            else:
                print("✨ SentezOS en güncel sürümde. (Aferin)")
        else:
            print(f"❌ API Hatası: GitHub repoya erişilemedi. {req.status_code}")
    except Exception as e:
        print(f"❌ Ağ hatası: {e}")

if __name__ == "__main__":
    check_for_updates()
