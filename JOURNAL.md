# STM32 Motor Driver Development Journal

**Total time spent: 75 hours**

i was working on a four-motor-driver-board using the KiCad software, and the components used were eight BTS7960B, an STM32G431CBU6, and two AND gate chips.

This journal covers the design process right from symbols through the schematic, placements, routing, board previewing, parts budget, and even a simple firmware demonstration

## 8 November 2026 — Devlog 01 — setting up the KiCad symbols

**Total time spent: 3 hours**
the driver symbol and footprint must match on the output tab because that is a large copper pad that gives out the same output as pin 4

a project library was set up for the parts required for the motor driver
the symbol for BTS7960B shows OUT on pin 4 and the output tab on pin 8, while TO-263 footprint was designated
footprint preview was viewed to confirm that there are seven leads and one tab pad

![bts7960b symbol and footprint](journal%20image/j01_01_bts7960b-symbol-and-footprint.png)

also, SN74ACT08D was included in the library for PWM logic
the symbol appears to have four gates and one power supply, all in one SOIC-14 footprint
now that both symbols appear in the picker, placement of parts for Motor A can begin

![sn74act08d symbol and footprint](journal%20image/j01_02_sn74act08d-symbol-and-footprint.png)


## 8 November 2026 — Devlog 02 — drawing the Motor A driver

**Total time spent: 5 hours**

i constructed the channel in smaller segments such that i could trace the flow of current from the PWM input into the driver and to the motor connection pin

the motor power supply capacitor is on the rails of the battery, while the direction LEDs are on the two outputs of the motors

for Motor A in KiCad i started the design process by placing two BTS7960B chips from the project library 
one chip represents one half of the bridge, therefore the motor would connect from both the two outputs

![Two BTS7960B chips placed for Motor A](journal%20image/j02_01_two-half-bridge-symbols.png)

i connected the first BTS7960B chip U1 with VBAT+ on VS and the GND pin to GND
the MTRA_LPWM input connects to IN using 1kΩ resistor and 10 kΩ resistor on the signal pin  
the INH connects to 5V_IN using 1kΩ resistor and SR and IS each to GND using individual resistors

![The first half-bridge wired up with its resistors](journal%20image/j02_02_first-half-bridge-wiring.png)

i rewired the second half bridge U2 using MTRA_RPWM as its input  
both pin 4 and the tab at pin 8 have the same output net in the respective chips, OUTA1 in U1 and OUTA2 in U2  
output nets must remain separate for feeding the opposite sides of the motor

![Both Motor A half-bridges wired up](journal%20image/j02_03_both-half-bridges-wired.png)

i connected the 1000 µF and the 100 nF capacitors across VBAT+ and GND in this channel  
positive side of the 1000 µF capacitor is connected to VBAT+

![Motor power source capacitors connected across VBAT and GND](journal%20image/j02_04_motor-supply-capacitors.png)

i inserted two LEDs facing in opposite directions, with a 10 kΩ common resistor across the motor outputs  
the goal is to indicate the polarity of the voltage difference on the motor by lighting the appropriate LED

![Two LEDs facing in opposite directions across motor outputs](journal%20image/j02_05_direction-leds.png)

i connected a two-pin motor connector as J1 and called it MTR_A  
pin 1 connected to OUTA2 and pin 2 connected to OUTA1

![Motor A connector pin order](journal%20image/j02_06_motor-output-connector.png)

This is the Motor A section altogether, along with the drivers, capacitors, LEDs, and connector outputs.
The PWM labels are ready for the AND gates, although the AND gates themselves still have to be done.
Before making a copy for the other sections, I still have to test the operation of the bridge at startup and with zero PWM while INH is high.


![Motor A schematic overview](journal%20image/j02_07_motor-a-schematic-overview.png)

## 8 November 2026 — Devlog 03 — copying Motor A to the other channels

**Total time spent: 3 hours**

Copying the channel involved repeating the process of input labels, output labels and connector renaming; a label copy could connect two channels even though the blocks appeared to be distinct from each other on the screen.

I copied the Motor A driver to drive Motor B
This was the new pair of blocks - U3 and U4, and OUTB1 and OUTB2 went to the LEDs and J2 connector
As seen in this screenshot below, the inputs were still labeled MTRA_LPWM and MTRA_RPWM because I had copied Motor A; hence, there was need to rename them to distinguish the channels.

![Motor B copy with its own output labels](journal%20image/j03_01_motor-b-first-copy.png)

After that, I created Motor C using U5 and U6
I named the inputs as MTRC_LPWM and MTRC_RPWM, and output pins were OUTC1 and OUTC2
I used 1000 µF and 100 nF capacitors on both VBAT+ and GND, and LEDs across the motor outputs



![Motor C half-bridges, capacitors and connector](journal%20image/j03_02_motor-c-channel.png)

I added Motor D with U7 and U8 and zoomed out to view the entire diagram of all four drivers.
The latest copy of the KiCad history file has PWM labels that are different for channels A, B, C, and D, along with a total of eight half-bridges.
The connectors are J1 to J4, however, their value is still labeled MTR_A, and therefore J2 to J4 must be properly labeled before going for another review.

![Schematic showing four motor driver blocks](journal%20image/j03_03_four-channel-schematic.png)

The power labels are common to the four blocks, and every motor has its own output nets.
My next step is to make a copy of the schematic design, implement the PWM and direction logic, and check the connection wires before going into the PCB design process.

## 8 November 2026 — Devlog 04 — adding the PWM and direction logic

**Total time spent: 3 hours**

- The two gates for each motor use the same PWM signal, but the separate direction signals choose which input to the driver gets the signal

- I made the timer and direction labels visible so that I could easily identify the nets used when going from the MCU to the gates and the drivers

- I placed the first SN74ACT08D IC, as U9, for both motors A and B
- KiCad displays the four AND gates as units A-D, and the power pins as unit E
- The five units all reside on the same chip

![U9 gate units and labels for the first time](journal%20image/j04_01_motor-a-b-gates-and-labels.png)

- I connected the two Motor A gates to the same timer signal MTRA_T8C1
- DIR1 connects to the gate which produces MTRA_RPWM, and DIR2 to the gate which produces MTRA_LPWM
- I did the same thing for Motor B using MTRB_T15C1 and the direction labels, then connect pin 14 to 5V_IN and pin 7 to GND

![Motor A and B logic wired to their PWM nets](journal%20image/j04_02_motor-a-b-logic-wired.png)

i added C9, 100 nF capacitor between 5V_IN and GND for the U9 power supply  
i have to place it close to the chip’s power pins when i get to the PCB  

![C9 supply decoupling capacitor](journal%20image/j04_03_u9-decoupling-capacitor.png)

i added U10 for motors C and D, along with the other 100 nF capacitor, C10  
Motor C operates by using MTRC_T16C1 for both gates and with different DIR1 and DIR2 inputs  
in the current screenshot Motor D labels are being placed and labels of DIR and PWM output are switched in units C and D

![Motor C wired and Motor D labels in progress](journal%20image/j04_04_motor-c-d-logic-in-progress.png)

i zoomed out to see both chips and both supply capacitors  
before continuing i have to make sure that timer and DIR inputs are going to Motor D with MTRD_RPWM on pin 8 and MTRD_LPWM on pin 11  
after that i will check whether all eight PWM outputs correspond to the labels on the motor driver inputs

![Both PWM logic blocks on the schematic](journal%20image/j04_05_pwm-logic-overview.png)

## 8 November 2026 — Devlog 05 — wiring the STM32 and its 3.3 V supply

**Total time spent: 5 hours**

Adding the bare chip required me to add the power wiring that is present in the reference module

I also needed to account for the pins VDD, VDDA, VREF+, and the backup supply pin along with the signal pins; the small capacitors near the above-mentioned pins would be incorporated in the center PCB location later

The placement of the bare STM32G431CBUx symbol as U11  
The reference uses a WeAct module, so I had to follow the signals by GPIO name and connect the power wiring of the chip

![Bare STM32 symbol before wiring](journal%20image/j05_01_bare-stm32-symbol.png)

I included the names for the four PWM timer outputs, direction signals, and the encoder pairs  
I labeled PA0 as the pin for battery voltage sensing, PA11 and PB9 for CAN, PB10 and PB11 for USART3, and PA15 and PB7 for I2C1  
At this stage, all the power pins were still available

![STM32 with motor, encoder and communication labels](journal%20image/j05_02_stm32-signal-labels.png)

Three status LEDs were drawn in pins PA2, PA3, and PB1  
Each of these LEDs had a separate 220 Ω resistor to GND so the MCU could control each of them separately

![Three GPIO status LEDs and their resistors](journal%20image/j05_03_status-leds.png)

i tied the three VDD pins and the STM32 backup supply pin to 3V3_MCU  
i used a 100 nF capacitor for each VDD pin and one common 4.7 µF capacitor  
VDDA is using the same 3.3 V rail with 10 nF and 1 µF to GND and VREF+ is connected to VDDA using 100 nF and 1 µF capacitors  
I also connected VSS and pad 49 to GND

![STM32 power pins, ADC reference and decoupling](journal%20image/j05_04_stm32-power-and-decoupling.png)

it depicts the STM32 microcontroller and status LEDs on the board  
3V3_MCU needs to be connected to its regulator somewhere else in the schematics  
next thing is to make some space for SWD connector, as PA13 and PA14 are left as unused in this drawing and connect PG10 for reset

![STM32 block and status LEDs overview](journal%20image/j05_05_stm32-and-leds-overview.png)

## 9 November 2026 — Devlog 06 — adding the battery input and signal headers

**Total time spent: 4 hours**

The headers gave the encoder and communication signals some place to connect externally from the board, so their names had to match the names of the signals in the MCU section

I sketched the motor battery input connection with J5 pin 1 at VBAT+ and pin 2 at GND  
I sketched D12 at the supply input for the TVS, with the cathode connected at VBAT+, and a power LED with a 10 kΩ resistor  
The value of D12 still is generic, the actual component should be SMBJ18A

![battery input](journal%20image/j06_01_battery-input.png)

I sketched four encoder connectors with 3V3_SENSOR at pin 1, the two encoder signals at pins 2 and 3, and GND at pin 4  
Motor A’s connector names still are ETCA instead of ENCA in this schematic, so they have to match the names on the STM32

![encoder headers](journal%20image/j06_02_encoder-headers.png)

I sketched USART3 and I2C headers with one supply pin, two signal pins and one return pin  
The I2C names still have to match I2C1_SDA and I2C1_SCL on the MCU, and the headers have GNDS that have to connect to the common GND of this design

![uart and i2c headers](journal%20image/j06_03_uart-and-i2c-headers.png)

I then added J12 with CAN_L at pin 1, CAN_H at pin 2 and GND at pin 3
This is the bus connector, and the transceiver connector comes next

![can bus header](journal%20image/j06_04_can-bus-header.png)

I made sure that I looked into how the battery input and the signal headers relate to the STM32 component
This indicates how the other components fit onto the sheet and which nets are still pending

![input and headers overview](journal%20image/j06_05_input-and-headers-overview.png)


## 9 November 2026 — Devlog 07 — adding the CAN module header and power circuits

**Total time spent: 4 hours**

at this stage, I managed to put power inputs, sensors connections and battery sensing circuit on one schematic sheet

as the regulator output and sensors supply lines were denoted separately, having an almost finished schematic left a few power supplies to verify

added connector J13 as a six-pin header for CAN transceiver module  
with the following pins: 3V3_SENSOR, GND, CAN_TX, CAN_RX, CAN_H and CAN_L  
the module itself is yet to be selected and its pinout checked

![CAN transceiver module header](journal%20image/j07_01_can-transceiver-module-header.png)

created battery sense voltage divider with 39 kΩ resistance on top, 10 kΩ on bottom and 100 nF capacitor between lower resistor  
the middle tap connects to ADC1 on PA0 pin, upper label is marked as VAB+ on the screenshot and should be VBAT+ to get to battery rail

![battery voltage divider](journal%20image/j07_02_battery-voltage-divider.png)

J14 created for 5 V logic input supply with D14 diode in series and power LED through 1 kΩ resistor  
diode D14 is currently represented as generic diode, the reference suggests Schottky diode


![five volt logic input](journal%20image/j07_03_five-volt-logic-input.png)

i added AMS1117-3.3 with 5V_IN on pin 3, 3V3_MCU on pin 2 and GND on pin 1  
this adds an additional rail for the bare STM32 apart from the motor rail

![regulator for STM32](journal%20image/j07_04_ams1117-regulator-wiring.png)

i added one 22 µF capacitor from each side of the regulator to ground  
i have yet to find out the output capacitor for this particular regulator and to determine the input voltage left after the series diode  

![capacitors at regulator input and output](journal%20image/j07_05_regulator-input-and-output-capacitors.png)

i zoomed out to see the motor drivers, gate logic, STM32 and all the added connectors and supply blocks  
the motor connectors are labeled MTR_A to MTR_D

![full schematic draft](journal%20image/j07_06_full-schematic-draft.png)

this is the draft version of the schematic i have done, i still need to provide power to 3V3_SENSOR, correct the nets, check Gate inputs and outputs for Motor D and include SWD before running the final electrical rules check

![full schematic draft](journal%20image/j07_06_full-schematic-draft.png)

## 9 November 2026 — Devlog 08 — cleaning up the schematic connections

**Total time spent: 3 hours**
i kept the previous and new screenshots to check which connection changed, particularly with regards to the encoder header and battery divider

i went back to the open connections and label typos found from the previous schematic analysis  
first of all, i looked at Motor D, which had some labels positioned away from the AND gate pins

![Labels of Motor D before connecting the other wires](journal%20image/j08_01_motor-d-labels-before-wiring.png)

i added the input wire for U10C and U10D  
in this schematic, the labels of DIR and PWM are still misplaced, meaning that more work is required to finish wiring the motor 
DIR1 and DIR2 must be connected to a gate together with MTRD_T17C1, while MTRD_RPWM comes from pin 8 and MTRD_LPWM comes from pin 11

![Motor D gate wiring but with the labels that need to be changed](journal%20image/j08_02_motor-d-gate-wires.png)

i went on to the battery divider where the label on top was called VAB+  
the name was different from the name of the rail, VBAT+, of the battery input

![Battery divider before fixing the net name](journal%20image/j08_03_battery-divider-label-typo.png)
i edited the net label from VBAT to VBAT+ so the top part of the divider uses the motor battery net  
the 39 kΩ, 10 kΩ resistors, and 100 nF capacitor remained untouched around the ADC1 node  

![Battery divider with label VBAT+](_assets/images/journal/j08_04_battery-divider-label-fixed.png)

i corrected the Motor A encoder header labels from ETCA to ENCA  
J6 now has labels ENCA_T1C1 and ENCA_T1C2, which match the names of the STM32 pins  

![Motor A encoder header with corrected labels](_assets/images/journal/j08_05_encoder-a-labels-fixed.png)

i also aligned the I2C header with I2C1_SDA and I2C1_SCL on the MCU  
now both USART and I2C headers use the same GND symbol  

![USART and I2C headers with common ground](_assets/images/journal/j08_06_uart-and-i2c-labels-fixed.png)

i added the +3V3 symbol to the 3V3_SENSOR net  
the name of the regulator output is still 3V3_MCU, hence, i have to connect the sensor net to the output if both nets are supposed to be fed by the U12  
the most recent changes can be seen in the KiCad history copy, i still need to make the main schematic and run the electrical rules check



![Sensor supply label attached to the +3V3 symbol](journal%20image/j08_07_sensor-supply-label.png)

## 9 November 2026 — Devlog 09 — fixing the ERC power errors

**Total time spent: 3 hours**


the power flags allowed ERC to determine where the external power feeds enter the circuit

i still had to trace out the wiring and nets themselves in addition to the flag because it only determines how the checker treats the given net

after resolving the Motor D gates, i ran the Electrical Rule Checker in KiCad
the first run reported four power input errors for nets without sources as well as one warning  
there were two errors referring to VS pin of the first motor driver and VCC of the first AND chip

![](journal%20image/j09_01_erc-power-input-errors.png)

i marked VBAT+ with the PWR_FLAG so KiCad would know that the motor rail is supplied through the external battery connector

![](journal%20image/j09_02_vbat-power-flag.png)

i added another flag to 5V_IN which is the rail supplied through the external logic power connector and the series diode

![](journal%20image/j09_03_5v-input-power-flag.png)



i inserted a power flag at the junction of +3V3 and 3V3_SENSOR
A power flag instructs KiCad that the net is powered for ERC, but a physical power connection must be made for the sensor rail

![Power flag on the sensor rail net](journal%20image/j09_04_sensor-rail-power-flag.png)

i reran ERC, and the number of errors reduced from four to zero 
However, at the bottom of the window, there is one warning, and under the ignored tests tab there are four tests, which i must check before declaring the schematic complete


![Final ERC screenshot showing zero errors and one warning](journal%20image/j09_05_erc-zero-errors.png)

## 11 November 2026 — Devlog 10 — assigning footprints and importing the PCB

**Total time spent: 4 hours**

footprint decisions defined the size and pitch of the pad for the PCB

footprints for motor screw terminals, battery connector and signal headers were all different, so the body sizes and pitch became important when making the parts list

for the resistors i used 0805 footprint in batches, which includes small resistors near the drivers and LEDs

![resistors footprints](journal%20image/j10_01_resistor-footprints.png)

for the small capacitors i used 0805 footprints while the big capacitors had different footprint

![ceramic capacitors footprints](journal%20image/j10_02_ceramic-capacitor-footprints.png)

as for the 4 motor supply capacitors, each 1000 µF, i decided to use CP_Elec_10x10.5 footprint, but the capacitor bodies themselves should fit this footprint

![bulk capacitors footprints](journal%20image/j10_03_bulk-capacitor-footprints.png)

for the motors i used screw terminals, for the battery input i used XT60 connector and 2.54 mm signal headers  
also i assigned diodes and LEDs footprints and the 5.08 mm screw terminal for the logic input


![connector and diode footprints](journal%20image/j10_04_connector-and-diode-footprints.png)

i updated the PCB with reference to the schematic, and the import window showed no warnings and no errors  
routing and board checks will come after this import of footprints

![results of updating the pcb](journal%20image/j10_05_results-of-updating-the-pcb.png)

footprints were imported in stacks and there were thin lines to show the connections  
i had 113 footprints in the PCB design that i needed to spread before routing

![importing first footprints](journal%20image/j10_06_importing-first-footprints.png)

i maintained the footprints of TO-263 with pad 8 as the output tab for the eight BTS7960B devices  
SOIC-14 is used by the gate chips, QFN-48 with an exposed pad for the STM32 and SOT-223 with tab at pin 2 for the regulator

![ic footprint list](journal%20image/j10_07_ic-footprint-list.png)

## 11 November 2026 — Devlog 11 — placing the motor drivers and starting the tracks

**Total time spent: 5 hours**

I began by placing the short traces related to the driver ICs first before working my way all over the board

Having the resistors placed near the driver pins made it easy to link up each repeated circuit while placing the motor connectors near the periphery leaves the center available for the logic and MCU circuits

I laid out the imported components so that I could place the driver ICs, motor connectors and large capacitors as distinct groups
The majority of the traces were still just lines at this point

![Spread out components](journal%20image/j11_01_parts-spread-out.png)

I placed the small resistors close to U1 and started routing the short traces between the pin and resistor pads
The large pad on the left is OUTA1 which is separate from the VBAT+ and ground

![U1 resistors and tracks](journal%20image/j11_02_u1-resistors-and-tracks.png)

I followed the same resistor layout near the other driver footprints

![resistor placement on drivers](journal%20image/j11_03_resistor-placement-on-drivers.png)

I shifted the terminals of Motors A and B to the outside area adjacent to their drivers and began routing the outputs to the terminals.

![motor a b terminal placement](journal%20image/j11_04_motor-a-b-terminal-placement.png)

I positioned the A and B pair to the left and the C and D pair to the right with the motor terminals oriented to be external  
This allowed some open space in the center for the controller and signal terminals

![driver banks on both sides](journal%20image/j11_05_driver-banks-on-both-sides.png)

I viewed the 3D layout to check how the two driver banks look with the screw terminals along their edges  
This was a placement preview with tracks remaining to be routed.

![first driver placement 3d view](journal%20image/j11_06_first-driver-placement-3d-view.png)

## 11 November 2026 — Devlog 12 — placing the STM32 and routing motor outputs

**Total time spent: 5 hours**

there are much more tiny signals for the central controller than for the outer motor terminals

i handled both these regions individually in the editor, inspecting the tiny QFN pads at close-up and zooming out to get an idea about their connections to the other elements of the layout

i positioned the STM32 in the center and moved its tiny capacitors and one of the AND chips to be closer to it  
i began routing some signals from the QFN pins to the logic chip while the header nets were not yet routed

![stm32 and logic routing](journal%20image/j12_01_stm32-and-logic-routing.png)

i zoomed out to have an idea of the placement of the two driver banks and the central components 
the guide lines were useful for identifying which clusters should be moved closer to each other or need to be routed

![placement overview with unrouted lines](journal%20image/j12_02_placement-overview-with-unrouted-lines.png)

i worked with the Motor A connector and the LED network around the IC U1 and U2  
two terminal pads of the connector are OUTA2 and OUTA1, and their tracks go to different output nets


![motor a output routing](journal%20image/j12_03_motor-a-output-routing.png)

the wider view shows tracks at the motor terminals on both sides and around the controller  
i still had connections to finish between the centre and the outer driver groups

![motor outputs and central logic](journal%20image/j12_04_motor-outputs-and-central-logic.png)

## 12 November 2026 — Devlog 13 — tightening the placement and checking it in 3D

**Total time spent: 3 hours**

The 3D views helped me compare the tall components to the small controller components where the large capacitors and screw terminals occupy a lot of space height-wise, whereas the controllers and logic fit tightly close to the PCB surface.

I moved the large capacitors closer to their respective driver groups and placed the battery input into the central area of the design.

![Large capacitors and battery input location](journal%20image/j13_01_bulk-caps-and-battery-input-placement.png)

I moved the headers and the small control components into the empty space between the left and right motor groups.
This particular view still displays the guidelines between the controller, headers, and drivers.

![Headers and Control update](journal%20image/j13_02_updated-headers-and-control-layout.png)

I checked the top view design by including motor terminals, four large capacitors and small headers.
STM32 footprint was positioned at the middle section of the board, not far from the external motor connectors.

![Top 3D placement check](journal%20image/j13_03_top-3d-placement-check.png)

I tilted the board to see how tall are screw terminals, bulk capacitors and headers.
It is helpful for the cable access planning and the height above the PCB.


![angled 3d placement check](journal%20image/j13_04_angled-3d-placement-check.png)

## 12 November 2026 — Devlog 14 — adding the outline and checking routing progress

**Total time spent: 4 hours**

The routing counts provided me with the possibility to evaluate the performance of the layout before and after filling copper

There were still 91 unrouted connections after that stage, which means that more work was required between the placed groups despite the fact that the board outline and filled regions were already done

I drew the rectangular outline for the board and evaluated the locations of the motor terminals near the borders of the layout

![rectangular outline of the board](journal%20image/j14_01_rectangular-board-outline.png)

The count from KiCad showed 414 pads, 107 nets, 252 track segments and 5 vias  
There were still 158 unrouted connections after that stage

![Routing before copper fill](journal%20image/j14_02_routing-before-copper-fill.png)

I filled copper around the layout and evaluated the numbers in the counter again  
There were 91 unrouted connections and 252 track segments, which means that 67 less connections required manual routing
However, the connection of pads by filling copper does not mean that 67 more track segments were manually created

![Front copper fill and count](journal%20image/j14_03_front-copper-fill-and-count.png)

I changed the view to the back copper and analyzed the copper fill around the pads and board outline


![back copper fill](journal%20image/j14_04_back-copper-fill.png)

i looked at the board in 3D again to check the outline against the four motor terminals and the central parts

![board outline 3d check](journal%20image/j14_05_board-outline-3d-check.png)

## 12 November 2026 — Devlog 15 — setting up four layers and importing more routing

**Total time spent: 7 hours**

i decided to keep the setup of layer, zone settings, and routing result altogether because it would be easier to understand the changes made

The routing session created some tracks and vias for the zones which were already placed, and the filled copper connected those pads that belonged to zone nets

The total number of connections left decreased from 91 to 13; however, the next step should be performed by carefully checking the area near the MCU and headers

The zone of +3V3 should be also checked against the net name of the regulator because the zone itself does not create the connection with 3V3_MCU

i decided to configure my board as a four-layer board with 1.6 mm thickness
the setup shows the thickness of 0.035 mm copper in each layer and FR-4 between them

![four layer stackup](journal%20image/j15_01_four-layer-stackup.png)

i configured a GND zone for F.Cu, B.Cu and In2.Cu
screenshot shows 0.5 mm clearance, 0.25 mm minimum width and 0.5 mm thermal gaps and spokes


![ground zone settings](journal%20image/j15_02_ground-zone-settings.png)

i set In1.Cu to the +3V3 net in the copper zones settings  
i still need to check whether it is the correct 3.3 V net because the label of the voltage regulator output is 3V3_MCU

![copper zones settings for inner 3 V zone](journal%20image/j15_03_inner-three-volt-zone-settings.png)

i filled the zones and looked at how well they cover the board in the area near the drivers, controller, and connectors

![filling zones on four layers](journal%20image/j15_04_four-layer-zone-fill.png)

i opened a routing session and KiCad informed me that it was merged correctly  
now i can see the tracks from the centre of the board to the drivers and connectors on several layers in the routing view

![routing view after session import](journal%20image/j15_05_routing-after-session-import.png)

the last screenshot shows 579 track segments and 71 vias, 414 pads, and 107 nets  
13 unrouted connections left, whereas there were 91 before the filling with 327 more track segments and 66 more vias than in Journal 14
the routing import is saved in the KiCad history copy, i still need to save the current version of the board to the main PCB file and complete the routing


![final routing count](journal%20image/j15_06_final-routing-count.png)

## 13 November 2026 — Devlog 16 — finishing routing and checking the board

**Total time spent: 5 hours**

I used the DRC list to navigate between the other problem areas and the close up views of the PCB

The last screenshot shows the status of the project at this point, the final artwork needed yet another check since it altered the graphics that would be printed on it afterwards

I kept on routing the connections around the MCU, the tracks around the smaller pads were too crowded, so I checked them closer

![MCU routing](journal%20image/j16_01_mcu-routing.png)

After another round of routing, the importation of the board resulted in 666 track segments and 81 vias, only 5 connections remained unrouted

It was 87 more track segments and 10 more vias compared to the previous journal entry

![Routing count](journal%20image/j16_02_routing-count.png)

I launched the design rule checker and it still reported the missing connections and incomplete thermal reliefs, the first report contained 6 unconnected items and 166 warnings

![First DRC](journal%20image/j16_03_first-drc.png)

I routed around the MCU power pins and UART tracks, the close view helped me visualize the vias and the way the 3V3_MCU tracks connected the pads

![mcu power tracks](journal%20image/j16_04_mcu-power-tracks.png)

another check revealed the unconnected items to be 2; one was between the ground zones, and the other was between the J8 and the +3V3 zones

![remaining connections](journal%20image/j16_05_remaining-connections.png)

I performed an analysis of the +3V3 connections at the headers and focused on the copper connections at these pads

![header power routing](journal%20image/j16_06_header-power-routing.png)

After addressing the remaining connections and the thermal relief issues, the final check revealed no errors and unconnected items

There were 166 warnings, which included the silkscreen clearance and silkscreen clipped by the board edge, and schematic parity was not run, while 5 checks were ignored

![zero errors](journal%20image/j16_07_zero-errors.png)

## 13 November 2026 — Devlog 17 — adding artwork and checking the 3d board

**Total time spent: 1 hour**


i reviewed the board in its entirety; the four motor sections are placed on the edges, while the controller and logic are situated between them.

![routed board](journal%20image/j17_01_routed-board.png)

i opened the 3D viewer for a glimpse of the drivers, big capacitors, and connectors. This way, it was much easier to examine the spacing and outline of the board.

![first 3d view](journal%20image/j17_02_first-3d-view.png)

then i added small drawings to the front silkscreen and examined how they would look among components in the 3D view.

![front artwork](journal%20image/j17_03_front-artwork.png)

later i added bigger character drawings and web design to the back side and rotated the board in the 3D viewer to review the artwork around the holes.

![back artwork](journal%20image/j17_04_back-artwork.png)

also i checked the back artwork in the PCB editor; since the pads and board outline were visible there, i could understand the location of the drawings.


![back editor](journal%20image/j17_05_back-editor.png)

the last screenshot shows the front artwork with the routing and component outlines

these are design previews, i still need to check the silkscreen clearances again after adding the drawings

![front editor](journal%20image/j17_06_front-editor.png)

## 13 November 2026 — Devlog 18 — checking the pcb quote and making the bom

**Total time spent: 3 hours**

The BOM used parts and footprints from the KiCad design, using the STM32 chip and the connector sizes that I chose

There were some parts whose packages differed, and therefore their purchase links needed to be verified using the footprints

The price of the PCB is for the five ordered boards, whereas the parts quantities are for just one assembled board

I uploaded my PCB files on JLCPCB and verified the preview for the two sides

The quote is for a four-layer board at around 162.5 mm by 57.4 mm with five boards chosen and thickness at 1.6 mm, green solder mask and white silkscreen

The online price shown was 30.80 USD, which consists of an engineering fee of 25 USD and 5.80 USD for the boards


![pcb quote](journal%20image/j18_01_pcb-quote.png)

I created the bom from the KiCad parts list and the previously referenced bom, which includes names, references, board quantities, order quantities, unit prices, and buying links.

I kept order quantities visible since certain parts have minimum orders. The file has 113 entries and includes 30.80 USD for PCB orders.

The total price was 88.28 USD, but C20 requires capacitor specifications to be verified, while shipping and other taxes were not included in the price.

This way, I have the parts budget for my project, and the screenshots below demonstrate the quote and bom stages; the hardware has yet to be assembled.

![bom](journal%20image/j18_02_bom.png)

## 13 November 2026 — Devlog 19 — making the motor test firmware

**Total time spent: 5 hours**

since the sketch has the PWM and direction pins stored together per motor, the mapping of the four channels becomes very easy for comparison with the board

the sketch checks the elapsed time with millis in order to receive the stop command during the test pulse; additionally, it does not consider the line ends coming from the serial terminal

while the motor is in operation, it ignores another motor command, and after receiving a new command following the test pulse, it starts another test

i have created a small sketch in Arduino for STM32 to test the four motor outputs of the board

each motor has one PWM and two direction pins, and i have chosen the pin numbers from the PCB layout in order to have the correct channels A to D mapping

at startup, the sketch initializes the motor control pins low and checks the selected board support PWM on all four pins, the UART connection happens at 115200 baud rate


![motor pin map and setup](journal%20image/j19_01_motor-pin-map-and-setup.png)

testing begins by sending a message through UART, `a` to `d` selects a motor and rotates it forward, `A` to `D` rotates it backwards

every command runs one motor for 500 milliseconds at around 16 percent PWM value, after that the sketch stops the PWM and direction signals, I can also use `s` to halt the process early

![UART commands and motor test timer](journal%20image/j19_02_serial-commands-and-timed-test.png)

i stored the sketch in the firmware folder called [motor_test.ino](firmware/motor_test/motor_test.ino)

screenshots below show the location and the code of the sketch, still need to compile the sketch for STM32 and try it on the actual board

![firmware folder](journal%20image/j19_03_firmware-folder.png)

