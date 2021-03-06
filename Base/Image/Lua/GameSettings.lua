function SaveToLuaFile(outputString, overwrite, filepath)
   print("SaveToLuaFile...")
   local f;      -- The file
   if overwrite == 1 then  -- Wipe the contents with new data
      f = assert(io.open(filepath, "w"))
   elseif overwrite == 0 then -- Append with new data
      f = assert(io.open(filepath, "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end


title = "Assignment 2"
width = 1920
height = 1080

walkspeed = 40

gun_ammo = 6

CPlayerInfoStartPos = {x=10,y=0,z=10}

WaypointOne = {x = 200,y = 0,z = 200}
WaypointTwo = {x = 200, y = 0,z = -200}
WaypointThree = {x = -200, y = 0, z = -200}
WaypointFour = {x = -200, y = 0, z = 200}

