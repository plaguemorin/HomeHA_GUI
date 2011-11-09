
-------------------------
-- Main LUA script     --
-- Home remote example --
-------------------------

print(GUI.version())

image = Image.load("data/picture_frame.png")


w = Widget.new("VOLUME")
w:label("Volume")
w:size(132, 109)
w:image(image)


b = Widget.new("POWEROFF")
b:label(w:label())
b:size(200, 200)
b:position(150, 200)

c = Widget.new("POWERON")
c:label(w:label())
c:position(150, 200)
c:size(132, 109)
c:image(image)

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
