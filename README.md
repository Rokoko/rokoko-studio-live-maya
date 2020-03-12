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

## Usage

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

