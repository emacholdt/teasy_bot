# teasy_bot
Teasy Bot is a project to build an Arduino based device that preps tea, based on time (and later temp) specific for each kind of tea.


## current state  video

v0.02 with display and interface for infusion time and number of initial dips
[![Watch the prototype video](https://img.youtube.com/vi/etB9GL0f5tQ/maxresdefault.jpg)](https://youtu.be/etB9GL0f5tQ)

## parts and materials

- Genuino / Elegoo Nano v3
- 3V Servo Motor
- 4 random red push buttons
- 1 button for reset trigger
- 2 LCD1602 Module (16x2)
- 1 Potentiometer 
- some universal PCB
- black crafting plastic board
- pipet tip box as inclosure
- wires


## code

teasy_bot.ino

### includes
- the start screen
- switch case for the current mode / state
- menu for adjusting and setting
    + time
    + number of initial dips
- prints remaining time
- methods for each step and each menu print
- finishing message

### missing
- implementing of millis()
- saving of last settings
- option for occational moving of the tea bag

potentially
- settings menu option
- adjustment for cup height



## video archive

v0.01 bare minimum
[![Watch the prototype video](https://img.youtube.com/vi/MZJTI-8vdiI/maxresdefault.jpg)](https://youtu.be/MZJTI-8vdiI)



