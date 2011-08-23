#!BPY
"""
Name: 'Game Engine Testbed Level Export'
Blender: 245
Group: 'Export'
Tooltip: 'Creats a Lua file representing the objects in a scene.'
"""

# MIT License - See LICENSE file.

from Blender import Window, sys
import bpy

def export_to_lua(sce):

    # Remove these when writing your own tool
    print 'Blend object count', len(bpy.data.objects)
    print 'Scene object count', len(sce.objects)

    f = open( '/home/nigel/prog/engine/scripts/levels/level2.lua', 'w' )

    f.write( '-- Generated by level_export.py from Blender 2.49\n' )
    f.write( '-- Copyright 2011 Nigel Atkinson.  See LICENSE file for license.\n\n' )
    f.write( 'meshes = {}\n' )
    f.write( 'cameras = {}\n' )
    f.write( 'points = {}\n' )
    f.write( '-------------\n\n' )

    for ob in [ x for x in sce.objects if x.type == 'Mesh']:
        print ob
        f.write( 'mesh = {}\n' )
        f.write( 'mesh.name = \'' + ob.name + '\'\n' )
        f.write( 'mesh.filename = \'' + ob.getData( name_only=True ) + '.mesh\'\n' )
        f.write( 'mesh.loc = { ' + str(ob.LocX) + ', ' + str(ob.LocZ) + ', ' + str(-ob.LocY) + ' }\n' )
        q = ob.getMatrix().toQuat()
        f.write( 'mesh.rot = { ' + str(q.w) + ', ' + str(q.x) + ', ' + str(q.z) + ', ' + str(-q.y) + ' }\n' )
        f.write( 'mesh.scale = { ' + str(ob.SizeX) + ', ' + str(ob.SizeZ) + ', ' + str(ob.SizeY) + ' }\n' )
        f.write( 'meshes[mesh.name] = mesh\n' )
        writeProperties( f, ob, 'mesh' )
        f.write( '-------------\n\n' )

    for ob in [ x for x in sce.objects if x.type == 'Camera']:
        print ob
        f.write( 'camera={}\n' )
        f.write( 'camera.name = \'' + ob.name + '\'\n' )
        f.write( 'camera.loc = { ' + str(ob.LocX) + ', ' + str(ob.LocZ) + ', ' + str(-ob.LocY) + ' }\n' )
        q = ob.getMatrix().toQuat()
        f.write( 'camera.rot = { ' + str(q.w) + ', ' + str(q.x) + ', ' + str(q.z) + ', ' + str(-q.y) + ' }\n' )
        f.write( 'cameras[camera.name] = camera\n' )
        writeProperties( f, ob, 'camera' )
        f.write( '-------------\n\n' )

    for ob in [ x for x in sce.objects if x.type == 'Empty']:
        print ob
        f.write( 'point={}\n' )
        f.write( 'point.name = \'' + ob.name + '\'\n' )
        f.write( 'point.loc = { ' + str(ob.LocX) + ', ' + str(ob.LocZ) + ', ' + str(-ob.LocY) + ' }\n' )
        q = ob.getMatrix().toQuat()
        f.write( 'point.rot = { ' + str(q.w) + ', ' + str(q.x) + ', ' + str(q.z) + ', ' + str(-q.y) + ' }\n' )
        f.write( 'points[point.name] = point\n' )
        writeProperties( f, ob, 'point' )
        f.write( '-------------\n\n' )

    f.close()

def writeProperties( f, obj, obj_type ):
    for prop in obj.getAllProperties():
        if prop.type == "FLOAT" or prop.type == "INT":
            f.write( obj_type + "." + prop.name + " = " + str(prop.data) + "\n" )
        elif prop.type == "STRING":
            f.write( obj_type + "." + prop.name + " = '" + prop.data + "'\n" )
        elif prop.type == "BOOL":
            if prop.data == True:
                f.write( obj_type + "." + prop.name + " = true\n" )
            else:
                f.write( obj_type + "." + prop.name + " = false\n" )

def main():

    # Gets the current scene, there can be many scenes in 1 blend file.
    sce = bpy.data.scenes.active

    Window.WaitCursor(1)
    t = sys.time()

    # Run the object editing function
    export_to_lua(sce)

    # Timing the script is a good way to be aware on any speed hits when scripting
    print 'My Script finished in %.2f seconds' % (sys.time()-t)
    Window.WaitCursor(0)


# This lets you import the script without running it
if __name__ == '__main__':
    main()