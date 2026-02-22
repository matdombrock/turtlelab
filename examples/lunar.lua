teleport(128, 128)
local function loop(len)
  local r = 255
  local g = 255
  local b = 255
  for i = 1, 256 do
    if i == math.floor(256 / 3) then
      b = 128
    end
    if i == math.floor(256 / 3 * 2) then
      g = 128
    end

    color(r, g, b)
    rotateCW(3)
    forward(len)
    goBack(1)
  end
  if len < 128 then
    loop(len + 33)
  end
end

loop(32)

print("Done!")
