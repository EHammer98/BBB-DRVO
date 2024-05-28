#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/ioport.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>  // Make sure this is included

// Declare the GPIO pin
static int gpio_pin = 51;  // Adjust if your GPIO number is different

// Function prototypes to ensure they are known before first use
static void led_on(int gpio_pin);
static void led_off(int gpio_pin);

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
	const char* label;
    struct device_node *np = pdev->dev.of_node;

    printk(KERN_INFO "gpio_ex_probe: Device probe started for %pOF\n", np);
    
    // Attempt to retrieve the GPIO pin
    gpio_pin = of_get_named_gpio(np, "gpios", 0);
    if (gpio_pin < 0) {
        printk(KERN_ERR "Failed to get GPIO pin from device tree: %d\n", gpio_pin);
        return gpio_pin;
    }
    printk(KERN_INFO "GPIO pin retrieved from device tree: %d\n", gpio_pin);

    // Request the GPIO
    ret = gpio_request(gpio_pin, "gpio_led");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO %d: %d\n", gpio_pin, ret);
        return ret;
    }

    // Set GPIO direction to output and turn the LED on for confirmation
    ret = gpio_direction_output(gpio_pin, 0);  // Assuming '0' means to turn off initially
    if (ret) {
        printk(KERN_ERR "Failed to set GPIO %d as output: %d\n", gpio_pin, ret);
        gpio_free(gpio_pin);
        return ret;
    }
    led_on(gpio_pin);  // Turn on the LED as confirmation

    // Create sysfs entry after successful GPIO setup
    ret = sysfs_create_file(&pdev->dev.kobj, &led_attribute.attr);
    if (ret) {
        printk(KERN_ERR "Failed to create sysfs entry for LED: %d\n", ret);
        gpio_free(gpio_pin);  // Cleanup GPIO before returning
        return ret;
    }
	
		
	if (!of_property_read_string(np, "label", &label)) {
		printk(KERN_INFO "Label from DT: %s\n", label);
	} else {
		printk(KERN_INFO "Failed to read label from DT\n");
	}


    printk(KERN_INFO "gpio_ex_probe: LED initialized and turned on on GPIO %d\n", gpio_pin);
    printk(KERN_INFO "gpio_ex_probe: Completed successfully\n");

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
        .name  = "led-extern",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(g_ids),
    }
};

// Initialize the driver
static int __init gpio_ex_init(void) {
    int ret;
    printk(KERN_INFO "gpio_ex_init() called\n");
    ret = platform_driver_register(&g_driver);
    if (ret) {
        printk(KERN_ERR "gpio_ex_init: Failed to register driver: %d\n", ret);
        return ret;
    }
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
MODULE_AUTHOR("E. Hammer");
MODULE_DESCRIPTION("A simple external LED driver for the BBB");
