#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
// #include <stdio.h>

#define DEVICE_NAME "brightness_control"
#define CLASS_NAME "brightness"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux device driver for changing brightness");
MODULE_VERSION("0.1");

static int major_number;
static char message[256] = {0};
static int brightness_value = 0;

#define batt_capacity "/sys/class/power_supply/BAT1/capacity"
#define batt_status "/sys/class/power_supply/BAT1/status"
#define bright_percentage "/sys/class/backlight/amdgpu_bl1/brightness"

#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 2
// #define FILE struct file

//int brightness_value;

void set_brightness(void){
    
    int batt_capacity_2;
    char batt_status_2[12];

    struct file *batt_status_1 = filp_open(batt_status, O_RDONLY, 0); // O_RDONLY
    struct file *batt_capacity_1 = filp_open(batt_capacity, O_RDONLY, 0);
    struct file *bright_percentage_1 = filp_open(bright_percentage, O_WRONLY | O_CREAT, 0644);
	
    char buffer2[100];
    int len2 = snprintf(buffer2, sizeof(buffer2), "%d", batt_capacity_2);
    kernel_read(batt_status_1, batt_status_2, sizeof(batt_status_2) - 1, &batt_status_1->f_pos);
    kernel_read(batt_capacity_1, buffer2, sizeof(buffer2) - 1 , &batt_capacity_1->f_pos);

    if (strcmp(batt_status, "Charging") == 0){
	char buffer[100];
	int len = snprintf(buffer, sizeof(buffer), "%d", MAX_BRIGHTNESS);
        kernel_write(bright_percentage_1, buffer, len, &bright_percentage_1->f_pos);
    }
    else{
	char buffer[100];
        brightness_value = (batt_capacity_2 * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) / 100 )+ MIN_BRIGHTNESS;
	int len = snprintf(buffer, sizeof(buffer), "%d", brightness_value);
        kernel_write(bright_percentage_1, buffer, len, &bright_percentage_1->f_pos);
    }

    filp_close(batt_status_1, NULL);
    filp_close(batt_capacity_1, NULL);
    filp_close(bright_percentage_1, NULL);
}

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device released\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    printk(KERN_INFO "Read function called\n");
    return 0;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
    int error_count = 0;
    if (copy_from_user(message, buffer, length))
    {
        printk(KERN_INFO "Failed to copy message from user\n");
        return -EFAULT;
    }
    error_count = kstrtoint(message, 10, &brightness_value);
    if (error_count != 0)
    {
        printk(KERN_INFO "Failed to convert message to integer\n");
        return -EINVAL;
    }
    set_brightness();
    printk(KERN_INFO "Set brightness value to %d\n", brightness_value);
    return length;
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init brightness_control_init(void)
{
    printk(KERN_INFO "Initializing brightness control device driver\n");
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0)
    {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "Registered successfully with major number %d\n", major_number);
    return 0;
}

static void __exit brightness_control_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Goodbye from brightness control device driver\n");
}

module_init(brightness_control_init);
module_exit(brightness_control_exit);
