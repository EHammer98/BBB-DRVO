#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/of.h>

// Forward declaration of the show function for the sysfs entry
static ssize_t distance_show(struct device *dev, struct device_attribute *attr, char *buf);

// Declaration of the sysfs attribute
static DEVICE_ATTR(distance, S_IRUGO, distance_show, NULL);

static int arduino_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    printk(KERN_INFO "Arduino probe start.\n");
    
    int err = device_create_file(&client->dev, &dev_attr_distance);
    if (err) {
        printk(KERN_ERR "Failed to create sysfs entry: %d\n", err);
        return err;
    }

    printk(KERN_INFO "Arduino I2C Driver Initialized.\n");
    return 0;
}


static int arduino_remove(struct i2c_client *client) {
    // Remove the sysfs file
    device_remove_file(&client->dev, &dev_attr_distance);
    printk(KERN_INFO "Arduino I2C Driver Removed.\n");
    return 0;
}

static ssize_t distance_show(struct device *dev, struct device_attribute *attr, char *buf) {
    int dist;
    char buffer[2];
    struct i2c_client *client = to_i2c_client(dev);

    // Read the distance data from the I2C device
    if (i2c_master_recv(client, buffer, 2) < 0)
        return -EIO;

    dist = (buffer[0] << 8) | buffer[1];
    return sprintf(buf, "%d\n", dist);
}

static const struct i2c_device_id arduino_id[] = {
    { "arduino", 0 },
    { }
};

static const struct of_device_id arduino_of_match[] = {
    { .compatible = "generic,i2c-slave" },
    {},
};
MODULE_DEVICE_TABLE(of, arduino_of_match);

static struct i2c_driver arduino_driver = {
    .driver = {
        .name = "arduino",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(arduino_of_match),
    },
    .probe = arduino_probe,
    .remove = arduino_remove,
    .id_table = arduino_id,
};


module_i2c_driver(arduino_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("E. Hammer");
MODULE_DESCRIPTION("I2C Driver for Arduino with Ultrasonic Sensor");
