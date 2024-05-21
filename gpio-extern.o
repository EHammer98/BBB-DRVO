#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/ioport.h>
#include <linux/gpio.h>  // Include GPIO library for GPIO functions

// Declare the GPIO pin
static int gpio_pin = 51;  // Adjust if your GPIO number is different

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
        gpio_free(gpio_pin);  // Free GPIO if setting direction fails
        return ret;
    }

    // Turn the LED on after initialization
    led_on(gpio_pin);

    printk(KERN_INFO "gpio_ex_probe: LED initialized and turned on on GPIO %d\n", gpio_pin);
    return 0;
}

// Remove function called when the device is removed
static int gpio_ex_remove(struct platform_device *pdev) {
    // Turn the LED off before removing the driver
    led_off(gpio_pin);

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
    int result;
    result = platform_driver_register(&g_driver); 
    printk(KERN_INFO "gpio_ex_init() called\n");
    return result;
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
