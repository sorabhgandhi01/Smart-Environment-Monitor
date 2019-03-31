
#include "i2c_helper.h"

pthread_mutex_t i2c_bus_lock = PTHREAD_MUTEX_INITIALIZER;

int i2c_open()
{
    int status = 0;

    if (pthread_mutex_init(&i2c_bus_lock, NULL) != 0) {
        
        printf("Failed to initialize mutex\n");
        return -1;
    }

    pthread_mutex_lock(&i2c_bus_lock);

    if (i2c_context != NULL) {
        status = 0;
    }
    else {
        i2c_context = mraa_i2c_init_raw(MY_I2C_BUS);
        if (i2c_context == NULL) {
            perror("Failed to initialize I2C");
            mraa_deinit();
            status = -1;
        }
    }

    pthread_mutex_unlock(&i2c_bus_lock); 

    return status;
}

int i2c_close()
{
     pthread_mutex_lock(&i2c_bus_lock); 
     int status = 0;

     if (i2c_context != NULL) {
        status = mraa_i2c_stop(i2c_context); 
        i2c_context = NULL;  
     }
     else {
        status = 0;
     }


     //mraa_deinit();

     pthread_mutex_unlock(&i2c_bus_lock); 

     if (pthread_mutex_destroy(&i2c_bus_lock) != 0) {

         printf("Failed to destroy mutex\n");
         status = -1;
     }

     return status;
}

int i2c_write(uint8_t slave_addr, uint8_t command)
{
    mraa_result_t status = MRAA_SUCCESS;

    pthread_mutex_lock(&i2c_bus_lock); 

    status = mraa_i2c_address(i2c_context, slave_addr);
    if (status == MRAA_SUCCESS) {
        status = mraa_i2c_write_byte(i2c_context, command);
    }

    pthread_mutex_unlock(&i2c_bus_lock); 

    return status;
}

int i2c_write_byte(uint8_t slave_addr, uint8_t data, uint8_t command)
{
    mraa_result_t status = MRAA_SUCCESS;

    pthread_mutex_lock(&i2c_bus_lock);

    status = mraa_i2c_address(i2c_context, slave_addr);
    if (status == MRAA_SUCCESS) {
        status = mraa_i2c_write_byte_data(i2c_context, data, command);
    }

    pthread_mutex_unlock(&i2c_bus_lock);
    
    return status;
}

int i2c_write_word(uint8_t slave_addr, uint16_t data, uint8_t command)
{
    mraa_result_t status = MRAA_SUCCESS;

    pthread_mutex_lock(&i2c_bus_lock);

    status = mraa_i2c_address(i2c_context, slave_addr);
    if (status == MRAA_SUCCESS) {
        status = mraa_i2c_write_word_data(i2c_context, data, command);
    }

    pthread_mutex_unlock(&i2c_bus_lock);

    return status;
}


int i2c_read(uint8_t slave_addr, uint8_t *data, uint8_t command)
{
    mraa_result_t status = MRAA_SUCCESS;

    pthread_mutex_lock(&i2c_bus_lock);

    status = mraa_i2c_address(i2c_context, slave_addr);
    if (status == MRAA_SUCCESS) {
        status = mraa_i2c_read_byte_data (i2c_context, command);

        if (status != -1) {
            *data = status;
            status = 0;
        }
    }

    pthread_mutex_unlock(&i2c_bus_lock);

    return status;
}

int i2c_read_bytes(uint8_t slave_addr, uint8_t *data, uint8_t command, size_t len)
{
    mraa_result_t status = MRAA_SUCCESS;

    pthread_mutex_lock(&i2c_bus_lock);

    status = mraa_i2c_address(i2c_context, slave_addr);
    if (status == MRAA_SUCCESS) {
        status = mraa_i2c_read_bytes_data (i2c_context, command, data, len);

        if (status == len)
            status = 0;
    }

    pthread_mutex_unlock(&i2c_bus_lock);

    return status;
}
