# updatedSim
simulator - update repository
 ## FlightGear-Interpreter

An Interpreter project for a FlightGear flight simulator. The program connects to the simulator and flys the aircraft based on the user's commands.

**Prerequisites**

Linux environment to run the code

**Installing**

Download and install the simulator on your computer -[https://www.flightgear.org/download/](https://www.flightgear.org/download/) 

<p> Add the generic_small.xml file to the /data/Protocol directory where you installed the simulator
Config the following settings in the 'Settings' tab in the simulator:

`--telnet=socket,in,10,127.0.0.1,5402,tcp` 

`--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small` 

This will open two communication sockets - 'in' where you send commands to the simulator, and 'out' where you receive data from it. </p>

# Running

a. Exceute the code using the terminal or any c++ work environment. The program will wait for a connection from the simulator.

b. Click the 'Fly!' icon in the simulator in the bottom left corner (see above picture), and wait for the simulator to load. It will connect to the interpreter in the meantime.<br>

The code is designed to read lines of commands entered in a terminal from the user once the program has started running. The commands can be entered one at a time or by using a pre-made script (make sure each command starts in a new line).

First, enter the following commands to open a local communication server to the simulator and connect to it:
openDataServer(5400)
connectControlClient("127.0.0.1",5402)

# Initialize your variables:
```
var warp -> sim("/sim/time/warp") <br>
var magnetos -> sim("/controls/switches/magnetos") <br>
var throttle -> sim("/controls/engines/current-engine/throttle")<br>
var mixture -> sim("/controls/engines/current-engine/mixture")<br>
var masterbat -> sim("/controls/switches/master-bat")<br>
var masterlat -> sim("/controls/switches/master-alt")<br>
var masteravionics -> sim("/controls/switches/master-avionics")<br>
var brakeparking -> sim("/sim/model/c172p/brake-parking")<br>
var primer -> sim("/controls/engines/engine/primer")<br>
var starter -> sim("/controls/switches/starter")<br>
var autostart -> sim("/engines/active-engine/auto-start")<br>
var breaks -> sim("/controls/flight/speedbrake")<br>
var heading <- sim("/instrumentation/heading-indicator/offset-deg")<br>
var airspeed <- sim("/instrumentation/airspeed-indicator/indicated-speed-kt")<br>
var roll <- sim("/instrumentation/attitude-indicator/indicated-roll-deg")<br>
var pitch <- sim("/instrumentation/attitude-indicator/internal-pitch-deg")<br>
var rudder -> sim("/controls/flight/rudder")<br>
var aileron -> sim("/controls/flight/aileron")<br>
var elevator -> sim("/controls/flight/elevator")<br>
var alt <- sim("/instrumentation/altimeter/indicated-altitude-ft")<br>
var rpm <- sim("/engines/engine/rpm")<br>
Initialize variables using the keyword '->' and '<-'. <br>
Initializing without the keyword will result in failure.<br>
The '->' and '<-' keyword is used to bind a variable to a specific path. See generic_small.xml file for all paths.<br>
Exceute your commands:<br>
breaks = 0<br>
throttle = 1<br>
var h0 = heading<br>
while alt < 1000 {<br>
    rudder = (h0 – heading)/20<br>
    aileron = - roll / 70<br>
    elevator = pitch / 50<br>
    Print alt<br>
    Sleep 250<br>
}<br>
Print "done"<br>
```


The program reads the code, parses it and sends the commands to the simulator, changing the behavior of the aircraft accordingly. <br>In this example, the interpreter enters the loop, adjusts variables, prints the current altitude of the aircraft and halts the reading for 250 miliseconds. <br>When the altitude is over 1000, the loop will terminate and the next lines will be parsed.<br>

When the program reads the keyword "done", it will stop sending commands to the simulator, terminating the communication with it and ending the program.<br>
