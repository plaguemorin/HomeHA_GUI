
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

c = Widget.newButton("POWERON")
c:label(w:label())
c:size(200, 200)
c:position(150, 200)

b:onClick(function(source)
    tvPage:present()
end
)

c:onClick(function(source)
    homePage:present()
end
)

homePage = Page.new("Home")
tvPage = Page.new("TV")

b:attachToPage(homePage)
c:attachToPage(tvPage)
w:attachToPage(tvPage)

tvPage:present()
