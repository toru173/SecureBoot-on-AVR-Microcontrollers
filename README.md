# SecureBoot on AVR Microcontrollers

 SecureBoot on AVR Microcontrollers implimented as part of the work sample for a job application as below:

## 1 Background


  Firmware security is unique in its responsibility to both identify and
  fix architectural security vulnerabilities. More concretely, members
  of this team are expected to be competent low level C (and assembly)
  developers, with the ability to navigate new codebases and contribute
  functional code (normally a security feature or enhancement) in a
  relatively short amount of time.  In other words, firmware security
  engineers have to be a hybrid of both software engineer and security
  engineer.

  The code bases that firmware security deals in include many resource
  constrained microcontrollers with limited RAM, processing power,
  archaic toolchains, etc.  Adding security features to such
  environments is often an exercise in engineering creativity and
  tenacity.

  This work sample has been designed to evaluate your ability to quickly
  adapt and contribute security features to these types of environments.


## 2 Project Overview

  "AVR" is a popular microcontroller architecture for electronics
  hobbyists. AVR microcontrollers such as atmega2560 and atmega328p are
  used as the basis for many of the "Arduino" kits. Due to this
  popularity, documentation, datasheets, simulators and toolchains for
  these microcontrollers are readily available.

  Despite their popularity, there is limited to no "security" used on
  these devices. Your task is to remedy this situation.

  Although you will be provided guidance for what security features to
  add, this is an open ended project so you are free to stray from the
  suggested course or add additional security features that were not
  asked for. Bonus points will be given for creativity, but reasonable
  justification and assumptions must be given for any "direction
  changes" or additional features that are added.

  It is expected that you will use an open source AVR simulator/emulator
  that supports atmega2560 or atmega328p. You are welcome to target
  either the 328p or the 2560. You only have to target one of
  them. There are pros and cons of each target. The 328p has less
  resources but is less complex. The 2560 has more resources but is more
  complex. They are relatively similar so it is possible (without much
  pain) to switch targets half way through the project if you decide you
  prefer one over the other.


## 3 AVR secureboot

  The primary objective for this work sample is to implement
  "secureboot" on one of the AVR microcontrollers described above. It is
  recommended you complete this before attempting any additional "bonus"
  tasks.

  atmega328p/atmega2560 each use separate regions of flash for the
  bootloader and the main application.  On the atmega2560, the
  bootloader is the first code to run on the cpu and is typically
  responsible for listening to serial port activity for purposes of
  reflashing and recovery. If the bootloader detects that the main
  region of flash is valid, it will jump to it (address 0x00000000). If
  the flash is invalid, it typically waits for some protocol over serial
  port (avrdude for instance) to recover application part of the flash.

  Your task is to implement "secureboot" by having the bootloader
  cryptographically verify the contents of the application part of the
  flash before jumping to it. The hypothetical attack model we are
  working with here is, the attacker can reflash the application part of
  the flash, but not the bootloader part of the flash. Hence the
  bootloader is our "root of trust" and must maintain the integrity of
  the boot chain. You are free to choose the cryptographic primitives
  you use to achieve this, but you must elaborate the pros/cons of your
  decision.

  In addition to implementing this, you must measure any performance
  "overhead" associated with your changes. For instance, how much longer
  does it take to jump to the main application, how much additional
  flash memory is used, etc. Bonus points will be given for being able
  to minimize this while still maintaining a reasonable security
  posture. It is expected that you will elaborate on the tradeoffs you
  made with respect to performance vs security.

  Most likely, you will need to deliver three different pieces of code
  - The main application code that does something trivial, for instance,
    printing "hello world"
  - The boot loader code which verifies the integrity of the main
    application before executing it
  - A modified "simduino" code, based off of avrsim board_simduino. This
    will drive the simulation, loading your bootloader into the memory
    map etc.

  Project execution may look like: ./my_simduino <bootloader binary>
  <application binary> OR ./my_simduino <raw_flash_image.bin>


## 4 Bonus Tasks

  Secure Debugging - Have your secureboot capable bootloader chain to an
  application that implements "secure debugging."  Imagine a scenario
  that we want to diagnose some failure of a device in the field via a
  debugging capability, but we do not want this debugging capability to
  be used by attackers in the wild.  The debugging capability should
  provide the ability to read and write memory over the serial port.
  Describe how you would implement this, why it's secure from being
  abused by attackers. Implement it if possible.

  Exploit mitigations - Conventional exploit mitigations include stack
  canaries, non executable stack, ASLR, etc. None of these are present
  on AVR. Which of the conventional exploit mitigations could be
  implemented on AVR? Which could not, and why? Implement the ones that
  you think can be made to work.

  Encrypted firmware - Assume we are interested in protecting whatever
  secrets are locked inside the application part of the firmware (that
  bootloader normally jumps to). Can you encrypt this part of the flash,
  and have the bootloader decrypt and then execute it, in order to
  prevent prying eyes from reverse engineering our super secret
  application code?


## 5 Deliverables and Expectations

  The following is expected upon completion of the project:
  - Full source code for all completed projects
  - Clear instructions for how to compile and run your completed
    projects
  - Source code for projects/tasks you attempted but did not complete
    - Describe where you got stuck and what you think was going wrong
  - A written description of how you approached each project and why
  - A written analysis of any "performance overhead" your security
    features added
  - A written description of what the attack surface is against your
    security features


## 6 Resources

  - [https://github.com/arduino/ArduinoCore-avr.git]
    - in particular, the "bootloaders" directory will be valuable to you
  - [https://github.com/buserror/simavr]
    - Look at the "board_simduino" example
  - [https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf]
    - Atmega2560 datasheet
  - [https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf]
    - Atmega328p datasheet
  - [https://www.pololu.com/docs/0J61/6.3]
    - Basic gudiance on compiling with avr-gcc for atmega


## 7 Tips

  - Use picocom to get serial out from your avrsim instance
