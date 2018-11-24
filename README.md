# WisperChipCli
CAN-BUS utillities command line serial console to monitor directly on Arduino
 

help                 Shows this help screen. welcome Reprint the welcome screen.
clear                Clear the console screen.
cltest n             VT100 color test
reset                Reset/Reboot Arduino
caninit125           Init can-bus - shield at 125Kbps.
caninit500 Init      Init can-bus - shield at 500Kbps.
candump              Dump all CAN-BUS data to console while monitoring changing messages.
rawdump              Dump all CAN-BUS data to console without monitoring.
emulate              Emulate the CAN-BUS.
bruteforce [n]       Perform brute force attack from ID
slowdown [n]         Create a delay between reading messages. Default=0.
ls mem               Print CAN messages currently in memory (Warning Buggy)
hexmode [on] [off]   Show CAN-BUS message using HEX values instead of numbers.


Commands Explained.

clear - Simply clears the console screen.

cltest - If you are interested in modifying the code to display certain values in a different color you can use cltest to test print colors to screen. I.E type: cltest 33

caninit500 or caninit125 will tell the CAN-BUS shield which speed it needs to connect to the CAN-BUS. Warning! if you get this wrong you will temporarily corrupt the CAN-BUS until you disconnect it. On my Jeep this does not do any harm, not sure about other vehicles.

slowdown - This will create a delay to slow things down a little if it all happens too fast for you.
I.E type: slowdown 50 to create a 50ms delay between messages. a 50ms slowdown works best for me.

rawdump - Can only be called after you have set the connection speed using caninit500 or caninit125. rawdump simply just dumps all of the data into the console line by line without applying any monitoring tactics.

candump - Can only be called after you have set the connection speed using caninit500 or caninit125. candump will apply some monitoring strategies by grouping codes by CAN-ID. If the message changed it will highlight the line. If you write down the message and play it back your vehicle should react to it.

hexmode - This will switch between hexmode and number mode. Most info out on the web shows CAN-BUS messages in hex mode. I find it easier to remember and identify numbers rather than looking at hex values.
to trun it on or off type: hexmode on or hexmode off

bruteforce - Can only be called after you have set the connection speed using caninit500 or caninit125. bruteforce will send all possible messages from a specified CAN-ID and work its way up.
I.E type: bruteforce 1000 to start sending all possible messages from id 1000 and upward. This could  potentially be dangerous. Maybe not so much on the interior bus but if you had to run this on the engine side you could potentially fire I.E ABS, injectors or even pre detonate at the wrong time when the engine is running which could lead to catastrophic failure. This is not a fact but  plausible. I have never messed with engine parameters. NEVER mess with the CAN-BUS whilst the vehicle is in motion or running unless you absolutely know what you are doing.

emulate  - Run a simulated demo of how the candump monitoring works. It simply generates serial data on the fly and is not real can-bus data coming from a can-bus network.
