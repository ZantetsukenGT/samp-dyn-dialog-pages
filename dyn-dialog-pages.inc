#if defined dyn_dialog_pages_included
	#endinput
#endif
#define dyn_dialog_pages_included

/**
	Yet another Dialog Pages Library that leverages malloc to avoid declaring super huge arrays at compile time.
	API heavily inspired by Nickk888SAMP's own library.

	Also leverages y_dialog, y_va and y_inline
*/

/** API
DynDialog_AddItem(playerid, const itemstring[], va_args<>);
DynDialog_Show(playerid, const callback[], DIALOG_STYLE: style, const title[], const button1[], const button2[] = "", const items_per_page, const nextbutton[] = "{FF0000}>>>", const backbutton[] = "{FF0000}<<<");
DynDialog_Clear(playerid);
*/

#include <a_samp>
#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_hooks>

#if !defined MAX_DIALOG_ITEMS
	#define MAX_DIALOG_ITEMS (1024 * 8)
#endif

static stock Alloc: s_DialogItems[MAX_PLAYERS][MAX_DIALOG_ITEMS] = { { NO_ALLOC, ... }, ... };
static stock s_ItemLength[MAX_PLAYERS][MAX_DIALOG_ITEMS]		 = { { 0, ... }, ... };
static stock s_ItemCount[MAX_PLAYERS]							 = { 0, ... };
static stock bool: s_Shown[MAX_PLAYERS]							 = { false, ... };

static stock s_CurrentPage[MAX_PLAYERS]							 = { 0, ... };
static stock s_CurrentNextButtonIndex[MAX_PLAYERS]				 = { -1, ... };
static stock s_CurrentBackButtonIndex[MAX_PLAYERS]				 = { -1, ... };

stock DynDialog_Clear(playerid)
{
	for (new i = 0, count = s_ItemCount[playerid]; i < count; i++)
	{
		free(s_DialogItems[playerid][i]);
		s_DialogItems[playerid][i] = NO_ALLOC;
		s_ItemLength[playerid][i]  = 0;
	}

	s_ItemCount[playerid]		= 0;
	s_Shown[playerid]			= false;
	s_CurrentPage[playerid]		= 0;
	s_CurrentNextButtonIndex[0] = -1;
	s_CurrentBackButtonIndex[0] = -1;
}

stock DynDialog_AddItem(playerid, const itemstring[], va_args<>)
{
	if (s_Shown[playerid])
		DynDialog_Clear(playerid);

	new formatted_item[YSI_MAX_STRING];
	va_format(formatted_item, sizeof(formatted_item), itemstring, va_start<2>);
	strcat(formatted_item, "\n");

	new allocIndex						= s_ItemCount[playerid]++;
	s_DialogItems[playerid][allocIndex] = Malloc_NewS(formatted_item);
	s_ItemLength[playerid][allocIndex]	= strlen(formatted_item);
}

static DynDialog_CalculatePage(playerid, items_per_page, DIALOG_STYLE: style, const nextbutton[], const backbutton[])
{
	new dialog_info[4096 + 1], item[YSI_MAX_STRING];
	new bool: hasHeaders	  = style == DIALOG_STYLE_TABLIST_HEADERS;
	new this_page_first_index = s_CurrentPage[playerid] * items_per_page + 1 * _: hasHeaders;
	new items_on_page		  = min(s_ItemCount[playerid] - this_page_first_index, items_per_page);
	new currentLength		  = 0;
	if (hasHeaders)
	{
		Malloc_GetS(item, sizeof(item), s_DialogItems[playerid][0], 0);
		strins(dialog_info, item, currentLength);
		currentLength += s_ItemLength[playerid][0];
	}
	for (new i = this_page_first_index, limit = this_page_first_index + items_on_page; i < limit; i++)
	{
		Malloc_GetS(item, sizeof(item), s_DialogItems[playerid][i], 0);
		strins(dialog_info, item, currentLength);
		currentLength += s_ItemLength[playerid][i];
	}

	if (this_page_first_index + items_on_page == s_ItemCount[playerid]) // must be the last page
	{
		if (s_CurrentPage[playerid] == 0) // and its the first page
		{
			s_CurrentNextButtonIndex[playerid] = s_CurrentBackButtonIndex[playerid] = -1;
			return dialog_info;
		}
		s_CurrentNextButtonIndex[playerid] = -1;
		s_CurrentBackButtonIndex[playerid] = items_on_page;
		strins(dialog_info, backbutton, currentLength);
		return dialog_info;
	}
	if (s_CurrentPage[playerid] == 0) // its the first page and cannot be the last
	{
		s_CurrentNextButtonIndex[playerid] = items_on_page;
		s_CurrentBackButtonIndex[playerid] = -1;
		strins(dialog_info, nextbutton, currentLength);
		return dialog_info;
	}
	s_CurrentNextButtonIndex[playerid] = items_on_page;
	s_CurrentBackButtonIndex[playerid] = items_on_page + 1;
	strins(dialog_info, nextbutton, currentLength);
	currentLength += strlen(nextbutton);
	strins(dialog_info, "\n", currentLength);
	currentLength += 1;
	strins(dialog_info, backbutton, currentLength);
	return dialog_info;
}

stock DynDialog_Show(playerid, const callback[], DIALOG_STYLE: style, const title[], const button1[], const button2[] = "",
					 const items_per_page, const nextbutton[] = "{FF0000}>>>", const backbutton[] = "{FF0000}<<<")
{
	inline const dialog_response(response, listitem, string: inputtext[])
	{
		if (listitem == s_CurrentNextButtonIndex[playerid])
		{
			s_CurrentPage[playerid]++;
			Dialog_ShowCallback(
				playerid, using inline dialog_response, style, title,
				DynDialog_CalculatePage(playerid, items_per_page, style, nextbutton, backbutton), button1, button2);
			return 1;
		}
		else if (listitem == s_CurrentBackButtonIndex[playerid])
		{
			s_CurrentPage[playerid]--;
			Dialog_ShowCallback(
				playerid, using inline dialog_response, style, title,
				DynDialog_CalculatePage(playerid, items_per_page, style, nextbutton, backbutton), button1, button2);
			return 1;
		}
		CallLocalFunction(callback, "iiis", playerid, response, s_CurrentPage[playerid] * items_per_page + listitem, inputtext);
		DynDialog_Clear(playerid);
		return 1;
	}
	s_Shown[playerid] = true;
	Dialog_ShowCallback(
		playerid, using inline dialog_response, style, title,
		DynDialog_CalculatePage(playerid, items_per_page, style, nextbutton, backbutton), button1, button2);
	return 1;
}

hook OnPlayerConnect(playerid)
{
	DynDialog_Clear(playerid);
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	DynDialog_Clear(playerid);
	return 1;
}