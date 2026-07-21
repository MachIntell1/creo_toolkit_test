/**
 * MachIntell Creo J-Link Test Application
 * MachIntell Technologies Pvt. Ltd.
 *
 * Purpose: Verify J-Link integration is working correctly in Creo 11.
 * 
 * This application:
 *   - Registers a custom menu button in Creo
 *   - On click, displays a success message
 *   - Reads the active model name (if any model is open)
 *   - Writes a log file (machintell_jlink_log.txt) for verification
 */

package com.machintell.creotest;

import com.ptc.cipjava.jlink.*;
import com.ptc.pfc.pfcGlobal.*;
import com.ptc.pfc.pfcSession.*;
import com.ptc.pfc.pfcModel.*;
import com.ptc.pfc.pfcModelItem.*;
import com.ptc.pfc.pfcCommand.*;
import com.ptc.pfc.pfcUI.*;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MachIntellTest {

    private static final String LOG_FILE = "machintell_jlink_log.txt";
    private static Session session = null;

    /**
     * Write a timestamped message to the log file
     */
    private static void writeLog(String message) {
        try {
            FileWriter fw = new FileWriter(LOG_FILE, true);
            PrintWriter pw = new PrintWriter(fw);
            String timestamp = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
            pw.println("[" + timestamp + "] " + message);
            pw.close();
        } catch (IOException e) {
            // Silent fail for logging
        }
    }

    /**
     * Start method - called by Creo when the J-Link app is loaded
     */
    public static void start() {
        try {
            writeLog("========================================");
            writeLog("MachIntell J-Link Test App - Loading...");

            session = pfcGlobal.GetProESession();

            if (session != null) {
                writeLog("Creo session obtained successfully!");
                writeLog("J-Link connection VERIFIED.");
            } else {
                writeLog("ERROR: Could not get Creo session");
                return;
            }

            // Register a UI command
            UICommand cmd = session.UICreateCommand("MachIntellJLinkTest", new TestActionListener());
            cmd.Designate("msg_machintell_jlink.txt",
                          "MACHINTELL_TEST_CMD",
                          "MACHINTELL_TEST_HELP",
                          "MACHINTELL_TEST_HELP");

            // Add to ribbon/menu
            session.UIAddButton(cmd,
                               "Utilities",
                               null,
                               null,
                               "MACHINTELL_TEST_LABEL",
                               "msg_machintell_jlink.txt");

            writeLog("Menu button registered: Utilities > MachIntell J-Link Test");
            writeLog("Initialization complete.");
            writeLog("========================================");

            // Show confirmation in Creo message area
            session.UIDisplayMessage("msg_machintell_jlink.txt",
                                    "STARTUP_MSG", null);

        } catch (Exception e) {
            writeLog("ERROR during initialization: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Stop method - called by Creo when the J-Link app is unloaded
     */
    public static void stop() {
        writeLog("MachIntell J-Link Test App - Unloaded cleanly.");
        session = null;
    }

    /**
     * Action listener for the test button click
     */
    private static class TestActionListener extends DefaultUICommandActionListener {

        public void OnCommand() {
            try {
                writeLog("=== MachIntell J-Link Test ===");
                writeLog("Button clicked - J-Link integration VERIFIED");

                String resultMessage;

                // Try to get current model
                Model currentModel = session.GetCurrentModel();

                if (currentModel != null) {
                    String modelName = currentModel.GetFileName();
                    String modelType = getModelTypeString(currentModel);

                    writeLog("Active model: " + modelName + " (Type: " + modelType + ")");
                    resultMessage = "J-Link Connection SUCCESSFUL!\n\n" +
                                   "Active Model: " + modelName + "\n" +
                                   "Type: " + modelType + "\n\n" +
                                   "Log file: machintell_jlink_log.txt\n\n" +
                                   "[MachIntell Technologies]";
                } else {
                    writeLog("No active model open (OK for basic verification)");
                    resultMessage = "J-Link Connection SUCCESSFUL!\n\n" +
                                   "No model currently open.\n\n" +
                                   "Log file: machintell_jlink_log.txt\n\n" +
                                   "[MachIntell Technologies]";
                }

                // Show dialog
                session.UIShowMessageDialog(resultMessage, null);

                writeLog("Test completed successfully.");
                writeLog("---");

            } catch (Exception e) {
                writeLog("ERROR during test: " + e.getMessage());
                try {
                    session.UIShowMessageDialog(
                        "Error: " + e.getMessage() + "\nCheck log file for details.", null);
                } catch (Exception ex) {
                    // ignore
                }
            }
        }

        private String getModelTypeString(Model model) {
            try {
                ModelType type = model.GetType();
                if (type == ModelType.MDL_PART) return "Part";
                if (type == ModelType.MDL_ASSEMBLY) return "Assembly";
                if (type == ModelType.MDL_DRAWING) return "Drawing";
                return "Other";
            } catch (Exception e) {
                return "Unknown";
            }
        }
    }
}
