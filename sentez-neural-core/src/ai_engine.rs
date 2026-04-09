pub async fn process_event(event_str: &str) {
    // SentezOS Olay Motoru - Parçalama (Parsing)
    for line in event_str.lines() {
        if line.starts_with("openwindow>>") {
            let parts: Vec<&str> = line.split(">>").collect();
            if parts.len() == 2 {
                let data: Vec<&str> = parts[1].split(',').collect();
                if data.len() >= 3 {
                    let class = data[2];
                    let title = data[3];
                    println!("🚀 [Sentez AI] Yeni Uygulama Yakalandı: Sınıf='{}', Başlık='{}'", class, title);
                    categorize_and_move(class).await;
                }
            }
        } else if line.starts_with("activewindow>>") {
            // Kullanıcının odağını takip edip donanım önceliği (CPU priority) vereceğimiz yer.
        }
    }
}

async fn categorize_and_move(class: &str) {
    // "SentezOS Smart Workspace" Algoritması
    // Not: Bu kısımda normalde local bir LLM veya model (ONNX framework) ile
    // class/başlık anlamlandırması yapılıp app'in ne işe yaradığı saptanır.
    // Şimdilik üst-düzey kurallı sınıflandırma makinesi (Heuristic).

    let target_workspace = match class.to_lowercase().as_str() {
        "code" | "rustrover" | "kitty" | "alacritty" => "2", // Dev & Code
        "firefox" | "google-chrome" | "brave" => "1",        // Web Surf
        "discord" | "spotify" | "obs" => "3",                // Medya & İletişim
        "sentez-store" | "systemsettings" => "9",            // Sistem Konfigürasyon
        _ => "4",                                            // Dinamik Diğerleri
    };
    
    println!("🧠 [Sentez AI] Karar: '{}' için en uygun vektör -> Çalışma Alanı {}", class, target_workspace);
    crate::hyprland_ipc::move_window_to_workspace(class, target_workspace).await;
}
