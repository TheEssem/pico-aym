# pico-aym
Experimenting with controlling an AY-3-8910/YM2149F chip using a Raspberry Pi Pico.

Currently, this has a custom player for the VGM file format that runs directly on the Pico. Proper timings are still being worked out, but it should be usable for playing back most Furnace or PT3/Vortex Tracker converted tunes. Very complex tunes (e.g. Atari ST tunes using samples, SID PWM, sync-buzzer) may have issues.

[See this thread for progress/demo recordings.](https://wetdry.world/@esm/111265310104803127)

TODO: Add support for sending commands over USB.