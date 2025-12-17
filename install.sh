#!/bin/bash
# termOS grub installer, auto-resolves termOS path

# get the directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# termOS binary path
TERMOS_PATH="${SCRIPT_DIR}/termOS"

if [[ ! -f "$TERMOS_PATH" ]]; then
    echo "termOS binary not found at $TERMOS_PATH"
    exit 1
fi

# detect root partition
ROOT_PART=$(df / | tail -1 | awk '{print $1}')

# detect current kernel version
KERNEL_VER=$(uname -r)

# grub menuentry template
ENTRY="menuentry \"termOS\" {
    linux /boot/vmlinuz-${KERNEL_VER} root=${ROOT_PART} rw init=${TERMOS_PATH} video=HDMI-1:1920x1080@60
    initrd /boot/initrd.img-${KERNEL_VER}
}"

# write to /etc/grub.d/42_termOS
echo "$ENTRY" | sudo tee /etc/grub.d/42_termOS > /dev/null
sudo chmod +x /etc/grub.d/42_termOS

# regenerate grub.cfg
sudo update-grub

echo "termOS menu entry installed! Reboot and select 'termOS' in grub."
