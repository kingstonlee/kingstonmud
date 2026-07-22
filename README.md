# kingstonmud

A text-based, multiplayer online game — a **MUD** (Multi-User Dungeon) — that
players connect to over telnet. It is a customized fork of
[tbaMUD](http://tbamud.com), which descends from
[CircleMUD](http://www.circlemud.org) and ultimately DikuMUD. The server is
written in C.

The engine is stock tbaMUD; what makes this world **kingstonmud** is its custom,
pop-culture-themed content, including:

- **Galaxy's Edge / the Millennium Falcon** (Star Wars)
- **Neverland** (Peter Pan)
- Fairy-tale zones (Sleeping Beauty, Prince Charming, and more)

The world contains roughly 200 zones, 13,000 rooms, 3,700 mobiles, and 4,700
objects.

## Quick start

```sh
# 1. Generate the build configuration (autoconf)
./configure

# 2. Compile the server
cd src && make

# 3. Run the game from the project root
cd ..
./autorun          # keeps the MUD running and reboots it if it crashes
# ...or run the binary directly for a single session:
# bin/circle
```

The server listens on **port 4000** by default. Connect with any MUD or telnet
client:

```sh
telnet localhost 4000
```

Override the port by passing it to the binary (`bin/circle <port>`) or by
editing `lib/etc/config` (see `src/config.c` for the defaults).

### Validating the world without playing

To load and syntax-check every world file without starting the game, run:

```sh
bin/circle -c
```

An exit code of `0` means the world data is structurally sound.

## Repository layout

| Path            | Contents                                                                 |
| --------------- | ------------------------------------------------------------------------ |
| `src/`          | The C source for the MUD server (~150 files)                             |
| `lib/`          | Runtime game data (world, players, text)                                 |
| `lib/world/`    | World definitions: `wld` rooms, `mob` mobiles, `obj` objects, `zon` zones, `shp` shops, `qst` quests, `trg` DG-Script triggers |
| `lib/text/`     | In-game text: MOTD, greetings, news, help, credits                       |
| `lib/plrfiles/`, `plrobjs/`, `plrvars/` | Player accounts, inventories, and saved variables      |
| `doc/`          | Documentation — building, coding, admin, OLC manuals, per-platform notes  |
| `bin/`          | Compiled binaries (created by the build; gitignored)                      |
| `autorun*`      | Launcher scripts for various platforms (`.sh`, `.cmd`, `.pl`, Amiga, VMS) |
| `configure`     | Autoconf build configuration                                              |
| `changelog`     | tbaMUD / CircleMUD release history                                        |

## Building the world

Most content is authored **in-game** using tbaMUD's Online Creation (OLC)
editors — `redit` (rooms), `medit` (mobiles), `oedit` (objects), `zedit`
(zones), `sedit` (shops), `qedit` (quests), and `trigedit` (DG Scripts). Changes
made in OLC are saved back into the files under `lib/world/`. See
`doc/building.txt` and `doc/coding.txt` for details.

## License

kingstonmud is built on CircleMUD/tbaMUD and is governed by the **CircleMUD
license** — see [`LICENSE.md`](LICENSE.md). If you use this code you are legally
bound to comply with that license, which includes crediting the CircleMUD
authors and not using it for profit.

## Credits

- **CircleMUD** — Jeremy Elson and contributors
- **tbaMUD** — the tbaMUD development team (http://tbamud.com)
- **kingstonmud** — custom world content by the kingstonmud team
