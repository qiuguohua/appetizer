-- Copyright (C) 2008 Laurent Cozic. All right reserved.
-- Use of this source code is governed by a GNU/GPL license that can be
-- found in the LICENSE file.

function shortcutTest()
	trace("Creating new dockItem...")
	s = DockItem:new(false)

	s:setPath("C:\\WINDOWS\\system32\\calc.exe")
	trace("Path = ", s:getPath())

	s:setName("Some random name")
	trace("Name (before autoSetName) = ", s:getName())
	s:autoSetName()
	trace("Name (after autoSetName) = ", s:getName())

	s:setParameters("-help")
	trace("Parameters = ", s:getParameters())
	s:setParameters("")

	trace("Belongs to MLG = ", s:belongsToMultiLaunchGroup())
	s:addToMultiLaunchGroup()
	trace("Belongs to MLG = ", s:belongsToMultiLaunchGroup())
	s:removeFromMultiLaunchGroup()
	trace("Belongs to MLG = ", s:belongsToMultiLaunchGroup())

	trace("Adding dockItem to root...")
	root = appetizer:getDockItemsRoot()
	root:addChild(s)

	trace("Launching dockItem...")
	s:launch()

	trace("Number of children on root: ", root:childrenCount())

	trace("Getting first child")
	firstChild = root:getChildAt(0)

	trace("Getting parent")
	parent = firstChild:getParent()
	trace("Parent name is (should be root): ", parent:getName())

	trace("Inserting calc.exe dockItem at index 0")
	root:insertChildAt(s, 0)

	trace("Removing the child that was previously the first one: ", firstChild:getName())
	firstChild:removeFromParent()
end
	
	
	
function groupTest()
	trace("Creating new group...")
	group = DockItem:new(true)	
	group:setName("test group")
	
	trace("Duplicating all the shortcuts on the dock, and adding them to the group")
	
	root = appetizer:getDockItemsRoot()	
	childrenCount = root:childrenCount()
	
	for i = 0, (childrenCount - 1) do
		child = root:getChildAt(i)
		
		trace("Duplicating dockItem: ", child:getName())
	
		dockItem = DockItem:new()
		dockItem:setPath(child:getPath())
		dockItem:autoSetName()
		
		group:addChild(dockItem)
	end	
	
	trace("Adding group to root")	
	root:addChild(group)	
end



function dialogTest()
	result = dialogs:showMessage("This is an error message with an ok button", "ok", "error")
	dialogs:showMessage("'"..result.."' was clicked")
	
	result = dialogs:showMessage("This is a warning message with yes / no buttons", "yesNo", "warning")
	dialogs:showMessage("'"..result.."' was clicked")	
	
	result = dialogs:showMessage("This is a confirmation message with yes / no / cancel buttons", "yesNoCancel", "confirmation")
	dialogs:showMessage("'"..result.."' was clicked")	
	
	dialogs:showMessage("Showing eject dialog...")	
	dialogs:showEjectDriveDialog()
	
	dialogs:showMessage("Showing 'new dockItem' dialog...")	
	dialogs:showNewDockItemDialog()	
	
	dialogs:showMessage("Showing 'config' dialog...")	
	dialogs:showConfigDialog()	
	
	dialogs:showMessage("Showing 'import' dialog...")	
	dialogs:showImportDialog()	
end



function applicationTest()
	dialogs:showMessage("Hidding application")
	appetizer:hide()	
	trace("Application visible = ", appetizer:isVisible())
	
	dialogs:showMessage("Showing application")
	appetizer:show()
	trace("Application visible = ", appetizer:isVisible())

	dialogs:showMessage("Closing option panel")
	optionPanel:close()		
	trace("Option panel is open = ", optionPanel:isOpen())
	
	dialogs:showMessage("Opening option panel")
	optionPanel:open()	
	trace("Option panel is open = ", optionPanel:isOpen())
	
	currentOrientation = appetizer:getOrientation()
	dialogs:showMessage("Current orientation is: "..currentOrientation..". Rotating it...")
	
	if currentOrientation == "vertical" then
		appetizer:setOrientation("horizontal")
	else
		appetizer:setOrientation("vertical")
	end
	
	dialogs:showMessage("And back to normal...")
	appetizer:setOrientation(currentOrientation)
	
	result = dialogs:showMessage("Do you wish to install the autorun.inf file on "..appetizer:getDrive().."?", "yesNo", "confirmation")
	
	if result == "yes" then
		appetizer:installAutoRunFile()
	end
	
	dialogs:showMessage("Showing help file...")
	appetizer:showHelpFile()
	
	dialogs:showMessage("Doing Multilaunch (only works if there are items in the multilaunch group)")
	appetizer:doMultiLaunch()	
	
	dialogs:showMessage("Closing...")
	appetizer:close()
end



function command_callback(event)
	dialogs:showMessage("Callback has been called with this output: "..event.output)	
end



function systemTest()
	dialogs:showMessage('Running cmd /c "dir c: /On"')
	result = system:runCommand('cmd /c "dir c: /On"')
	dialogs:showMessage(result)	
	
	result = dialogs:showMessage("Do you wish to close all the apps on "..appetizer:getDrive().."?", "yesNo", "confirmation")
	
	if result == "yes" then
		system:killLockingProcesses(appetizer:getDrive(), true)
	end
	
	dialogs:showMessage('Running cmd /c "dir c: /On" in asynchronous mode')
	result = system:runCommand('cmd /c "dir c: /On"', true, "command_callback")
end



function optionButton_click(event)
	trace("Option button was clicked!")
	
	trace("Building menu to select test units...")
	
	menu = Menu:new("Test units")
	
	menuItem = MenuItem:new("DockItem test")
	menuItem:setOnSelected("shortcutTest")
	menu:append(menuItem)
	
	menuItem = MenuItem:new("Group test")
	menuItem:setOnSelected("groupTest")
	menu:append(menuItem)
	
	menuItem = MenuItem:new("Dialog test")
	menuItem:setOnSelected("dialogTest")
	menu:append(menuItem)
	
	menuItem = MenuItem:new("Application test")
	menuItem:setOnSelected("applicationTest")
	menu:append(menuItem)
	
	menuItem = MenuItem:new("System test")
	menuItem:setOnSelected("systemTest")
	menu:append(menuItem)
	
	trace("Get the object that has sent the event, in this case the option button")
	optionButton = event.sender
	
	trace("Showing menu")	
	optionButton:popupMenu(menu)
end



function appetizer_trayIconMenuOpening(event)
	menu = event.menu

	menu:appendSeparator()
	
	menuItem = MenuItem:new("Do dialog test (Test Unit Plugin)")
	menuItem:setOnSelected("dialogTest")
	
	menu:append(menuItem)
end



trace("Creating new button")	
button = OptionButton():new()

trace("Setting tooltip");
button:setToolTip("My plugin button")

trace("Adding button to option panel")
optionPanel:addButton(button)

button:addEventListener("click", "optionButton_click")
appetizer:addEventListener("trayIconMenuOpening", "appetizer_trayIconMenuOpening")

trace("Enumerating option buttons...")

buttonCount = optionPanel:buttonCount()

for i = 0, (buttonCount - 1) do
	button = optionPanel:getButtonAt(i)
	trace("Button name = ", button:getName())
	trace("Button tooltip = ", button:getToolTip())
end

