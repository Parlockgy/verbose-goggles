# Size Mayhem (Geode / Android arm64)

A small Geometry Dash Geode mod prototype. It exposes three in-game mod settings:

- **Enable Size Mayhem** — off by default.
- **Player size** — 0.25× to 4×.
- **Spike size** — 0.25× to 4×.

The current spike filter uses known spike object IDs (8, 39, 40, 103, 392, 1333 and 1334). Other spike variants may need to be added after testing in the target Geometry Dash build.

## Build a real `.geode` package

This project includes a GitHub Actions workflow that builds an Android 64-bit package using the official Geode build action. The source archive itself is **not** a compiled `.geode` mod.

1. Create a new GitHub repository.
2. Upload the contents of this folder (including the hidden `.github` directory) to the repository's `main` branch.
3. Open **Actions** and run **Build Size Mayhem for Android** (or let it run after pushing to `main`).
4. Open the completed workflow run and download the `Size-Mayhem-Android64` artifact. It contains the compiled `.geode` file if the build succeeds.
5. Copy the `.geode` file to the Geode mods folder on Android:
   `/storage/emulated/0/Android/media/com.geode.launcher/game/geode/mods/`

## Compatibility notes

- Target SDK: Geode v5.11.2; target Geometry Dash Android 2.2081.
- Intended for a 64-bit Android installation of Geometry Dash + Geode.
- Size changes are experimental. Scaling visual nodes may not perfectly match every collision behavior, and changing player scale can interact with size portals or other mods.
- This environment does not have the Android NDK, Geode CLI, or Geode Android SDK binaries installed, and cannot reach GitHub to fetch them. Therefore the source was prepared, but no compiled `.geode` file is claimed here.
