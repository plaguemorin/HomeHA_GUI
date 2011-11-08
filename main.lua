
-------------------------
-- Main LUA script     --
-- Home remote example --
-------------------------

print(GUI.version())

w = Widget.newLabel("VOLUME")
w:label("Volume")
w:size(200, 200)

b = Widget.newButton("POWEROFF")
b:label(w:label())
b:size(200, 200)
b:position(150, 200)

b:onClick(function(source)
    GUI.displayPage(tvPage)
end
)

print(w:size())

homePage = Page.new("Home")
tvPage = Page.new("TV")

b:attachToPage(homePage)
w:attachToPage(tvPage)

GUI.displayPage(homePage)

print(homePage)
print(tvPage)
