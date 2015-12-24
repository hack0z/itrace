-- the base package
add_option("base")
    
    -- set category
    set_option_category("package")
   
    -- add links
    if os("windows") then add_option_links("ws2_32") 
    elseif os("android") then add_option_links("m", "c") 
    elseif os("macosx", "ios") then 
        add_option_cxflags("-framework Foundation") 
        add_option_mxflags("-framework Foundation") 
        add_option_ldflags("-framework Foundation") 
    else add_option_links("pthread", "dl", "m", "c") end

