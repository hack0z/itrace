-- add demo
function add_demo(name)
   
    -- add target
    target(name)

        -- make as a binary
        set_kind("binary")

        -- add defines
        add_defines("__tb_prefix__=\"" .. name .. "\"")

        -- set the object files directory
        set_objectdir("$(buildir)/.objs")

        -- add packages
        add_options("tbox", "base")

        -- test?
        if name == "test" then
            
            -- add the source files
            add_files("test.m")  
        else
 
            -- add the source files
            add_files(name .. ".c")  

        end

        -- itrace?
        if name == "itracer" then

            -- set package script
            on_package( function (target) 

                            -- ldid -S ./itrace
                            os.run("ldid -S$(projectdir)/res/ios/entitlements.plist %s", target:targetfile())
                        end)

        end

end

-- add demos
add_demo("demo")
add_demo("test")
add_demo("dump")
add_demo("itracer")


