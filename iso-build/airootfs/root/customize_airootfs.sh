#!/usr/bin/env bash
set -e -u

# SentezOS Live ISO Post-Install Script
sed -i 's/#\(en_US\.UTF-8\)/\1/' /etc/locale.gen
locale-gen

# Temel Servisleri Aktifleştir
systemctl enable NetworkManager.service
systemctl enable sddm.service

# Live Ortamı İçin Sentez Kullanıcısını Yarat
useradd -m -p "" -g users -G "adm,audio,log,network,rfkill,scanner,storage,power,wheel" -s /bin/bash sentez
echo "sentez ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers.d/sentez

# Kurulum scriptini yetkilendir
chmod +x /usr/local/bin/sentez-install.sh
