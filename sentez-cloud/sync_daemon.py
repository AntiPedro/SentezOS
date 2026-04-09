import os
import time
import requests
import json
from pathlib import Path

# SentezOS Supabase Cloud Sync Daemon
# Bu servis arkaplanda calisip kullanicinin theme, app config 
# vs gibi seylerini Supabase DB uzerine yedekler / geri ceker.

SUPABASE_URL = os.getenv("SENTEZ_SUPABASE_URL", "https://xyz.supabase.co")
SUPABASE_KEY = os.getenv("SENTEZ_SUPABASE_KEY", "your-anon-key")
USER_ID = os.getenv("USER")

# İzlenecek konfigürasyon dosyaları
CONFIG_PATHS = {
    "hyprland_config": str(Path.home() / ".config/hypr/hyprland.conf"),
    "sentez_panel": str(Path.home() / ".config/sentez-panel/config.json")
}

def sync_configs():
    print(f"[Sentez Cloud] {USER_ID} kullanicisi verisi sync ediliyor...")
    headers = {
        "apikey": SUPABASE_KEY,
        "Authorization": f"Bearer {SUPABASE_KEY}",
        "Content-Type": "application/json"
    }
    
    payload = {"user_id": USER_ID, "configs": {}}
    
    # Yereldeki dosyalari topla
    for key, path in CONFIG_PATHS.items():
        if os.path.exists(path):
            with open(path, 'r') as f:
                payload["configs"][key] = f.read()

    try:
        response = requests.post(
            f"{SUPABASE_URL}/rest/v1/sentez_user_profiles", 
            headers=headers, 
            json=payload
        )
        if response.status_code in [200, 201]:
            print(f"[Sentez Cloud] Basarili: UUID: {USER_ID}")
        else:
            print(f"[Sentez Cloud] API Hatasi: {response.status_code} - {response.text}")
    except Exception as e:
        print(f"[Sentez Cloud] Baglanti koptu: {e}")

if __name__ == "__main__":
    print("[Sentez Cloud Daemon] Baslatildi.")
    while True:
        sync_configs()
        # Her yarim saatte bir cloud sync calistir
        time.sleep(1800)
