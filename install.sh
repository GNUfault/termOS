#!/bin/bash
# termOS grub installer, auto-resolves termOS path

# get the directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# detect root partition
ROOT_PART=$(df / | tail -1 | awk '{print $1}')

# grub menuentry template
ENTRY="menuentry \"termOS\" {
    linux /vmlinuz-6.14.0-37-generic root=/dev/termOSloop ro init=/termOS
    initrd /initrd.img-6.14.0-37-generic
}"

# write to /etc/grub.d/42_termOS
echo "$ENTRY" | sudo tee /etc/grub.d/42_termOS > /dev/null
sudo chmod +x /etc/grub.d/42_termOS

# regenerate grub.cfg
sudo update-grub

dd if=/dev/zero of=$SCRIPT_DIR/termOS.img bs=1M count=1024   # 1GB
LOOP_DEV=$(sudo losetup -f --show $SCRIPT_DIR/termOS.img)

sudo mkfs.ext4 $LOOP_DEV
mount $LOOP_DEV /mnt/

sudo cp -r $SCRIPT_DIR/* /mnt/
sudo umount /mnt/

echo "termOS installed! Reboot and select 'termOS' in grub."
