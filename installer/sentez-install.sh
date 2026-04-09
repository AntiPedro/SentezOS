#!/bin/bash
# ==========================================
# SENTEZ OS - BTRFS & SYSTEM INSTALLER
# ==========================================
set -e
echo "==== SENTEZ OS KURULUM SIHIRBAZI ===="
DISK=${1:-"/dev/sda"}
echo "Hedef Disk: $DISK (DIKKAT: DISK SILINECEK!)"
sleep 5

# Disk yapılandırma (EFI + BTRFS)
echo "1. Diski partisyonluyoruz..."
sgdisk -Z $DISK
sgdisk -n 1:0:+512M -t 1:ef00 -c 1:"EFI System" $DISK
sgdisk -n 2:0:0 -t 2:8300 -c 2:"SentezOS BTRFS" $DISK

# Format
echo "2. Format Atiliyor..."
mkfs.vfat -F32 ${DISK}1
mkfs.btrfs -f ${DISK}2

# BTRFS Subvolumes (Timeshift / Snapshot mimarisi için)
echo "3. Subvolumeler yaratiliyor (@, @home, @log, @pkg)..."
mount ${DISK}2 /mnt
btrfs subvolume create /mnt/@
btrfs subvolume create /mnt/@home
btrfs subvolume create /mnt/@log
btrfs subvolume create /mnt/@pkg
umount /mnt

# Mount subvols with optimal flags
mount -o noatime,compress=zstd,space_cache=v2,subvol=@ ${DISK}2 /mnt
mkdir -p /mnt/{boot,home,var/log,var/cache/pacman/pkg}
mount -o noatime,compress=zstd,space_cache=v2,subvol=@home ${DISK}2 /mnt/home
mount -o noatime,compress=zstd,space_cache=v2,subvol=@log ${DISK}2 /mnt/var/log
mount -o noatime,compress=zstd,space_cache=v2,subvol=@pkg ${DISK}2 /mnt/var/cache/pacman/pkg
mount ${DISK}1 /mnt/boot

# Install Base System (Pacstrap)
echo "4. PACSTRAP: Temel sistem kuruluyor..."
pacstrap /mnt base linux linux-firmware btrfs-progs grub efibootmgr networkmanager hyprland sddm qt6-wayland sudo nano git

# FSTAB uret
genfstab -U /mnt >> /mnt/etc/fstab

# Chroot ortami icerisinde konfigürasyon
echo "5. GRUB, Kullanici ve Boot ayarlari yapiliyor..."
arch-chroot /mnt /bin/bash <<EOF
ln -sf /usr/share/zoneinfo/Europe/Istanbul /etc/localtime
hwclock --systohc
echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
locale-gen
echo "LANG=en_US.UTF-8" > /etc/locale.conf
echo "sentezos" > /etc/hostname

grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=GRUB
grub-mkconfig -o /boot/grub/grub.cfg

systemctl enable NetworkManager.service
systemctl enable sddm.service

# Root sifresi
echo "root:sentez" | chpasswd
EOF

echo "==== KURULUM TAMAMLANDI! Reboot atabilirsiniz. ===="
