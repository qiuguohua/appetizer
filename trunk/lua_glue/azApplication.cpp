/*
  Copyright (C) 2008 Laurent Cozic. All right reserved.
  Use of this source code is governed by a GNU/GPL license that can be
  found in the LICENSE file.
*/



/**
 * This class represents an application. You do not need to directly create an instance
 * of it. Instead, directly use the methods and properties of the <code>appetizer</code> global object.
 *     
 * @beginEventTable
 * @event MenuOpening trayIconMenuOpening when the tray icon menu is about to be displayed
 * @event DockItemMenuOpening dockItemMenuOpening when a dock item menu is about to be displayed
 * @event DockItemClick dockItemClick when a dock item is clicked
 * @event Ready ready when the application is fully ready
 * @event Close close when the application is about to be closed
 * @endEventTable
 * @see Global#appetizer
 *
 */	



#include "../stdafx.h"

#include "azApplication.h"
#include "azDockItem.h"
#include "azGlobal.h"
#include "LuaUtil.h"
#include "../Plugin.h"
#include "../FilePaths.h"
#include "../MiniLaunchBar.h"



//*****************************************************************
//
// LUNAR DATA
//
//*****************************************************************

const char azApplication::className[] = "Application";

#define method(class, name) {#name, &class::name}

Lunar<azApplication>::RegType azApplication::methods[] = {
  method(azApplication, addEventListener),
  method(azApplication, getDockItemsRoot),
  method(azApplication, getDockItemById),
  method(azApplication, hide),
  method(azApplication, show),
  method(azApplication, close),
  method(azApplication, isVisible),
  method(azApplication, setOrientation),
  method(azApplication, getOrientation),
  method(azApplication, doMultiLaunch),
  method(azApplication, installAutoRunFile),
  method(azApplication, getDrive),
  method(azApplication, showHelpFile),
  method(azApplication, getSkinNames),  
  method(azApplication, setSkin),
  method(azApplication, enable),  
  method(azApplication, disable),
  method(azApplication, getDirectory),
  method(azApplication, getFilePath),
  method(azApplication, getDataDirectory),
  method(azApplication, showTrayIcon),
  {0,0}
};


//*****************************************************************
//
// NON-EXPORTED MEMBERS
//
//*****************************************************************


MiniLaunchBar* azApplication::Get() const { return &(wxGetApp()); }


//*****************************************************************
//
// EXPORTED MEMBERS
//
//*****************************************************************


azApplication::azApplication(lua_State *L) {
  luaL_error(L, "This object cannot be directly created. Instead use the global 'appetizer' object.");
}


/**
 * This method lets you register event listener functions with the specified control or object.
 * The event listener functions always receive an <code>event</code> object as a parameter. This
 * event has, among other, a <code>sender</code> property which is a reference to the object that has
 * dispatched the event. This object may also have additional properties which depend on the
 * the event.
 * @param String eventName The event you wish to listen to.
 * @param String callback The Lua function to call when the event is triggered.
 * @example The following code registers a listener for the "trayIconMenuOpening" event,
 * which is triggered when the user right-click on the tray icon. A menu item is then added
 * to the menu.
 * <listing version="3.0">
 * function appetizer_trayIconMenuOpening(event)
 *     trace("Sender: "..event.sender) -- the application is the sender
 *  
 *     -- The tray icon menu is about to be shown, but before it is
 *     -- we add a menu item to it.
 *     menu = event.menu -- get the tray icon menu
 *  	
 *     menu:appendSeparator()
 *  
 *     menuItem = MenuItem:new("Tray Icon Test")
 *  	
 *     menu:append(menuItem)
 * end
 * 
 * appetizer:addEventListener("trayIconMenuOpening", "appetizer_trayIconMenuOpening")
 * 
 * trace("Listening to the "trayIconMenuOpening" event...")
 * </listing>
 * 
 */	
int azApplication::addEventListener(lua_State *L) {
  wxString eventName = LuaUtil::ToString(L, 1);
  wxString functionName = LuaUtil::ToString(L, 2);
  Plugin* p = wxGetApp().GetPluginManager()->GetPluginByLuaState(L);
  p->AddEventListener(this->Get(), eventName, functionName);
  return 0;
}


/**
 * Returns the root of all the dock items. Dock items are organized in a tree-like structure
 * with groups being the branches and the shortcut being the leafs. This function allows getting the
 * root of this structure. You can then call <code>childrenCount()</code>, <code>getChildAt()</code>, etc. to
 * access the children.
 * @return DockItem The root of all the dock items.
 * @example The following code gets the dock items' root and prints the name of all its children.
 * <listing version="3.0">
 * root = appetizer:getDockItemsRoot()
 * childrenCount = root:childrenCount()
 * 
 * for i = 0, (childrenCount - 1) do
 *     subDockItem = root:getChildAt(i)
 *     trace(subDockItem:getName())
 * end
 * </listing>
 * 
 */	
int azApplication::getDockItemsRoot(lua_State *L) {
  appFolderItem* rootFolderItem = wxGetApp().GetUser()->GetRootFolderItem();
  Lunar<azDockItem>::push(L, new azDockItem(rootFolderItem), true);

  return 1;
}


/**
 * Gets the dock item with the given id or <code>nil</code> if it does not exist or has been deleted. Note
 * that this function will also look for the dock item in all the groups and subgroups starting from the root.
 * @return DockItem The dock item or <code>nil</code> if does not exists.
 * 
 */	
int azApplication::getDockItemById(lua_State *L) {
  int folderItemId = luaL_checkinteger(L, 1);
  appFolderItem* folderItem = appFolderItem::GetFolderItemById(folderItemId);
  Lunar<azDockItem>::push(L, new azDockItem(folderItem), true);
  
  return 1;
}


/**
 * Hides the application.
 * 
 */	
int azApplication::hide(lua_State *L) { wxGetApp().GetMainFrame()->Hide(); return 0; }

/**
 * Shows the application.
 * 
 */	
int azApplication::show(lua_State *L) { wxGetApp().GetMainFrame()->Show(); return 0; };

/**
 * Closes the application.
 * 
 */	
int azApplication::close(lua_State *L) {
  // Don't directly close the app, but schedule a close operation
  // This allows the script and plugin to finish properly
  wxGetApp().GetMainFrame()->ScheduleCloseOperation();
  luaL_error(L, "Closing application (This is NOT an error)"); // This is just to force the script to exit  
  return 0;
};

/**
 * Tells whether the application is visible or not.
 * @return Boolean <code>true</code> if the application is visible.
 * @example The following code toggles the application visibility.
 * <listing version="3.0">
 * function hideShowToggle()
 *     if appetizer:isVisible() then
 *         appetizer:hide()
 *     else
 *         appetizer:show()
 *     end
 * end
 * 
 * hideShowToggle()
 * </listing>
 * 
 */	
int azApplication::isVisible(lua_State *L) { lua_pushboolean(L, wxGetApp().GetMainFrame()->IsVisible()); return 1; }


/**
 * Sets the application orientation.
 * @param String orientation Can be "horizontal" or "vertical".
 * @example The following code rotates the application.
 * <listing version="3.0">
 * function rotateApplication()
 *     if appetizer:getOrientation() == "horizontal" then
 *         appetizer:setOrientation("vertical")
 *     else
 *         appetizer:setOrientation("horizontal")
 *     end
 * end
 * 
 * rotateApplication()
 * </listing>
 * 
 */	
int azApplication::setOrientation(lua_State *L) {
  wxString orientation = LuaUtil::ToString(L, 1);

  if (orientation == _T("vertical")) {
     if (wxGetApp().GetMainFrame()->GetRotated()) return 0;
     wxGetApp().GetMainFrame()->SetRotated(true, true);
  } else { // default to "horizontal"
     if (!wxGetApp().GetMainFrame()->GetRotated()) return 0;
     wxGetApp().GetMainFrame()->SetRotated(false, true);
  }

  return 0;
}


/**
 * Gets the application orientation.
 * @return String Can be "horizontal" or "vertical".
 * @see #setOrientation()
 * 
 */	
int azApplication::getOrientation(lua_State *L) { LuaUtil::PushString(L, wxGetApp().GetMainFrame()->GetRotated() ? _T("vertical") : _T("horizontal")); return 1; }

/**
 * Triggers multi-launch functionality. See Appetizer's help file for some information about this feature.
 * 
 */	
int azApplication::doMultiLaunch(lua_State *L) { wxGetApp().GetUtilities().DoMultiLaunch(); return 0; }


/**
 * Installs Appetizer's autorun.inf file at the root of the current drive.
 * 
 */	
int azApplication::installAutoRunFile(lua_State *L) { wxGetApp().GetUtilities().InstallAutorunFile(); return 0; }


/**
 * Returns the drive on which the application is running.
 * @return String Drive name.
 * 
 */	
int azApplication::getDrive(lua_State *L) { LuaUtil::PushString(L, FilePaths::GetApplicationDrive()); return 1; }


/**
 * Opens the help file.
 * @param String sectionName The section to open the help file at. (default "")
 * 
 */	
int azApplication::showHelpFile(lua_State *L) {
  wxString anchor = LuaUtil::ToString(L, 1, true);
  wxGetApp().GetUtilities().ShowHelpFile(anchor);
  return 0;
}


/**
 * Returns the list of skins currently installed.
 * @return Array The list of string names.
 */	
int azApplication::getSkinNames(lua_State *L) {
  wxArrayString skinNames = wxGetApp().GetUtilities().GetSkinNames();

  lua_createtable(L, skinNames.size(), 0);
  int tableIndex = lua_gettop(L);

  for (int i = 0; i < skinNames.Count(); i++) {
    wxString skinName = skinNames[i];

    lua_pushinteger(L, i + 1);
    LuaUtil::PushString(L, skinName);
    lua_settable(L, tableIndex);
  }

  return 1;
}


/**
 * Switches the skin to the given skin name. Use <code>getSkinNames</code>
 * to get the list of possible skins.
 * @param String skinName The skin name.
 * @see #getSkinNames()
 */	
int azApplication::setSkin(lua_State *L) {
  wxString skinName = LuaUtil::ToString(L, 1);
  wxGetApp().GetUtilities().SwitchSkin(skinName);
  
  return 0;
}


/**
 * Enables the application.
 * @param Boolean enable Sets this to <code>false</code> to disable the application instead (default true)
 * @see #disable()
 */	
int azApplication::enable(lua_State *L) {
  bool enable = LuaUtil::ToBoolean(L, 1, true, true);

  if (enable) {
    wxGetApp().GetMainFrame()->Enable();
  } else {
    wxGetApp().GetMainFrame()->Disable();
  }

  return 0;
}


/**
 * Disables the application.
 * @see #enable()
 */	
int azApplication::disable(lua_State *L) {
  wxGetApp().GetMainFrame()->Disable();

  return 0;
}


/**
 * Gets the application directory
 * @return String The application directory
 */	
int azApplication::getDirectory(lua_State *L) {
  LuaUtil::PushString(L, FilePaths::GetApplicationDirectory());
  return 1;
}


/**
 * Gets the application file path
 * @return String The application file path
 */	
int azApplication::getFilePath(lua_State *L) {
  LuaUtil::PushString(L, FilePaths::GetApplicationPath());
  return 1;
}


/**
 * Gets the application data directory
 * @return String The application data directory
 */	
int azApplication::getDataDirectory(lua_State *L) {
  LuaUtil::PushString(L, FilePaths::GetDataDirectory());
  return 1;
}


/**
 * Show or hide the application tray icon
 * @param Boolean enable Sets this to <code>false</code> to hide the application tray icon (default true)
 */	
int azApplication::showTrayIcon(lua_State *L) {
  bool doShow = LuaUtil::ToBoolean(L, 1, true, true);
  wxGetApp().GetMainFrame()->ShowTrayIcon(doShow);

  return 0;
}