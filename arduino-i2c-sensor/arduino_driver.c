#include <linux/module.h>
#include <linux/i2c.h>

static int arduino_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    int err;

    err = device_create_file(&client->dev, &dev_attr_distance);
    if (err)
        return err;

    printk(KERN_INFO "Arduino I2C Driver Initialized.\n");
    return 0;
}


static int arduino_remove(struct i2c_client *client) {
    device_remove_file(&client->dev, &dev_attr_distance);
    printk(KERN_INFO "Arduino I2C Driver Removed.\n");
    return 0;
}

static int arduino_remove(struct i2c_client *client) {
    return 0;
}

static const struct i2c_device_id arduino_id[] = {
    { "arduino", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, arduino_id);

static struct i2c_driver arduino_driver = {
    .driver = {
        .name = "arduino",
        .owner = THIS_MODULE,
    },
    .probe = arduino_probe,
    .remove = arduino_remove,
    .id_table = arduino_id,
};

static ssize_t distance_show(struct device *dev, struct device_attribute *attr, char *buf) {
    int dist;
    char buffer[2];
    struct i2c_client *client = to_i2c_client(dev);

    if (i2c_master_recv(client, buffer, 2) < 0)
        return -EIO;

    dist = (buffer[0] << 8) | buffer[1];
    return sprintf(buf, "%d\n", dist);
}

static DEVICE_ATTR(distance, S_IRUGO, distance_show, NULL);


module_i2c_driver(arduino_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("E. Hammer");
MODULE_DESCRIPTION("I2C Driver for Arduino with Ultrasonic Sensor");
