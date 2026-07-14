/*
 * Creo Toolkit Test Application
 * MachIntell Technologies Pvt. Ltd.
 *
 * Purpose: Verify Creo Toolkit integration is working correctly.
 * Tested for: Creo 11 (Pro/TOOLKIT)
 */

#include <ProToolkit.h>
#include <ProMenuBar.h>
#include <ProMessage.h>
#include <ProMdl.h>
#include <ProUtil.h>
#include <ProWindows.h>
#include <ProUICmd.h>
#include <ProTKRunTime.h>

#include <stdio.h>
#include <time.h>
#include <string.h>

static wchar_t MSGFIL[] = {'m','s','g','_','m','a','c','h','i','n','t','e','l','l','.','t','x','t','\0'};

/*--------------------------------------------------------------------*/
static void write_log(const char *message)
{
    FILE *fp;
    time_t now;
    char time_buf[64];

    fp = fopen("machintell_test_log.txt", "a");
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
    return (ACCESS_AVAILABLE);
}

/*--------------------------------------------------------------------*/
static int action_test_connection(uiCmdCmdId command,
                                   uiCmdValue *p_value,
                                   void *p_push_cmd_data)
{
    ProError status;
    ProMdl current_model = NULL;
    ProName w_model_name;
    ProMdlType model_type;
    char name_str[PRO_NAME_SIZE] = {0};
    char log_msg[512];
    int window_id;

    write_log("=== MachIntell Creo Toolkit Test ===");
    write_log("Button clicked - Toolkit integration VERIFIED");

    ProTKPrintf("\n----------------------------------------------\n");
    ProTKPrintf("  MachIntell Creo Toolkit Test - SUCCESSFUL\n");
    ProTKPrintf("----------------------------------------------\n");

    status = ProWindowCurrentGet(&window_id);
    if (status == PRO_TK_NO_ERROR)
    {
        write_log("ProWindowCurrentGet: OK (Creo API access working)");
    }

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
        ProMessageDisplay(MSGFIL, "TEST_WITH_MODEL %0s", name_str);
    }
    else
    {
        write_log("No active model open (this is OK for basic verification)");
        ProTKPrintf("  No model currently open.\n");
        ProMessageDisplay(MSGFIL, "TEST_NO_MODEL");
    }

    ProTKPrintf("  Log file: machintell_test_log.txt\n");
    ProTKPrintf("----------------------------------------------\n\n");

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
        uiProe2ndImmediate,
        AccessDefault,
        PRO_B_TRUE,
        PRO_B_TRUE,
        &cmd_id);

    if (status != PRO_TK_NO_ERROR)
    {
        write_log("ERROR: ProCmdActionAdd failed");
        ProStringToWstring(errbuf, "MachIntell: Failed to register command");
        return (-1);
    }

    status = ProMenubarmenuPushbuttonAdd(
        "Utilities",
        "MachIntell_Test",
        "MachIntell_Test",
        "MachIntell_Test_Help",
        NULL,
        PRO_B_TRUE,
        cmd_id,
        MSGFIL);

    if (status == PRO_TK_NO_ERROR)
    {
        write_log("Menu button added to: Utilities > MachIntell Test");
    }
    else
    {
        write_log("NOTE: Utilities menu item not added (ribbon-only mode).");
    }

    write_log("Initialization complete.");
    write_log("========================================");

    ProTKPrintf("\n** MachIntell Test App loaded successfully **\n");
    ProTKPrintf("** Find 'MachIntell Test' in Utilities menu **\n\n");

    return (0);
}

/*--------------------------------------------------------------------
 * user_terminate - Called by Creo when the application is unloaded
 *--------------------------------------------------------------------*/
void user_terminate()
{
    write_log("MachIntell Test App - Unloaded cleanly.");
    ProTKPrintf("** MachIntell Test App unloaded **\n");
}
