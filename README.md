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
Adds an item to a player's dynamic dialog contents.
```pawn
DynDialog_AddItem(playerid, additional_data, const itemstring[], va_args<>);
```
Clears the contents for a player's dynamic dialog and hides the dynamic dialog if its shown, won't hide normal dialogs.
```pawn
DynDialog_Destroy(playerid);
```
Shows the dialog
```pawn
DynDialog_Show(playerid, Func: callback<iiiis>, DIALOG_STYLE: style, const title[], const button1[], const button2[] = "", const items_per_page, const nextbutton[] = "{FF0000}>>>", const backbutton[] = "{FF0000}<<<");
```

## How to Use
To start adding items to a dynamic dialog, use `DynDialog_AddItem`.
* `playerid` - The player you want to associate the item with.
* `additional_data` - Associate an arbitrary value to an item, like a player id, database id or weapon id, negative values, get creative!.
* `itemstring[]` - The item as a string without `\n`.
* `va_args<>` - Optional additional arguments to format the `itemstring` argument.
* doesn't return anything.

To show the resulting dialog, use `DynDialog_Show`, won't show anything if there are no items.
* `playerid` - The player you want to show the dialog to.
* `Func: callback<iiiis>` - A public or hook function or even an inline function which will be called when one of your items is picked.
* `style` - The style of the dialog.
* `title[]` - The title of the dialog.
* `button1[]` - The first button of the dialog.
* `button2[]` - The second button of the dialog.  (Optional)
* `items_per_page` - The amount of items showed on a single page.
* `nextbutton[]` - The "Next" button string. (Optional)
* `backbutton[]` - The "Back" button string. (Optional)
* returns 1 if able to show
* returns 0 if already shown or there are no items added (tablist header doesn't count as item)

`DynDialog_Destroy(playerid)` is a wrapper around `HidePlayerDialog` and `ShowPlayerDialog(playerid)` and will behave as expected, you can also use these directly.

To get the response of the dialog, create a new callback by either forwarding or by hook, you can also use inlines!

```pawn
forward myCallback(playerid, response, additional_data, listitem, string: inputtext[]);
public myCallback(playerid, response, additional_data, listitem, string: inputtext[]) { }
//or
hook myCallback(playerid, response, additional_data, listitem, string: inputtext[]) { }
//or
stock MyModule_ShowMyDynamicDialog(playerid)
{
	inline const generic_response(pid, response, additional_data, listitem, string: inputtext[]) { }
	...
}
```
* `playerid` - The player associated to the dialog.
* `response` - Either the player clicked Button1 or Button2.
* `additional_data` - The arbitrary value you associated to this item.
* `listitem` - The selected listitem's absolute index in the dialog (not very useful since added support for `additional_data`).
* `string: inputtext[]` - The selected listitem's text as a string.

## Small Example, if you look for a bigger example script, look [here](https://github.com/ZantetsukenGT/samp-dyn-dialog-pages/blob/main/ddp-examples.pwn)
```pawn
#include <a_samp>
#define YSI_YES_HEAP_MALLOC
#include <dyn-dialog-pages>
#include <YSI_Visual\y_commands>
#include <YSI_Coding\y_hooks>

hook MyDynamicDialog(playerid, response, additional_data, listitem, string: inputtext[])
{
	if (!response)
		return 1;

	va_SendClientMessage(playerid, -1, "[DDP] You selected listitem ID: {666666}%i{FFFFFF}, extra_data: {666666}%i{FFFFFF}, listitem's text: {666666}%s", listitem, additional_data, inputtext);
	return 1;
}

YCMD: test1(playerid, params[], help)
{
	for (new i; i < 250; i++)
	{
		// in this example the 'additional_data' argument means whether the row's index is odd or even
		DynDialog_AddItem(playerid, i % 2, "{FFFFFF}List Item {FF00FF}%i", i);
	}
	DynDialog_Show(playerid, using public MyDynamicDialog<iiiis>, DIALOG_STYLE_LIST, "{FFFFFF}Test Dialog Name {FF00FF}DialogName", "Button 1", "Button 2", 15);
	return 1;
}

// or you can use an inline instead
YCMD: test2(playerid, params[], help)
{
	inline const dialog_response(pid, response, additional_data, listitem, string: inputtext[])
	{
		if (!response)
			return 1;

		va_SendClientMessage(pid, -1, "[DDP-inline] You selected listitem ID: {666666}%i{FFFFFF}, extra_data: {666666}%i{FFFFFF}, listitem's text: {666666}%s", listitem, additional_data, inputtext);
		return 1;
	}
	for (new i; i < 250; i++)
	{
		// in this example the 'additional_data' argument means whether the row's index is odd or even
		DynDialog_AddItem(playerid, i % 2, "{FFFFFF}List Item {FF00FF}%i", i);
	}
	DynDialog_Show(playerid, using inline dialog_response, DIALOG_STYLE_LIST, "{FFFFFF}Test2 Dialog Name {FF00FF}DialogName", "Button 1", "Button 2", 15);
	return 1;
}
```
