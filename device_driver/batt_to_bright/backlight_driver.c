#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/backlight.h>

MODULE_LICENSE("GPL");

static struct backlight_device *backlight_dev;

static int __init backlight_init(void)
{
    struct backlight_properties props = {
        .type = BACKLIGHT_RAW,
        .max_brightness = 255,
    };

    backlight_dev = backlight_device_register("my_backlight", NULL, NULL, &props);
    if (IS_ERR(backlight_dev)) {
        printk(KERN_ERR "Failed to register backlight device\n");
        return PTR_ERR(backlight_dev);
    }

    return 0;
}

static void __exit backlight_exit(void)
{
    backlight_device_unregister(backlight_dev);
}

module_init(backlight_init);
module_exit(backlight_exit);
