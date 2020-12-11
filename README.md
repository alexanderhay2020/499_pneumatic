# MRI Compatible Actuator

<!-- ## Timeline of Project
<p>
    <center><img src="/design/schedule1.png" width="100%;" height="100%;" alt/></center>
</p> -->

## Scope & Motivation

The goal of this project is to design a device to provide a sensory stimulus in an MRI environment. The sensory signal generated from the stimulus will be traced using functional magnetic resonance imaging (fMRI). In individuals who have experienced a stroke this pathway is disrupted and can inhibit sensorimotor function. Tracing the sensory signal can help determine if the cause is from the signal not reaching the brain, or processing within the brain.

## Somatosensory System

#### Overview

The somatosenory system is the mechanism by which the body interprets touch, pressure, temperature, proprioception, and pain. Once a sensation is perceived, sensory information travels via the Dorsal Column-Medial Lemniscal (DCML) pathway, or  the anterolateral column (noxious/thermal)<sup>[1]</sup>. After traveling up the spinal column and reaching the medulla the sensory information decussates, continuing up through the thalamus, and terminating in the somatosensory cortex. The crossing in the medulla is why a stroke in the right hemisphere of the brain affects the left side of the body, and vice versa.

<p>
    <center><img src="/design/dcml.png" width="80%;" height="100%;" alt/>
    <br>
    <em>Figure 1: The dorsal column-medial lemniscus (DCML) and anterolateral pathways</em></center>
</p>

To deliver a stimuli to the finger it is important to understand how the stimuli will be received. In the human finger there are four such receptors; Meissner, Pacinian, and Ruffini, and Merkel cells. All four together encode touch sensory information that is sent to the brain.

Meissner and Pacinian corpuscles are both rapidly adapting cells, meaning they don’t respond to a sustained stimulus. Meissner’s corpuscles are sensitive to light touch while Pacinian corpuscles are sensitive to pressure and vibrations such as the sensations involved when handling an object. They are sensitive to higher frequency vibrations, peaking at 50 Hz and 200 Hz respectively.

<p>
    <center><img src="/design/receptors.png" width="100%;" height="100%;" alt/>
    <br>
    <em>Figure 2: Receptors in the fingertip and the information they transduce</em></center>
</p>

Merkel cells are sensitive to deep, static touches, and low vibrations (0-100Hz). They have a small receptive field and transduce detailed information about the surface they’re interacting with. Merkel cells signal the static aspect of a touch stimulus, such as pressure, whereas the terminal portions of the Merkel afferents in these complexes transduce the dynamic aspects of stimuli.<sup>[2]</sup>

## Design

### Requirements

The following requirements were placed on the project for the following justifications:

<b>MRI Compatibility</b> - The lab will use fMRI imaging during stimulation. The device must be able to operate within an MRI environment and should be made with non-magnetic materials such as brass, rubber, or plastic. Typical MRI scanners range in strength from 1.5T to 3T with research scanners reaching up to 8T. RF heating is also a concern and should be taken into consideration if conductive materials (including the body) are used.

<b>Documentation</b> - Information in this report will serve as a manual for the device. When appropriate, links to source files, CAD files, datasheets, and other supporting documents will be provided and cited. Rationales will also be provided for design decisions.

<b>Fabrication</b> - Due to the accessibility constraints placed by Covid-19 the prototype must be able to be fabricated locally, limiting specialized tools and equipment such as those found in a machine shop. That said, anyone with sufficient access to a lab or fab shop should be able to fabricate this device.

<b>Controller</b> - The controller will be based on the PIC32 microcontroller. The PIC32 provides a platform to exercise firmware level control. Anyone who is comfortable programming in Arduino should reasonably be expected to understand the functions of the controller. Any electrical components or sensors should be capable of interfacing with the PIC32 as it will act as the hub of the device.

### High Level Design

Five (5) methods of actuation were considered for this project; pneumatic, hydraulic, piezoelectric/ultrasonic, mechanical, and electromotive. MRI compatibility shaped most of the decision process, followed by ability to fabricate the device.

The necessity of the actuator operating in an MRI environment rules out many conventional methods of actuation. Traditional actuators like DC or stepper motors cause electromagnetic interference. If placed near enough to the scanner they will cause EMF noise. Even if placed outside the scanner room there is still nonzero electromagnetic interference caused by the motors. Piezoelectric and ultrasonic motors are more common for this application, however a drawback to using such motors is that the motors can’t be actuated while imaging is taking place, otherwise there will be artifacts in the image.<sup>[[3]](https://pubmed.ncbi.nlm.nih.gov/18982643/)</sup> There are methods that address this issue<sup>[[4]](https://pubmed.ncbi.nlm.nih.gov/19964890/)</sup> but fall out of the scope of the project.

<p>
    <center><img src="/design/snr.jpg" width="80%;" height="50%;" alt/>
    <br>
    <em>Figure 3: SNR results under 3T imaging</em></center>
</p>

Pneumatic and hydraulic systems provide an alternative means of actuation. Often there is an air source present in or near the scanner room. Air can be delivered using plastic or rubber tubing, and pneumatic fittings are commonly available in plastic. A pneumatic system can potentially be an electromagnetic interference free system. One of the issues that pneumatic systems face is the nonlinear behavior of compressed air. Hydraulic systems work around this issue at the cost of complexity and risk of leakage.

Mechanical devices can still be used if the proper precautions are made, but ultimately makes it a larger design problem than it needs to be.

<p>
    <center><img src="/design/2.png" width="100%;" height="100%;" alt/>
    <br>
    <em>Figure 4: System decision matrix</em></center>
</p>

I chose to go with a pneumatic system for three reasons; 1) it's inherently free of any metal, plastic hardware is readily available and commonly used, 2) you can infer the force applied by the piston if you know the dimensions of the piston, and 3) it eliminates leakage as a concern, since fabrication and testing would take place in my apartment.

### Somatosensory Considerations

As mentioned earlier, Merkel cells respond to fine pressure related stimuli. Figure 5 provides information on the properties of each receptor cell.
<p>
    <center><img src="/design/receptor_chart.png" width="100%;" height="100%;" alt/>
    <br>
    <em>Figure 5: Sensitivity information about the receptors</em></center>
</p>

Because the apparatus will press on the user’s fingertip It's also important to understand how forces (or pressure) travels through the finger. Figure 6 illustrates what happens to the tissue in the finger tip; pressure applied at the fingertip causes veins in the tissue to be compressed. This tissue compression is how the Merkel cells are stimulated.

<p>
    <center><img src="/design/squish.png" width="80%;" height="80%;" alt/>
    <br>
    <em>Figure 6: Normal force applied to finger tip</em></center>
</p>

The fingernail is intrinsically part of the force-load structure of the fingertip; “note that the compression extends to the area around the bone as well as beneath it. This is because the nail bed fibers are in tension and pull the nail down with the bone, compressing all the tissue that is around the bone but beneath the nail...” “...slowly adapting type I afferents from the sides and end of the finger respond to stimuli on the center of the fingerpad.”<sup>[[5]](https://my.mech.utah.edu/~smascaro/pdf/Mascaro-2002-Haptics-NailBone.pdf)</sup> The experimental apparatus used by Birznieks et. al has a similar function and caps the force applied to the fingertip at 4N.<sup>[[6]](https://pubmed.ncbi.nlm.nih.gov/11588194/)</sup>

It’s also important to understand whatever pain/damage thresholds exist in the body. Brennum et. al. investigated the pressure-pain threshold in 30 people [H08], and finds that pain can first be experienced when a pressure ~400kPa is applied to the fingertip. It should be noted that there’s some nuance to this, such as speed [H09] or probe shape.

## Apparatus

<p>
    <center><img src="/design/1.png" width="80%;" height="50%;" alt/>
    <br>
    <em>Figure 7: Main system components</em></center>
</p>

To create the sensory signal I designed a small piston that presses on the finger. I used Solidworks to model the the piston, controller housing, and relay housing, Cura for slicing, and an Ender3 Pro 3D printer. It's made with PLA filament, Fittings and ports were purchased online. Here are links for the [.stl files](https://docs.google.com/document/d/1iMdl5hpP_emYXVEchicDH41gPpMzWWeitnvOTYf4hh8/edit?usp=sharing) and [device manual](https://drive.google.com/drive/folders/13laoiJDyfBOlWCCcYVfk1w05I2iibtdy?usp=sharing). A PIC32 microcontroller controls the system programmed using MPLAB. The PIC communicates with a computer serially using Python.

<p>
    <center><img src="/design/cutaway.png" width="80%;" height="50%;" alt/>
    <br>
    <em>Figure 8: Cross section of the pneumatic actuator model</em></center>
</p>

An FTDI-USB cable provides a voltage source for the PIC and facilitates serial communication with a computer. The voltage from the regulator typically measures 3.36V. A separate 6V source is needed for the MIC2940 regulator to power the 5V peripherals. System status is displayed to a 320x240 LCD touchscreen. It monitors system pressure, sensor frequency, serial communications, and touch sensitivity, and can graph sensor data in real time.

<p>
    <center><img src="/design/2020_11_10.png" width="100%;" height="100%;" alt/>
    <br>
    <em>Figure 9: Controller</em></center>
</p>

Air is directed in and out of the piston using a solenoid controlled by the PIC. A 3-position solenoid from AutomationDirect is used to direct the airflow to/from the chambers of the pneumatic actuator (Figure 10). Air is exhausted to the environment through mufflers. It uses 2 coils to direct the air, each requiring 120V and command signal, necessitating the use of a relay. Air supplied to the actuator is also supplied to the controller received by the pressure sensor.

<p>
  <center>
    <a href="https://www.lunchboxsessions.com/materials/flow-directional-control-valves/directional-control-valve-simulation"><img src="/design/left-open.gif" width="40%;" height="100%;" style="margin-right:50px;"></a>
    <a href="https://www.lunchboxsessions.com/materials/flow-directional-control-valves/directional-control-valve-simulation"><img src="/design/right-open.gif" width="40%;" height="100%;"></a>
    <br>
    <em>Figure 10: Solenoid directing airflow. Graphic source: lunchboxsessions.com</em>
  </center>
</p>

## Circuit Diagram

<p>
    <center><img src="/design/circuit.png" width="100%;" height="100%;" alt/>
    <br>
    <em>Figure 11: Circuit diagram</em></center>
</p>

### Version Control

As development continued it became increasingly important to track changes. Each peripheral had its own dedicated branch. Being the sole developer made development rather linear, but it provided a good opportunity to flex some git knowledge and pick up new tricks, like `git stash` and `git stash pop`. Working peripherals were then merged into the main branch. This project also demonstrated how important version control is for CAD! It was easy to take for granted how convenient git made working on the firmware. Moving forward, I'd like to utilize a revision system for the associated CAD designs.

### Manual

[Manual](https://drive.google.com/drive/folders/13laoiJDyfBOlWCCcYVfk1w05I2iibtdy?usp=sharing)

## Discussion & Future work

During development I made it a priority that the device was MRI-compatible, which influenced many of the design considerations and challenges. The decision to use a pneumatic system made it so there is no interference from the apparatus during the MR imaging process. However, air is a compressible fluid which makes it difficult to control. A hydraulic system was considered to address that but was never implemented due to concerns about leakage.

That said, leakage still proved to be a nuisance throughout the project. Pistons of various sizes were printed in an effort to dial in the tolerances, but ultimately will require better surface finishing. In addition to O-Rings, piston rod rings were incorporated into the design to alleviate the issue. Though not unsuccessful, it was not utilized in the final design.

Future work with this project will primarily focus on characterization of the actuator, as well as making each element more robust and providing the framework for which development can continue. The two largest lessons were in time management, setting realistic expectations and goals, and project management, learning how to juggle multiple facets of a project and navigating the way forward.

The control loop has two pressure sensors and one touch sensor. An optical linear encoder using fiber optic cables was considered, which would've provided more certain feedback of the piston, but ultimately was shelved due to time constraints.

Knowing if/when the piston engages with the finger is also important. The controller currently takes advantage of the CTMU module on the PIC32. For it to work, the surface that the fingertip engages with must be conductive (and MRI-compatible, ie. aluminum foil) for it to work. Studies examining the heating effects of MRI scanning have shown that no significant heating occurs with nonferromagnetic materials.<sup>[[7]](https://onlinelibrary.wiley.com/doi/abs/10.1002/mrm.1910070302?sid=nlm%3Apubmed)</sup> This means the individual won't risk burning their finger. And because the peripheral is non-moving, there's no risk of interference during imaging<sup>[[3]](https://pubmed.ncbi.nlm.nih.gov/18982643/)</sup>.

## References

[1] Kandel, E. R., 2013. Principles of Neural Science. 5th ed.

[2] Purves, D., Augustine, G., Fitzpatrick, D., Hall, W., LaMantia, A., Mooney, R., Platt, M. and White, L., 2018. Neuroscience. 6th ed.

[3] Fischer GS, Krieger A, Iordachita I, Csoma C, Whitcomb LL, Gabor F. MRI compatibility of robot actuation techniques--a comparative study. Med Image Comput Comput Assist Interv. 2008;11(Pt 2):509-517. doi:10.1007/978-3-540-85990-1_61

[4] Wang Y, Cole GA, Su H, Pilitsis JG, Fischer GS. MRI compatibility evaluation of a piezoelectric actuator system for a neural interventional robot. Conf Proc IEEE Eng Med Biol Soc. 2009;2009:6072-6075. doi:10.1109/IEMBS.2009.5334206

[5] Mascaro S, Asada H. Understanding of fingernail-bone interaction and fingertip hemodynamics for fingernail sensor design. Proceedings 10th Symposium on Haptic Interfaces for Virtual Environment and Teleoperator Systems HAPTICS 2002. 2002. doi:10.1109/haptic.2002.998948

[6] Birznieks I, Jenmalm P, Goodwin AW, Johansson RS. Encoding of direction of fingertip forces by human tactile afferents. J Neurosci. 2001;21(20):8222-8237. doi:10.1523/JNEUROSCI.21-20-08222.2001

[7] Buchli R, Boesiger P, Meier D. Heating effects of metallic implants by MRI examinations. Magn Reson Med. 1988;7(3):255-261. doi:10.1002/mrm.1910070302
