#define MAX_PLAYERS 50
#include <a_samp>

//#define MALLOC_MEMORY (3276800)
#define YSI_YES_HEAP_MALLOC
#include <dyn-dialog-pages>
#include <YSI_Visual\y_commands>
#include <YSI_Coding\y_hooks>

/* Callback */
hook test_response(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	SendClientMessage(playerid, -1, "[Dynamic-Dialog-Pages] You have selected listitem ID: {666666}%i{FFFFFF}, listitem's text: {666666}%s", listitem, inputtext);
	return 1;
}

/* Commands */
YCMD: test1(playerid, params[], help) // DIALOG_STYLE_LIST
{
	for(new i; i < MAX_DIALOG_ITEMS / 8; i++)
	{
		DynDialog_AddItem(playerid, "{FFFFFF}List Item {FF00FF}%i", i);
	}
	DynDialog_Show(playerid, #test_response, DIALOG_STYLE_LIST, "{FFFFFF}Test Dialog Name {FF00FF}DDP_Test", "Button 1", "Button 2", 15);
	return 1;
}

YCMD: test2(playerid, params[], help) // DIALOG_STYLE_TABLIST
{
	for(new i; i < MAX_DIALOG_ITEMS / 8; i++)
	{
		DynDialog_AddItem(playerid, "{FFFFFF}List Item {CCCCCC}%i\t{FFFFFF}List Item {FF00FF}%i\t{FFFFFF}List Item {FFFF00}%i\t{FFFFFF}List Item {00FFFF}%i", i, i, i, i);
	}
	DynDialog_Show(playerid, #test_response, DIALOG_STYLE_TABLIST, "{FFFFFF}Test Dialog Name {FF00FF}DDP_Test", "Button 1", "Button 2", 27);
	return 1;
}

YCMD: test3(playerid, params[], help) // DIALOG_STYLE_TABLIST_HEADERS
{
	DynDialog_AddItem(playerid, "{CCCCCC}Column 1\t{FF00FF}Column 2\t{FFFF00}Column 3\t{00FFFF}Column 4");
	for(new i; i < MAX_DIALOG_ITEMS / 8; i++)
	{
		DynDialog_AddItem(playerid, "{FFFFFF}List Item {CCCCCC}%i\t{FFFFFF}List Item {FF00FF}%i\t{FFFFFF}List Item {FFFF00}%i\t{FFFFFF}List Item {00FFFF}%i", i, i, i, i);
	}
	DynDialog_Show(playerid, #test_response, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Test Dialog Name {FF00FF}DDP_Test", "Button 1", "Button 2", 32);
	return 1;
}