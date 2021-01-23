#Sensorio hardware errata

## v1.0

1. BNO055 interrupt line is not usable, since the embedded firmware does not support it. This pin can be freed.

2. Battery charge monitor pin is input only without pull-up or pull-down resistor, therefore not possible to check if STAT pin of MCP73831 is floating or not. 
    - solution #1: use SW4 to monitor, if USB is connected
    - solution #2: use a different pin for this input (a good candidate is BNO_INT, since it is very close and easy to rework)

3. MCP73831 suffers from overheating, the charge current limit must be set to a lower value, like 200 mA. It can be achieved by replacing R4 with a 5k-10k resistor.
    - better thermal design and PCB layout is required 