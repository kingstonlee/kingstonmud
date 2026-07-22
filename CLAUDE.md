# CLAUDE.md

Guidance for working in this repository.

## What this is

kingstonmud is a customized fork of **tbaMUD** (a CircleMUD/DikuMUD descendant),
a C-based text MUD server. The engine lives in `src/`; the game world and player
data live in `lib/`. Most day-to-day changes are **world content** (new zones,
mobs, objects), not engine code.

## Build, run, validate

```sh
./configure              # once, to generate src/conf.h and Makefiles (autoconf)
cd src && make           # compile; produces ../bin/circle
make clean               # remove object files and binaries

# from the project root:
bin/circle -c            # load & syntax-check the whole world, then exit (exit 0 = OK)
./autorun                # run the MUD (auto-reboots on crash); default port 4000
bin/circle 4000          # run a single session on a specific port
```

The build should be **warning-free with 0 errors**. If you change engine code,
recompile and keep it that way.

## Code layout (`src/`)

- `comm.c` — main game loop, socket/network I/O, startup (`-c` syntax-check mode)
- `interpreter.c` — command parsing and the command table
- `db.c` — boots the world from `lib/world/`
- `handler.c`, `utils.c` — core object/character manipulation helpers
- `fight.c`, `magic.c`, `spells.c`, `spell_parser.c` — combat and magic
- `act.*.c` — player commands, grouped (`informative`, `movement`, `offensive`,
  `other`, `social`, `wizard`, `comm`, `item`)
- `dg_*.c` — **DG Scripts**, the in-game trigger/scripting engine
- `oasis.c` + `*edit.c` (`redit`, `medit`, `oedit`, `zedit`, `sedit`, `qedit`,
  `hedit`, `aedit`, `cedit`, `tedit`) — **OLC** (Online Creation) editors
- `gen*.c` (`genmob`, `genobj`, `genwld`, `genzon`, `genshp`, `genqst`, `genolc`)
  — routines that read/write world files for OLC
- `spec_procs.c`, `spec_assign.c`, `castle.c` — special procedures (custom NPC/room behavior)
- `house.c`, `mail.c`, `boards.c`, `shop.c`, `quest.c` — subsystems
- `structs.h`, `utils.h`, `config.c`/`config.h` — core types, macros, tunables
- `util/` — standalone maintenance tools (autowiz, shopconv, plrtoascii, etc.)

## World data (`lib/world/`)

One subdirectory per record type, each with an `index` file listing its members:

| Dir   | Ext    | Contents           |
| ----- | ------ | ------------------ |
| `wld` | `.wld` | rooms              |
| `mob` | `.mob` | mobiles (NPCs)     |
| `obj` | `.obj` | objects/items      |
| `zon` | `.zon` | zones + reset cmds |
| `shp` | `.shp` | shops              |
| `qst` | `.qst` | quests             |
| `trg` | `.trg` | DG-Script triggers |

Records start with `#<vnum>`. Zone reset commands (`M`/`O`/`G`/`E`/`P`/`D`/`R`/`T`)
reference mob/obj/room/trigger vnums; a reset that points at a vnum with no
matching record is the classic builder bug. After editing world files, always
run `bin/circle -c` to confirm the world still loads.

Prefer editing content **in-game via OLC** (`redit`, `medit`, `oedit`, `zedit`,
…) rather than hand-editing files — OLC writes correct, canonical formatting.

## Conventions

- C style is legacy K&R/CircleMUD: existing files mix tabs and spaces — **match
  the surrounding file** rather than reformatting.
- Use `snprintf` over `sprintf` for anything touching player/world strings.
- Don't commit build artifacts — `bin/`, `src/*.o`, generated `src/conf.h`,
  `src/Makefile`, `log/`, and `lib/plr*/` are gitignored.
- Player files under `lib/plrfiles/`, `lib/plrobjs/`, `lib/plrvars/` are runtime
  state, not source — leave them out of commits.

## Docs

See `doc/`: `building.txt`, `coding.txt`, `admin.txt`, `dg_events.txt`, plus
per-platform `README.*` files and OLC manuals.
