#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/ioport.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/gpio.h>

// Declare the GPIO pin
static int gpio_pin = 51;  // Adjust if your GPIO number is different


// Function to read the LED state
static ssize_t led_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(gpio_pin));
}

// Function to write the LED state
static ssize_t led_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    int value;
    sscanf(buf, "%d", &value);
    if (value == 0) {
        led_off(gpio_pin);
    } else {
        led_on(gpio_pin);
    }
    return count;
}

// Define the sysfs attribute
static struct kobj_attribute led_attribute = __ATTR(led, 0664, led_show, led_store);

// Function to turn the LED on
static void led_on(int gpio_pin) {
    gpio_set_value(gpio_pin, 1);  // Set GPIO to high
    printk(KERN_INFO "LED on GPIO %d turned on\n", gpio_pin);
}

// Function to turn the LED off
static void led_off(int gpio_pin) {
    gpio_set_value(gpio_pin, 0);  // Set GPIO to low
    printk(KERN_INFO "LED on GPIO %d turned off\n", gpio_pin);
}

// Array of compatible device IDs
static const struct of_device_id g_ids[] = {
    { .compatible = "gpio-extern", },
    { } // Ends with an empty entry; MUST be the last member
};

// Probe function called when the device is detected
static int gpio_ex_probe(struct platform_device *pdev) {
    int ret;

    printk(KERN_INFO "gpio_ex_probe: Device probe started\n");

    // Request the GPIO
    ret = gpio_request(gpio_pin, "gpio_led");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO %d\n", gpio_pin);
        return ret;
    }

    // Set GPIO direction to output and initialize to low (LED off)
    ret = gpio_direction_output(gpio_pin, 0);
    if (ret) {
        printk(KERN_ERR "Failed to set GPIO %d as output\n", gpio_pin);
        gpio_free(gpio_pin);
        return ret;
    }

    // Create sysfs entry
    ret = sysfs_create_file(&pdev->dev.kobj, &led_attribute.attr);
    if (ret) {
        printk(KERN_ERR "Failed to create sysfs entry for LED\n");
        gpio_free(gpio_pin);
        return ret;
    }

    // Turn the LED on after initialization for confirmation
    led_on(gpio_pin);

    printk(KERN_INFO "gpio_ex_probe: LED initialized and turned on on GPIO %d\n", gpio_pin);
    return 0;
}

// Remove function called when the device is removed
static int gpio_ex_remove(struct platform_device *pdev) {
    // Turn the LED off before removing the driver
    led_off(gpio_pin);

    // Remove sysfs entry
    sysfs_remove_file(&pdev->dev.kobj, &led_attribute.attr);

    // Free the GPIO pin
    gpio_free(gpio_pin);

    printk(KERN_INFO "gpio_ex_remove: LED turned off and GPIO freed\n");
    return 0;
}

// Define the platform driver structure
struct platform_driver g_driver = {
    .probe = gpio_ex_probe,
    .remove = gpio_ex_remove,
    .driver = {
        .name  = "gpio-extern",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(g_ids),
    }
};

// Initialize the driver
static int __init gpio_ex_init(void) {
    struct platform_device *pdev;
    pdev = platform_device_register_simple("gpio-extern", -1, NULL, 0);
    if (IS_ERR(pdev)) {
        printk(KERN_ERR "gpio_ex_init: Failed to create test device\n");
        return PTR_ERR(pdev);
    }
    printk(KERN_INFO "gpio_ex_init() called, test device created\n");
    return 0;
}


// Exit the driver
static void __exit gpio_ex_exit(void) {
    platform_driver_unregister(&g_driver);
    printk(KERN_INFO "gpio_ex_exit() called\n");
}

module_init(gpio_ex_init);
module_exit(gpio_ex_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple external LED driver for the BBB");
