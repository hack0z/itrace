-- the base package
option("base")
    
    -- set category
    set_default(true)
    set_category("package")
   
    -- add links
    if is_os("windows") then add_links("ws2_32") 
    elseif is_os("android") then add_links("m", "c") 
    elseif is_os("macosx", "ios") then 
        add_cxflags("-framework Foundation") 
        add_mxflags("-framework Foundation") 
        add_ldflags("-framework Foundation") 
        add_shflags("-framework Foundation") 
    else add_links("pthread", "dl", "m", "c") end

