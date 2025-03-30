#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>  // copy_to_user, copy_from_user

#define DEVICE_NAME "virtual_keyboard"
#define BUF_LEN 256

static int major;
static char buffer[BUF_LEN];
static int buffer_len = 0;

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "[vkbd] Device opened\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "[vkbd] Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *user_buf, size_t len, loff_t *offset) {
    if (*offset >= buffer_len) return 0;

    if (copy_to_user(user_buf, buffer, buffer_len)) {
        return -EFAULT;
    }

    printk(KERN_INFO "[vkbd] Sent '%s' to user\n", buffer);
    *offset += buffer_len;
    return buffer_len;
}

static ssize_t dev_write(struct file *filep, const char __user *user_buf, size_t len, loff_t *offset) {
    if (len > BUF_LEN - 1)
        len = BUF_LEN - 1;

    if (copy_from_user(buffer, user_buf, len)) {
        return -EFAULT;
    }

    buffer[len] = '\0';
    buffer_len = len;
    printk(KERN_INFO "[vkbd] Received '%s' from user\n", buffer);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init vkbd_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "[vkbd] Failed to register a major number\n");
        return major;
    }

    printk(KERN_INFO "[vkbd] Registered with major number %d\n", major);
    return 0;
}

static void __exit vkbd_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "[vkbd] Unregistered device\n");
}

module_init(vkbd_init);
module_exit(vkbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("A virtual keyboard-like character device driver");

