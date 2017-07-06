/*********************************************************************
 * robotiq850.cpp
 *
 * Copyright (c) 2017, National Chiao Tung University, Taiwan
 *
 * Author: Howard Chen (s880367@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **********************************************************************/
/*
 * TODO: 
 * More control method for gripper (force, position)
 * 
 */

#include <ros/ros.h>

#include <boost/asio/serial_port.hpp> 
#include <boost/asio/serial_port_base.hpp> 
#include <boost/asio.hpp> 

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
 
using namespace std;
int main(int argc, char **argv)
{
	ros::init(argc, argv,"robotiq_850");
	char Return_data[100];
	int return_size = 0;
	boost::asio::io_service robotiq850;
	boost::asio::serial_port port(robotiq850,"/dev/ttyUSB1");
	
	if(!port.is_open())
	{
		printf("Cannot open port!!\n");
		exit(0);
	}

	boost::asio::serial_port_base::parity::type none;
	boost::asio::serial_port_base::stop_bits::type one;
	port.set_option(boost::asio::serial_port_base::baud_rate(115200));
	port.set_option(boost::asio::serial_port_base::parity());
	port.set_option(boost::asio::serial_port_base::stop_bits());
	port.set_option(boost::asio::serial_port_base::character_size(8));

	//activation
	unsigned char data[] = {0x09,0x10,0x03,0xE8,0x00,0x03,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0x30};  
	port.write_some(boost::asio::buffer(data,sizeof(data)));

	return_size = port.read_some (boost::asio::buffer(Return_data,sizeof(Return_data)));
	printf("Response1 : ");
	for (int i = 0; i < return_size; ++i)
	{
		printf("%02x ",Return_data[i] & 0xff);
	}
	printf("\n");
	usleep(10000);

	// read gripper status until activation is completed
	unsigned char data2[] = {0x09,0x03,0x07,0xD0,0x00,0x01,0x85,0xCF};  
	port.write_some(boost::asio::buffer(data2,sizeof(data2)));

	return_size = port.read_some (boost::asio::buffer(Return_data,sizeof(Return_data)));
	printf("Response2 : ");
	for (int i = 0; i < return_size; ++i)
	{
		printf("%02x ",Return_data[i] & 0xff);
	}
	printf("\n");
	sleep(1);

	while(ros::ok())
	{
		//Open the Gripper at full speed and full force
		unsigned char data3[] = {0x09,0x10,0x03,0xE8,0x00,0x03,0x06,0x09,0x00,0x00,0x00,0xFF,0xFF,0x72,0x19};
		port.write_some(boost::asio::buffer(data3,sizeof(data3)));

		return_size = port.read_some (boost::asio::buffer(Return_data,sizeof(Return_data)));
		printf("Open : ");
		for (int i = 0; i < return_size; ++i)
		{
			printf("%02x ",Return_data[i] & 0xff);
		}
		printf("\n");

		if(getchar() == 'q')
			break;

		//Close the Gripper at full speed and full force
		unsigned char data4[] = {0x09,0x10,0x03,0xE8,0x00,0x03,0x06,0x09,0x00,0x00,0xFF,0xFF,0xFF,0x42,0x29};
		port.write_some(boost::asio::buffer(data4,sizeof(data4)));

		return_size = port.read_some (boost::asio::buffer(Return_data,sizeof(Return_data)));
		printf("Close : ");
		for (int i = 0; i < return_size; ++i)
		{
			printf("%02x ",Return_data[i] & 0xff);
		}
		printf("\n");
		
		if(getchar() == 'q')
			break;
	}



	port.close();
	return 0;
}