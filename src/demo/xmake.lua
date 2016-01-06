-- add demo
function add_demo(name)
   
    -- add target
    add_target(name)

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
            set_packagescript(  function (target) 
                    
                                    -- import modules
                                    local os = import("os")
                                    local path = import("path")
                                    local project = import("project")
                                    assert(target and os and path and project) 

                                    -- get targetfile
                                    local targetfile = target.archs[project.arch()].targetfile
                                    assert(targetfile)

                                    -- the logfile
                                    local logfile = path.absolute(project.logfile())

                                    -- ldid -S ./itrace
                                    if 0 ~= os.execute(string.format("ldid -S%s %s > %s 2>&1", project.projectdir() .. "/res/ios/entitlements.plist", targetfile, logfile)) then
                                        -- failed
                                        os.cat(logfile)
                                        return -1 
                                    end

                                    -- ok
                                    return 1
                                end)

        end

end

-- add demos
add_demo("demo")
add_demo("test")
add_demo("dump")
add_demo("itracer")


