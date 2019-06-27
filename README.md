# 101: Streamlined 1v1
## Code
See `code.txt` for the code in text format or use the following import codes.

- v1.0.0: `3V02C`
- v1.1.0: `BFSDJ`
- v1.2.0: `5XWAR`



## Features
101 by Corrade. Classic 1v1 streamlined for practicing raw deathmatch. Based on Lange's TF2 mod, MGE.

-   Smart respawn

    Instantly respawn nearby your opponent after you die.
-   Respite

    Receive a brief period of invulnerability after each exchange.
-   Refresh

    Automatically reload, reset cooldowns, heal and cleanse statuses after getting a kill.
-   Health check

    See how much health your opponent had remaining after they kill you.
-   Seamless rounds

    After a match finishes, scores reset and the next round begins immediately.
-   Round history

    Leave the record-keeping to the gamemode.
-   General settings

    First to 20. No health packs. No unsuitable heroes or abilities. No ults. Only maps you'll see in QP/competitive. Change these settings as you see fit.



## Commands
-   Ult

    Suicide without instantly respawning, allowing you to change heroes. You'll only lose a point if you're not at full health.
-   Crouch + Interact

    Display the round history.



## Known bugs
> *This bug may have been resolved in patch 1.37. Please notify me at Corrade#0901 on Discord if you encounter it playing on OW v1.37 or later.*

- It's possible to respawn in a spawn room that's usually inaccessible and consequently become trapped. There's currently no easy way to solve this, so for the time being just press ult to change heroes and respawn. Avoid damaging yourself so you don't lose a point in the process.



## Changelog
### v1.1.0
- Improved the randomness of spawns.
- Significantly reduced the time taken to respawn.
- Significantly reduced the delay for refreshing.
- Made refreshing restore a more current position.
- Fixed bugs with the teleport module used by the smart respawn and refresh features.

### v1.2.0
- Refactored many rules and tidied up everything.

### v1.3.0
- Sharpened UI and messages.
- Fixed a player's score being able to change when they didn't have an opponent.
- Replaced pG using 'entity exists' and replaced pC using 'count of'.
- Disabled move speed in teleport module.