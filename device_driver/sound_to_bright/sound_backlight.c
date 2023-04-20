#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <math.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <string.h>

#define DEVICE_NAME "brightness_control"
#define CLASS_NAME "brightness"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux device driver for changing brightness");
MODULE_VERSION("0.1");

static int brightness_value = 0;

#define bright_percentage "/sys/class/backlight/amdgpu_bl1/brightness"

#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 2

#define MAX_SOUND 120
#define MIN_SOUND 1

#define NUM_SAMPLES (SAMPLE_RATE) // 0.1 seconds of samples
#define A_REF 1.0 // reference amplitude for calculating decibel value

int record() {
    int SAMPLE_RATE = 44100;
    int rc;
    int16_t buffer[NUM_SAMPLES];
    double rms = 0.0;
    double db = 0.0;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;

    // Open default ALSA device for recording
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Set hardware parameters for recording
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 1);
    snd_pcm_hw_params_set_rate_near(handle, params, &SAMPLE_RATE, 0);
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, 0);
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set HW parameters: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Start recording
    rc = snd_pcm_prepare(handle);
    if (rc < 0) {
        fprintf(stderr, "unable to prepare PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }
    rc = snd_pcm_start(handle);
    if (rc < 0) {
        fprintf(stderr, "unable to start PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Read samples from microphone and calculate decibel value
    rc = snd_pcm_readi(handle, buffer, NUM_SAMPLES);
    if (rc != NUM_SAMPLES) {
        fprintf(stderr, "short read from PCM device: %d\n", rc);
        exit(1);
    }
    for (int i = 0; i < NUM_SAMPLES; i++) {
        rms += pow(buffer[i], 2);
    }
    rms /= NUM_SAMPLES;
    rms = sqrt(rms);
    db = 20 * log10(rms / A_REF);

    printf("Decibel value: %f dB\n", db);

    // Cleanup
    snd_pcm_close(handle);
    return (int)db;
}

void set_brightness(){
    
    FILE *bright_percentage_1 = fopen(bright_percentage, "w");
    int db = record();

    brightness_value = (db/MAX_SOUND)(MAX_BRIGHTNESS - MIN_BRIGHTNESS) + MIN_BRIGHTNESS;
    
    fprintf(bright_percentage_1,"%d",brightness_value);
    fclose(bright_percentage_1);
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
