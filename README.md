# teasy bot
Teasy bot (short for "tea easy" robot) is a project to build an Arduino based device that preps tea, based on time (and maybe later temp) specific for each kind of tea.

v0.06 with enclosure, bilingual interface, menu for infusion time or predefined teas
[![latest version](https://img.youtube.com/vi/lFy_RQyiyvs/maxresdefault.jpg)](https://youtu.be/lFy_RQyiyvs)

## change log

### v.0.6

__what's new__
- EEPROM saves last settings and selections:
    - last selected predifined tea
    - last menu choice
    - last language setting
    - last selected infuse time
    
__known issues__
- remaining time print out still buggy (see issue tab)

### v.0.5

__what's new__
- predifined teas in external file ````teas.h````
- bilingual String Arrays in external file ````Strings.h````
- buggy time printing is fixed


### v.0.4

__what's new__
- bilingual English and German (predifined or hold +, -, BACK while battery status)
- battery status for later battery power
- menu selection for tea or time
- predifined teas

__known issues__
- remaining time print out sometimes buggy
- no battery included yet
- reset button not accesable
- calble management is horrible


### v.0.3

__what's new__
- change PIN setup for LCD and buttons
- encosure
- change button order ( BACK  -  +  NEXT)
- soldered to universal PCB
- improvise LCD socket with some PCB and pins

__known issues__
- calble management is horrible

### v.0.2

__what's new__
- fixed time variable type error
- LCD
- 4 buttons ( -  +  BACK   NEXT)
- start message
- added customization
    - infuse time menu
    - no of dips menu
- starting message
- print remaining time
- finish message

__known issues__
- enclosure missing

### v.0.1

__what's new__
- start button
- fixed servo routine
    + initial dipping
    + longer infusion time
    + retrieval
    
__known issues__
- no customization
- wrong infuse time 
    + wrong variable type 


## parts and materials

- Genuino / Elegoo Nano v3
- 3V Servo Motor 9g
- 4 random red push buttons
- 1 button for reset trigger
- LCD1602 Module (16x2)
- 1 Potentiometer 
- some universal PCB
- black crafting plastic board
- pipet tip box as inclosure
- wires


## code

teasy_bot.ino

### includes
- the start screen
- battery status screen (for batterypower later)
- switch case for the current mode / state
- menu for adjusting and setting
    + time
- menu for choosing tea presets
- prints remaining time
- methods for each step and menu print
- finishing message
- bilingual German and English changed with 3-button hold (+, -, BACK) during battery status
- saving of last selected settings in EEPORM

### missing
- implementing of millis()
- option for occational moving of the tea bag

### potentially
- adjustment for cup height


## wiring and schematic (fritzing)

![teasy bot schematic](/img/wiring_teasy_bot_schem.png)

![teasy bot wiring](/img/wiring_teasy_bot_bb.png)


## video archive


v0.02 with display and interface for infusion time and number of initial dips
[![Watch the prototype video](https://img.youtube.com/vi/etB9GL0f5tQ/maxresdefault.jpg)](https://youtu.be/etB9GL0f5tQ)


v0.01 bare minimum
[![Watch the prototype video](https://img.youtube.com/vi/MZJTI-8vdiI/maxresdefault.jpg)](https://youtu.be/MZJTI-8vdiI)



