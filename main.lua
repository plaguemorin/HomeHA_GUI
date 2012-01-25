
-------------------------
-- Main LUA script     --
-- Home remote example --
-------------------------

local onMessage = function(headers, body)
    print("In callback")

    for i,v in pairs(headers) do print(i,v) end

    print(body)
    print("out of callback")
end

Core.onMessage(onMessage)

image = Image.load("data/picture_frame.png")
bg = Image.load("data/background.png")

w = Widget.new("VOLUME")
w:label("Volume")
w:size(132, 109)
w:image(image)


b = Widget.new("POWEROFF")
b:label("POWEROFF")
b:size(200, 200)
b:position(150, 200)

c = Widget.new("POWERON")
c:label("Poweron")
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

tvPage:background(bg)
-- GUI.backgroundColor(0.5, 0, 0)
tvPage:present()
