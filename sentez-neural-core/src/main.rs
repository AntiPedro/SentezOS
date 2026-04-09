mod hyprland_ipc;
mod ai_engine;

#[tokio::main]
async fn main() {
    println!("=======================================");
    println!(" 🧠 SENTEZ NEURAL CORE BAŞLATILIYOR... ");
    println!("=======================================");
    println!("Sistem Hafızası İndeksleniyor...");
    println!("Wayland WM Soketine Bağlanılıyor...");
    
    // Asenkron event dinleme motorunu başlat
    hyprland_ipc::listen_events().await;
}
