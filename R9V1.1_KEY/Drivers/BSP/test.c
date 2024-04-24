// #include "mbed.h"

// int addr = 0x0C <<1; // 8bit I2C address

// I2C i2c(PB_9 , PB_8);   //sda, scl

// Serial pc(PA_2, PA_3); //Tx/Rx

// int main()
// {
//     char config [4];
//     char data[7] = {0};

//     config[0] = 0x60;
//     config[1] = 0x00;
//     config[2] = 0x5C;
//     config[3] = 0x00;

//     i2c.write(addr, config, 4, false);

//     i2c.read(addr, data, 1);

//     config[0] = 0x60;
//     config[1] = 0x02;
//     config[2] = 0xB4;
//     config[3] = 0x02;

//     i2c.write(addr, config, 4, false);

//     i2c.read(addr, data, 1);

//     wait(0.25);

//     while (1) {

//         config[0] = 0x3E; // Single measurement mode, ZYX enabled

//         i2c.write(addr, config, 1, false);
//         i2c.read(addr, data, 1);

//         wait(0.1);

//         config[0] = 0x4E;

//         i2c.write(addr, config, 1, false); // Read command, followed by ZYX bits set
//         i2c.read(addr, data, 7);

//         int xMag = ((data[1] * 256) + data[2]);
//         int yMag = ((data[3] * 256) + data[4]);
//         int zMag = ((data[5] * 256) + data[6]);

//         printf("X Axis = %d \n", xMag);
//         printf("Y Axis = %d \n", yMag);
//         printf("Z Axis = %d \n", zMag);

//         wait(5);
//     }
// }