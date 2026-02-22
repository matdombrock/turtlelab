teleport(128, 128)
color(255, 255, 255)
local function loop(i, growth, limit, iters)
  local v = (i * growth) % limit
  forward(v)
  rotateCW(v)
  if i < iters then
    loop(i + 1, growth, limit, iters)
  end
end

loop(1, 3, 16, 128)
color(255, 0, 0)
loop(1, 5, 32, 128)
color(0, 255, 0)
loop(1, 7, 64, 128)
color(0, 0, 255)
loop(1, 11, 128, 128)

print("Done!")
