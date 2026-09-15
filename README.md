# Smart Exam Hall Monitoring and Management System

An embedded system built on the NXP LPC2148 (ARM7TDMI-S) microcontroller that automates examination timing, environmental monitoring and status indication in an exam hall — replacing manual stopwatches and wall clocks with a secure, self-contained countdown and alert system.

## Overview

Before an exam starts, the system idles on a 16x2 LCD showing live RTC time, date and room temperature. An invigilator authenticates with a password (triggered by an external interrupt) to set the exam duration or edit the RTC. Once the exam starts, a countdown runs on two multiplexed 7-segment displays, a three-stage Green → Yellow → Red LED system escalates urgency as time runs low, and a buzzer signals the end of the exam. A second interrupt-driven switch lets the invigilator pause and resume the countdown at any time without losing the remaining duration.

## Features

- Live RTC-based time, date and day-of-week display
- Real-time room temperature monitoring via an LM35 sensor
- Password-protected admin access (RTC edit, exam duration, password change) via External Interrupt 0
- Pause/resume countdown control via External Interrupt 1
- Exam duration entry (00–99) through a 4x4 matrix keypad
- Countdown displayed on two multiplexed 7-segment digits
- Three-stage LED status indication (Green / Yellow / Red) based on time remaining
- Buzzer alert on critical time and exam completion
- Modular, driver-per-peripheral embedded C codebase

## Hardware

| Component | Purpose |
|---|---|
| LPC2148 (ARM7TDMI-S) | Main controller |
| 16x2 character LCD | Time, date, temperature and menu display |
| 4x4 matrix keypad | Password entry, RTC edit, duration entry |
| 2 push switches | SW1 → EINT0 (admin access), SW2 → EINT1 (pause/resume) |
| 2x 7-segment displays (multiplexed) | Remaining exam time in minutes |
| LM35 temperature sensor | Room temperature via on-chip ADC |
| 3 LEDs (Green / Yellow / Red) | Time-remaining status |
| Buzzer | End-of-exam / critical-time alert |
| MAX232 + DB-9 / USB-UART | Programming via Flash Magic |

## Block diagram

```
   KEYPAD  ─────────────►┌─────────────┐───────────► LCD
                          │             │
 SWITCH 1 ──(EINT0)──────►│             │───────────► 7-SEGMENT
                          │   LPC2148   │
 SWITCH 2 ──(EINT1)──────►│   [ RTC ]   │───────────► GREEN LED
                          │             │───────────► YELLOW LED
    LM35  ────(ADC)──────►│             │───────────► RED LED
                          └─────────────┘───────────► BUZZER
```

## Pin mapping

| Pin(s) | Function |
|---|---|
| P0.8–P0.15 | LCD data bus D0–D7 |
| P0.16 / P0.17 / P0.18 | LCD RS / EN / RW |
| P0.1 | EINT0 — admin switch |
| P0.7 | EINT1 — pause/resume switch |
| P0.2 / P0.3 / P0.4 | Green / Yellow / Red LED |
| P0.23 | Buzzer |
| P0.20 / P0.21 | 7-segment digit select (tens / ones) |
| P0.28 | LM35 input (AD0.1) |
| P1.16–P1.19 | Keypad rows (output) |
| P1.20–P1.23 | Keypad columns (input) |
| P1.24–P1.31 | 7-segment segment data bus |

## Software architecture

Each peripheral has its own driver module, all initialized and coordinated from `main.c`:

```
├── main_pro.c        # main() — init + idle display loop
├── interrupt_p.c/h   # EINT0/EINT1 ISRs, password check, settings menu
├── rtc_mpt.c/h        # RTC init, set/get, display
├── rtc_edit.c         # keypad-driven RTC edit state machine
├── kpm_mp.c/h         # 4x4 keypad scan + input reader
├── lcd_t.c/h          # HD44780 LCD driver
├── adc_mpt.c/h        # ADC driver
├── lm35_mpt.c/h       # LM35 temperature conversion
├── led_mpt.c/h        # LED status logic
├── buzzer_mpt.c/h     # Buzzer control
├── 7seg_mpt.c/h       # Multiplexed 7-segment driver
├── timer.c/h          # Timer0 ISR — countdown engine
├── delay.c/h          # Busy-wait delays
└── types_t.h, defines.h  # typedefs & bit macros
```

## How it works

1. Idle mode: LCD continuously shows RTC time/date and LM35 temperature.
2. Admin access: SW1 triggers `EINT0`; a 4-digit password (masked with `*`) unlocks a settings menu to edit the RTC, set exam duration, or change the password.
3. Exam countdown: once a duration is set, `Timer0` drives a 1 ms tick that counts down the exam, updates the multiplexed 7-segment display, and escalates LED/buzzer alerts as time runs low (Green > 50%, Yellow 30–50%, Red 10–30%, blinking Red + buzzer < 10%).
4. Pause/resume: SW2 triggers `EINT1` to stop and restart the countdown timer without losing the remaining duration.
5. Completion: when time reaches zero, the timer, LEDs and buzzer state are handled automatically inside the interrupt.

## Tools & environment

- Language: Embedded C
- MCU: NXP LPC2148 (ARM7TDMI-S)
- Programmer: Flash Magic via UART0 (MAX232 / USB-UART)
- Board: Advanced Development Board for ARM7

## Future scope

- Log exam start/end times and pause events over UART or to an SD card
- Network multiple boards for multi-hall monitoring from a central station
- Add RFID/fingerprint-based student attendance
- Add sensor-triggered malpractice alerts reusing the existing LED/buzzer outputs
- Battery-backed RTC for power-loss resilience
- Multi-level (admin/invigilator) password access
