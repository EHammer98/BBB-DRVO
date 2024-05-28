#include <linux/module.h>
#include <linux/i2c.h>

static int arduino_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    char buffer[2];
    int dist;

    // Example read
    i2c_master_recv(client, buffer, 2);
    dist = (buffer[0] << 8) | buffer[1];

    printk(KERN_INFO "Distance: %d cm\n", dist);

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

module_i2c_driver(arduino_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("E. Hammer");
MODULE_DESCRIPTION("I2C Driver for Arduino with Ultrasonic Sensor");
