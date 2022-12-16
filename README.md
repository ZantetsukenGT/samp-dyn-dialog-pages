# Dynamic Dialog Pages
Yet another Dialog Pages Library, leverages y_malloc, y_dialog, y_inline and y_va.

Heavily inspired by Nickk888SAMP's own library
## Installation

Simply install to your project:

```bash
sampctl install ZantetsukenGT/samp-dyn-dialog-pages
```

Include in your code and begin using the library:

```pawn
#include <dyn-dialog-pages>
```

## Functions
Adds an item to the dialog.
```pawn
DynDialog_AddItem(playerid, additional_data, const itemstring[], va_args<>);
```
Resets the lister for the dialog.
```pawn
DynDialog_Clear(playerid);
```
Shows the dialog
```pawn
DynDialog_Show(playerid, const callback[], DIALOG_STYLE: style, const title[], const button1[], const button2[] = "", const items_per_page, const nextbutton[] = "{FF0000}>>>", const backbutton[] = "{FF0000}<<<");
```

## How to Use
To add items to a paged dialog, use the function "```DynDialog_AddItem```".
* ```playerid``` - The player you want to add items to the dialog.
* ```additional_data``` - Associate an arbitrary value to an item, like a database id or weapon id
* ```itemstring[]``` - The item as a string without ```\n```
* ```va_args<>``` - Optional additional arguments to format the itemstring argument

To show the paged dialog, use function "```DynDialog_Show```".
* ```playerid``` - The player you want to show the dialog.
* ```callback[]``` - A public or hook function which will be called when one of your items is picked".
* ```style``` - The style of the dialog.
* ```title[]``` - The title of the dialog.
* ```button1[]``` - The first button of the dialog.
* ```button2[]``` - The second button of the dialog.  (Optional)
* ```items_per_page``` - The amount of items showed on a single page.
* ```nextbutton[]``` - The "Next" button string. (Optional)
* ```backbutton[]``` - The "Back" button string. (Optional)

If you want to clear the ```Items Cache```, you can always use ```DynDialog_Clear(playerid)```, it's optional because it's always cleared when the first item has been added after a paged dialog has been showed to the player.

To get the response of the Dialog, create a new callback by either forwarding or by hook

```hook Name_Of_The_Dialog(playerid, additional_data, response, listitem, inputtext[])```
* ```playerid``` - The player who responded to the Dialog.
* ```additional_data``` - The arbitrary value you associated to this item.
* ```response``` - Did the player clicked Button1 or Button2.
* ```listitem``` - The selected listitem of the dialog.
* ```inputtext[]``` - The selected listitem's text as a string.

## Example
```pawn
#include <a_samp>
#include <dyn-dialog-pages>
#include <YSI_Visual\y_commands>
#include <YSI_Coding\y_hooks>

YCMD:test(playerid, params[], help)
{
	for(new i; i < 250; i++)
	{
		DynDialog_AddItem(playerid, i % 2, "{FFFFFF}List Item {FF00FF}%i", i);
	}
	DynDialog_Show(playerid, "MyPagedDialog", DIALOG_STYLE_LIST, "{FFFFFF}Test Dialog Name {FF00FF}DialogName", "Button 1", "Button 2", 15);
	return 1;
}

hook MyPagedDialog(playerid, additional_data, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	va_SendClientMessage(playerid, -1, "[DDP] You selected listitem ID: {666666}%i{FFFFFF}, extra_data: {666666}%i{FFFFFF}, listitem's text: {666666}%s", listitem, additional_data, inputtext);
	return 1;
}
```

[Example Script](https://github.com/ZantetsukenGT/samp-dyn-dialog-pages/blob/main/ddp-examples.pwn)
