# MachIntell J-Link Test Application

**Company:** MachIntell Solutions Pvt. Ltd.  
**Purpose:** Validate that J-Link (Java) integration works in your Creo 11 environment.

---

## What This Test Verifies

| Check | What it proves |
|-------|---------------|
| Java app loads into Creo | J-Link setup is correct |
| Custom button appears | Command/UI registration works |
| Popup dialog shows | UI integration works |
| Model name is read | Data access API works |
| Log file is written | External I/O works |

---

## Prerequisites

- **Creo 11** with J-Link / Creo Object TOOLKIT Java installed
- **Java JDK 11+** (or use Creo's bundled JRE)
- No special license unlock needed for J-Link!

---

## Quick Start

### Step 1: Find pfc.jar on Your System

Open Command Prompt and search:
```
dir /s /b "C:\Program Files\PTC\*pfc.jar"
```

Typical locations:
- `C:\Program Files\PTC\Creo 11.0.0.0\Common Files\text\java\pfc.jar`
- `C:\Program Files\PTC\Creo 11.0.0.0\Common Files\otk\otk_java\pfc.jar`

**Write down this path** — you'll need it.

### Step 2: Verify J-Link is Installed

Check if this folder exists:
```
C:\Program Files\PTC\Creo 11.0.0.0\Common Files\otk\otk_java\
```
OR
```
C:\Program Files\PTC\Creo 11.0.0.0\Common Files\text\java\
```

If neither exists, J-Link may not be installed. Re-run Creo installer and select "Creo Object TOOLKIT Java" / "J-Link" component.

### Step 3: Build

1. Open Command Prompt
2. Navigate to this folder:
   ```
   cd C:\MachIntell\creo-jlink-test
   ```
3. Edit `build.bat` — update `CREO_COMMON` path (line 18)
4. Run:
   ```
   build.bat
   ```
5. Verify `classes\com\machintell\creotest\MachIntellTest.class` exists

### Step 4: Update .dat File Paths

Open `machintell_jlink_test.dat` and set absolute paths:
```
JAVA_APP_CLASSPATH  C:\MachIntell\creo-jlink-test\classes
TEXT_DIR            C:\MachIntell\creo-jlink-test\text\
```

**IMPORTANT:** The JAVA_APP_CLASSPATH must also include pfc.jar! 
Update it to:
```
JAVA_APP_CLASSPATH  C:\MachIntell\creo-jlink-test\classes;C:\Program Files\PTC\Creo 11.0.0.0\Common Files\text\java\pfc.jar
```
(Use the pfc.jar path you found in Step 1)

### Step 5: Load into Creo

1. Start Creo 11
2. Go to **Utilities** → **Auxiliary Applications**
3. Click **Register** → browse to `machintell_jlink_test.dat`
4. Click **Start**
5. Status should show "Running"

### Step 6: Test

1. Look in **Utilities** menu for **"MachIntell J-Link Test"**
2. Click it
3. A popup dialog should appear: **"J-Link Connection SUCCESSFUL!"**
4. Check for `machintell_jlink_log.txt` in Creo's working directory

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| "Cannot find pfc.jar" during build | Find pfc.jar manually: `dir /s /b "C:\*pfc.jar"` |
| "javac not recognized" | Install JDK or use full path to javac |
| App fails to start in Creo | Check JAVA_APP_CLASSPATH includes both your classes AND pfc.jar |
| "Class not found" error | Verify classes folder has `com\machintell\creotest\MachIntellTest.class` |
| Button not visible | Search "MachIntell" in ribbon search box |
| J-Link not installed | Re-run Creo installer, select "API Toolkits" > "Creo Object TOOLKIT Java" |

---

## Key Advantage Over Pro/TOOLKIT

**J-Link does NOT require the TOOLKIT license to run!** 
It uses the JLINK license which is often included with Creo by default, or Java apps may run without any special license depending on your Creo configuration.

---

## File Structure

```
creo-jlink-test/
├── src/
│   └── com/machintell/creotest/
│       └── MachIntellTest.java         # Source code
├── classes/                            # Compiled output (after build)
│   └── com/machintell/creotest/
│       └── MachIntellTest.class
├── text/
│   └── usascii/
│       └── msg_machintell_jlink.txt    # UI message strings
├── machintell_jlink_test.dat           # Registration file
├── machintell_jlink_test_absolute.dat  # Registration (absolute paths)
├── build.bat                           # Build script
└── README.md                           # This file
```

---

## Contact

**MachIntell Solutions Pvt. Ltd.**  
For any issues, contact us — we can assist via remote session.
