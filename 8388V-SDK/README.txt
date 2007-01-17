                              
                    
                             User Guide for Standalone SDK 
		  ---------------------------------------------------
Following commands are implemented for the user to configure the application using the user interface

1. iwlist scan;
   scans for APs or Ad-hoc networks and presents them to the user
2. iwconfig mode <ad-hoc/managed/any>;
   configures the firmware in one of the specified modes.
3. iwconfig essid <essid>;
   establishes a connection to the given essid. If mode is ad-hoc and no matching essid found then 
   the ad-hoc network is started for the given essid.
4. iwconfig ap <bssid>;
   establishes a connection with the given bssid.
4. econfig <ipaddr> <netmask> <gateway>;
   configure the firmware to use the ip address with the netmask and the gateway.
5. ping <ip address>;
   sends ICMP pings to the given ip address. prints the replies back to the user.
6. ping stop;
   stops sending the ping commands. 
   
Note: Commands end with ';'. This is can be changed by the SDK user.   

The application is written using the mli APIs exposed by the SDK. Layer 2 SDK builds the ICMP/IP/ARP
frames to send the ping commands. Layer 3 SDK uses the ETCP stack to send ping commands.

example scripts:

iwconfig essid marvell-ap1;econfig 192.168.3.24 255.255.255.0 192.168.3.1;ping 192.168.3.1;

iwconfig mode ad-hoc;iwconfig essid marvell_adhoc;econfig 192.168.0.24 255.255.255.0 192.168.0.1;ping 192.168.0.1;

