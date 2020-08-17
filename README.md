# 499 - Summer Project

## Timeline of Project
<p>
    <center><img src="/summer_project/design/schedule.png" width="100%;" height="100%;" alt/></center>
</p>

## Scope & Motivation

The goal of this project was to design an MRI-compatible device to provide a sensory stimulus. The sensory signal generated from the stimulus will be traced using functional magnetic resonance imaging (fMRI). In individuals who have experienced a stroke this pathway is disrupted and can inhibit sensorimotor function. Tracing the sensory signal can help determine if the cause is from the signal not reaching the brain, or how the signal is processed once it reaches the brain.

## Apparatus
<!--
### System Design
For this project I've chosen the Miga T220 SMA linear actuator. Actuation parallels human muscle; the Nitinol wire contracts when a current is passed through, just as muscles contract when activated by a neuron. Each actuator has a fan installed to increase how quickly the wire could relax. The T220 also has an onboard MOSFET switching and protection circuit, which cuts power to SMA wire when the output stage reached the End-of-Stroke contact.

<p>
    <img src="/assets/miga.jpg" width="50%;" height="50%;" alt/><img src="/assets/circuit_mosfet.png" width="50%;" height="50%;" alt/>
    <br>
    <em>left: Miga T220 SMA Linear Actuator, right: power protection circuit</em>
</p>

### Design

I choose the following design parameters for the project:

* Apparatus has a max length of 369mm<sup>[3]</sup>
* Minimum range of motion of 45 degrees
* Tricep, bicep, and branchialis muscles must be modeled
* Equilibrium point control model

The max length was determined as the average length of a male arm, measured from the shoulder to the elbow. The range of motion was a compromise between human ability (~150 degrees) and the mechanics of the system. Since the actuator has a very short throw they needed to be anchored to the "elbow" at very small distances to achieve any decent range of motion. To achieve 90 degrees the anchor point would need a radius of 4mm. For reference, a 1/4" shaft is 6.35mm (radius 3.175mm).

<p>
    <img src="/assets/chart1.png" width="70%;" height="70%;" alt/>
    <br>
    <em>chart shows relation between range of motion and actuator anchor distance</em>
</p>

Originally the project was to include modeling of all the major muscles in the arm; tricep, bicep, and brachialis. This was scraped towards the end for the presentation at the Museum of Science and Industry, the primary reasons being actuation speed, performance, and complexity. Stacking actuators is also a concern due to the fact that the mounting holes are part of the circuit and require plastic hardware. Limiting to two actuators simplifies the overall design and allows the fans the blow across more surface area, allowing the Nitinol wires to cool faster increasing actuation rate.

Equilibrium point control is an intuitive method of motor control proposed by Feldman, with the λ-model published in 1986. Motion of the joint is instigated by the reciprocal command, "which alters the relative activation of agonist and antagonist muscles to produce a shift in the equilibrium between the muscle force and external load"<sup>[[2]](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2342993/)</sup> While falling out of favor somewhat recently it still provides a means by which motion control can be achieved.

### Assembly
Solidworks was used for modeling and assembly. .stl files can be found [here](https://drive.google.com/drive/folders/16_4EUu6pd2tG_H0EAVOFKAJVKnEg8ENC?usp=sharing). The frame is made from six (6) pieces made from acrylic or plywood.

1/4" bearings were press fit into the holes after a bit of sanding. Fans are mounted to their respective sides, making sure the fan pushed air over the actuator. Likewise, the actuators were screwed into the middle bracket using plastic spacers.

<p>
    <center><img src="/assets/assembly.gif" width="50%;" height="50%;" alt/>
    <br>
    <em>Apparatus exploded view</em></center>
</p>


A 1/4" shaft was placed through the bearings with two (2) collars placed on the shaft, one on the outside of each bearing bracket. Another collar was placed in the center, between the bearing brackets. 3D printed hollow screws were connected to the actuators and collars with a high strength nylon filament. A turnbuckle mechanism was used to maintain tension in the system. The apparatus is only designed to work in tension reflecting how our own muscles are structured.

The Ultimaker3 3D printer was used to make the shaft collars and the special screws needed for the turnbuckle assembly. A full BOM can be found [here](https://docs.google.com/spreadsheets/d/1KzUj851-XKCJ2QYp6Q05hCkz7jHLwCAjlV-5XQmAJzQ/edit?usp=sharing)

<p>
    <img src="/assets/img01.jpg" width="80%;" height="80%;" alt/>
    <br>
    <em>Complete assembly of project</em>
</p>

### Electronics
An Arduino UNO was used to interface with the actuators and fans. The actuators require analog/PWM pins, the fans require digital I/O pins.

Each actuator operates nominally at 9v at 0.3A<sup>[[4]](http://www.migamotors.com/Media/Miga-T220-Data-Sheet-102218.pdf)</sup>. Each fan requires 5v 0.2A. The UNO requires 5.5v to operate drawing minimal current. A 9V 2A power supply was used to power the components.

## Code

A structure was created to link the actuator and opposite fan; controlling the opposite fan (ie. sending an activation signal to the "tricep" actuator and "bicep" fan) because the speed of actuation depends on the cooling rate of the Nitinol wire.

```
typedef struct {
  int fanPin;
  int actPin;
} Muscle;
```

Functions flex() and relax(), requiring the Muscle struct.
```
void flex(Muscle m){
  analogWrite(m.actPin, 1023);
  digitalWrtie(m.fanPin, HIGH)
}

void relax(Muscle m){
 analogWrite(m.actPin, 0);
 digitalWrite(m.fanPin, LOW);
}
```

Because the wires require some amount of time to cool, a delay is inserted.
```
flex(tricep);
relax(bicep);
delay(5000);
``` -->

## Future work

Knowing if/when the piston engages with the finger is also important. The PIC32 microcontroller has a peripheral called the Charge Time Measurement Unit (CTMU), allowing for capacitive touch sensing. The surface the fingertip engages with must be conductive (and MRI-compatible, ie. aluminum foil) for it to work. Studies examining the heating effects of MRI scanning have shown that no significant heating occurs with nonferromagnetic materials.<sup>[[1]](https://onlinelibrary.wiley.com/doi/abs/10.1002/mrm.1910070302?sid=nlm%3Apubmed)</sup> This means the individual won't risk burning their finger. And because the peripheral is non-moving, there's no risk of interference during imaging.

## References

[1] Buchli R, Boesiger P, Meier D. Heating effects of metallic implants by MRI examinations. Magn Reson Med. 1988;7(3):255-261. doi:10.1002/mrm.1910070302
