# MachIntell Creo Toolkit Test Application

**Company:** MachIntell   
**Purpose:** Validate that Creo Toolkit (Pro/TOOLKIT) integration works in your Creo 11 environment.

---

## What This Test Verifies

| Check | What it proves |
|-------|---------------|
| DLL loads into Creo | Toolkit setup is correct, paths work |
| Custom button appears | Command registration API works |
| Popup dialog shows | UI integration works |
| Model name is read | Data access API works |
| Log file is written | External I/O from Toolkit works |

---

## Quick Start (5 minutes)

### Step 1: Build the DLL

1. Open **"x64 Native Tools Command Prompt for VS 2022"**  
   (Start Menu > Visual Studio 2022 > x64 Native Tools Command Prompt for VS 2022)

2. Navigate to this folder:
   ```
   cd C:\path\to\creo-toolkit-test
   ```

3. Edit `build.bat` — change the `CREO_INSTALL` path on line 19 to your Creo location:
   ```
   SET CREO_INSTALL=C:\Program Files\PTC\Creo 11.0.0.0\Common Files
   ```

4. Run:
   ```
   build.bat
   ```

5. If successful, you'll see: `bin\machintell_test.dll`

### Step 2: Load into Creo

1. **Start Creo 11**
2. Go to **Utilities** tab in the ribbon (or **Tools** menu)
3. Click **Auxiliary Applications**
4. Click **Register**
5. Browse to and select `machintell_test.dat` from this folder
6. Click **Start**
7. Status should show "Running"

### Step 3: Test

1. Look in the **Utilities** menu for **"MachIntell Test"** button
   - If not visible there, type "MachIntell" in the ribbon search box (top-right)
2. Click the button
3. A popup dialog should appear saying: **"Creo Toolkit Connection SUCCESSFUL!"**
4. Check that `machintell_test_log.txt` was created in Creo's working directory

### Step 4: Share Results

Please send us:
- The `machintell_test_log.txt` file
- A screenshot of the popup dialog (optional but helpful)
- Any error messages if something didn't work

---

## Troubleshooting

### "Cannot find ProToolkit.h" during build
- Pro/TOOLKIT may not be installed. Check: does `C:\Program Files\PTC\Creo 11.0.0.0\Common Files\protoolkit\includes\` exist?
- If not, re-run Creo installer and include "Pro/TOOLKIT" or "Creo Toolkit" component

### "cl is not recognized"
- You're not in the right command prompt
- Must use "x64 Native Tools Command Prompt for VS 2022" (NOT regular cmd/PowerShell)

### "Cannot find protoolkit.lib" during link
- Check folder: `<creo>\Common Files\protoolkit\x86e_win64\obj\`
- If the folder name is different (e.g., `x86e_win64` vs `i486_nt`), update `build.bat` line 60

### DLL loads but button doesn't appear
- In Auxiliary Applications, verify status shows "Running" (not "Failed")
- Try searching "MachIntell" in the ribbon search box
- Check Creo's trail file for error messages

### "Application failed to start" in Auxiliary Applications
- Path in `.dat` file might be wrong
- Edit `machintell_test.dat` and use FULL absolute paths:
  ```
  EXEC_FILE  C:\full\path\to\bin\machintell_test.dll
  TEXT_DIR   C:\full\path\to\text\
  ```
  (see `machintell_test_absolute.dat.example` for a template)

### Log file not created
- Creo writes it to its working directory (usually `C:\Users\<name>\Documents` or whatever folder Creo was started from)
- Search your system for `machintell_test_log.txt`

---

## File Structure

```
creo-toolkit-test/
├── src/
│   └── test_app.c                        # Source code
├── bin/
│   └── machintell_test.dll               # Built output (after build.bat)
├── text/
│   └── usascii/
│       └── msg_machintell.txt            # Message strings for Creo UI
├── machintell_test.dat                   # Registration file (relative paths)
├── machintell_test_absolute.dat.example  # Registration template (absolute paths)
├── build.bat                             # Build script
└── README.md                             # This file
```

---

## Technical Details

- **Language:** C (Pro/TOOLKIT native API)
- **Target:** 64-bit DLL (x86_64 Windows)
- **Creo Version:** 11.x
- **API Used:** ProCmdActionAdd, ProMdlCurrentGet, ProMdlNameGet, ProUIMessageDialogDisplay
- **Build Tool:** MSVC (Visual Studio 2019/2022)

---

## Contact

**MachIntell**  
For assistance with building or loading, contact us — we can help via remote session.


