# Rokoko Studio Live for Maya

This plugin lets you stream animation data from Rokoko Studio into Maya.

![pluginManager](images/pluginView.png)

## Requirements

Maya 2020 or higher

## Installation

* Download the plugin: `link`
* In Maya go to Windows -> Settings/Preferences -> Plug-in Manager
    ![pluginManager](images/openPluginManager.png)
* Browse downloaded mll
    ![browsePlugin](images/browsePlugin.png)
* And enable it
    ![loadPlugin](images/loadPlugin.png)
* Go to script editor (Windows -> General Editors -> Script Editor)
and type `showRSLM` command
    ![scriptCmd](images/scriptCmd.png)
* Select command text and middle click drag it onto the shelf
    ![dragScript](images/dragScript.png)

## Plugin anatomy

Plugin's UI consists of 3 collapsible categories.

![anatomy](images/anatomy.png)

1. **Rokoko Studio Live** - Is the main category where user will see studio contents, map/unmap entities.
2. **Studio Command API** - Functionalities for calibrating and recording in Rokoko Studio. See [detailed description](https://rokokoelectronics.github.io/studio-command-api-docs/) here.
3. **Info** - Links to documentation and tutorials.

# Usage

First of all, make sure Rokoko Studio is running and live streaming **Custom** option is enabled.
Select `JSON v2` as data format and `UDP` as protocol. Leave `Port` and `Forward IP` values by default.
![enableStreaming](images/customStreaming.png)

Than go to Maya and hit `Start Receiver` button.
You will see received objects below in a tree structure that resembles scene overview in Studio.
![startReceiver](images/startReceiver.png)

## General workflow idea
All interactions between studio objects and maya objects are made by context menus. User selects
something in maya and this selection will be considered by context menu actions.
![ctxMenus](images/ctxMenus.gif)

## Streaming virtual production data

### Start streaming (Map objects)

After starting the receiver, select the object in maya viewport that
you want to associate with studio object. Right click on studio object in tree view
and select `"Map to selected objects"` entry.
![mapProp](images/mapProp.gif)

### Stop streaming (Unmap objects)
Right click on studio object in tree view and
select `Unmap selected objects` or `Unmap all` option.
![mapProp](images/unmapProp.gif)

> **NOTE**: Same valid for tracker objects ( <img src="resources/icon-vp-32.png" height="16"> )

> **HINT**: Transformations applied to all 6 channels including scale. You may want to scale or offset your maya object, just put it into a group and map this group
instead.

## Streaming character data

After starting the receiver, right click on character studio object in tree view
and select `"Create HIK skeleton"`. Once new skeleton created select, right click character again
and select `"Map to active character"`. Currently active character in HIK panel will be mapped.

![mapCharacters](images/mapCharacters.gif)

> **NOTE**: In order to map your custom character, you must define it in HIK panel,
and set studio mapped character as source.

## Streaming face data

After starting the receiver, select geometry in maya viewport that hosts face blend shapes, right click on face studio object in tree view
and select `"Map to selected objects"`. After face geometry is mapped you need to map weight names, to do
that right click face studio object again and select `"Automap weights"`.
![faceMapping](images/faceMapping.gif)