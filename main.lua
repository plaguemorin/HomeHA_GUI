
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
    a = Widget.retreive(source)
    a:label("CLICKED !")
    w:label(source)
    a:destory()
end
)

print(w:size())
