/*
 * Creo Toolkit Test Application
 * MachIntell solutions Pvt. Ltd.
 *
 * Purpose: Verify Creo Toolkit integration is working correctly.
 * This application adds a custom menu button to Creo and on click:
 *   - Shows a success message in Creo's message area
 *   - Reads the active model name (if any model is open)
 *   - Writes a log file (machintell_test_log.txt) for verification
 *
 * Tested for: Creo 11 (Pro/TOOLKIT)
 */

#include <ProToolkit.h>
#include <ProMenuBar.h>
#include <ProMessage.h>
#include <ProMdl.h>
#include <ProUtil.h>
#include <ProArray.h>
#include <ProUIMessage.h>

#include <stdio.h>
#include <time.h>
#include <string.h>

static char log_path[260] = "machintell_test_log.txt";

/*--------------------------------------------------------------------*/
static void write_log(const char *message)
{
    FILE *fp;
    time_t now;
    char time_buf[64];

    fp = fopen(log_path, "a");
    if (fp == NULL)
        return;

    time(&now);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(fp, "[%s] %s\n", time_buf, message);
    fclose(fp);
}

/*--------------------------------------------------------------------*/
static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

/*--------------------------------------------------------------------*/
static int action_test_connection(uiCmdCmdId command,
                                   uiCmdValue *p_value,
                                   void *p_push_cmd_data)
{
    ProError status;
    ProMdl current_model = NULL;
    ProMdlType model_type;
    ProName w_model_name;
    ProMdlFileName w_file_name;
    char name_str[PRO_NAME_SIZE] = {0};
    char log_msg[512];
    wchar_t w_msg[256];

    write_log("=== MachIntell Creo Toolkit Test ===");
    write_log("Button clicked - Toolkit integration VERIFIED");

    ProTKPrintf("----------------------------------------------\n");
    ProTKPrintf("  MachIntell Creo Toolkit Test - SUCCESSFUL\n");
    ProTKPrintf("----------------------------------------------\n");

    status = ProMdlCurrentGet(&current_model);

    if (status == PRO_TK_NO_ERROR && current_model != NULL)
    {
        ProMdlNameGet(current_model, w_model_name);
        ProMdlTypeGet(current_model, &model_type);
        ProWstringToString(name_str, w_model_name);

        snprintf(log_msg, sizeof(log_msg),
                 "Active model: %s (type: %s)",
                 name_str,
                 (model_type == PRO_MDL_PART) ? "Part" :
                 (model_type == PRO_MDL_ASSEMBLY) ? "Assembly" :
                 (model_type == PRO_MDL_DRAWING) ? "Drawing" : "Other");
        write_log(log_msg);

        ProTKPrintf("  Active model: %s\n", name_str);

        ProMessageDisplay(L"msg_machintell.txt", "TEST_WITH_MODEL", name_str);
    }
    else
    {
        write_log("No active model open (this is OK for basic verification)");
        ProTKPrintf("  No model currently open.\n");

        ProMessageDisplay(L"msg_machintell.txt", "TEST_NO_MODEL");
    }

    ProTKPrintf("  Log file: %s\n", log_path);
    ProTKPrintf("----------------------------------------------\n");

    /* Show a popup info dialog for clear visual confirmation */
    {
        ProUIMessageButton *buttons = NULL;
        ProUIMessageButton user_choice;

        ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray*)&buttons);
        buttons[0] = PRO_UI_MESSAGE_OK;

        if (name_str[0] != '\0')
        {
            ProTKSprintf(w_msg, L"Creo Toolkit Connection SUCCESSFUL!\n\n"
                                 L"Active Model: %s\n\n"
                                 L"Log written to: machintell_test_log.txt\n\n"
                                 L"[MachIntell ]", name_str);
        }
        else
        {
            ProTKSprintf(w_msg, L"Creo Toolkit Connection SUCCESSFUL!\n\n"
                                 L"No model currently open.\n\n"
                                 L"Log written to: machintell_test_log.txt\n\n"
                                 L"[MachIntell ]");
        }

        ProUIMessageDialogDisplay(
            PROUIMESSAGE_INFO,
            L"MachIntell - Toolkit Test",
            w_msg,
            buttons, PRO_UI_MESSAGE_OK,
            &user_choice);

        ProArrayFree((ProArray*)&buttons);
    }

    write_log("Test completed successfully.");
    write_log("---");

    return 0;
}

/*--------------------------------------------------------------------
 * user_initialize - Entry point called by Creo when app loads
 *--------------------------------------------------------------------*/
int user_initialize(
    int argc,
    char *argv[],
    char *version,
    char *build,
    wchar_t errbuf[80])
{
    ProError status;
    uiCmdCmdId cmd_id;
    char log_msg[256];

    write_log("========================================");
    write_log("MachIntell Test App - Loading...");
    snprintf(log_msg, sizeof(log_msg), "Creo version: %s build: %s", version, build);
    write_log(log_msg);
    write_log("Creo Toolkit connection established!");

    status = ProCmdActionAdd(
        "MachIntell_Test",
        (uiCmdCmdActFn)action_test_connection,
        uiProeImmediate,
        AccessDefault,
        PRO_B_TRUE,
        PRO_B_TRUE,
        &cmd_id);

    if (status != PRO_TK_NO_ERROR)
    {
        write_log("ERROR: ProCmdActionAdd failed");
        swprintf(errbuf, 80, L"MachIntell: Failed to register command");
        return -1;
    }

    status = ProCmdDesignate(
        cmd_id,
        "MachIntell Test",
        "Click to test Creo Toolkit connection",
        "MachIntell Test - Verify Toolkit Integration",
        NULL);

    if (status != PRO_TK_NO_ERROR)
    {
        write_log("NOTE: ProCmdDesignate returned non-zero (cosmetic only)");
    }

    status = ProMenubarmenuPushbuttonAdd(
        "Utilities",
        "MachIntell_Test_Btn",
        "MachIntell_Test",
        "MachIntell_Test_Help",
        NULL,
        PRO_B_TRUE,
        cmd_id,
        L"msg_machintell.txt");

    if (status == PRO_TK_NO_ERROR)
    {
        write_log("Menu button added to: Utilities > MachIntell Test");
    }
    else
    {
        write_log("NOTE: Utilities menu item not added (ribbon-only mode).");
        write_log("      Use ribbon search to find 'MachIntell Test'");
    }

    write_log("Initialization complete.");
    write_log("========================================");

    ProTKPrintf("\n** MachIntell Test App loaded successfully **\n");
    ProTKPrintf("** Find 'MachIntell Test' in Utilities menu or ribbon search **\n\n");

    return 0;
}

/*--------------------------------------------------------------------
 * user_terminate - Called by Creo when the application is unloaded
 *--------------------------------------------------------------------*/
void user_terminate()
{
    write_log("MachIntell Test App - Unloaded cleanly.");
    ProTKPrintf("** MachIntell Test App unloaded **\n");
}
