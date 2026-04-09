use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::UnixStream;
use std::env;

/// Hyprland (Wayland) olaylarını canlı olarak dinler
pub async fn listen_events() {
    let his = env::var("HYPRLAND_INSTANCE_SIGNATURE").unwrap_or_default();
    let socket_path = format!("/tmp/hypr/{}/.socket2.sock", his);
    
    match UnixStream::connect(&socket_path).await {
         Ok(mut stream) => {
             println!("✅ [Neural Core] SentezOS Wayland Soketine Bağlanıldı. Canlı dinleniyor...");
             let mut buf = vec![0; 4096];
             loop {
                 match stream.read(&mut buf).await {
                     Ok(n) if n > 0 => {
                         let event = String::from_utf8_lossy(&buf[..n]);
                         crate::ai_engine::process_event(&event).await;
                     }
                     _ => break,
                 }
             }
         }
         Err(e) => println!("❌ [Neural Core] IPC Kritik Hatası: {}. (Sistemde Wayland WM kapalı olabilir)", e),
    }
}

/// Yapay zeka motorunun kararı doğrultusunda pencereyi ışınlar (dispatch eder)
pub async fn move_window_to_workspace(window_class: &str, workspace: &str) {
    let his = env::var("HYPRLAND_INSTANCE_SIGNATURE").unwrap_or_default();
    let socket_path = format!("/tmp/hypr/{}/.socket1.sock", his);
    let cmd = format!("dispatch movetoworkspace {},class:{}", workspace, window_class);
    
    if let Ok(mut stream) = UnixStream::connect(&socket_path).await {
        let _ = stream.write_all(cmd.as_bytes()).await;
    }
}
