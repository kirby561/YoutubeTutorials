import bpy
import bmesh
import os

outputPath = 'S:/Documents/Unreal Projects/TutorialProject/output.png'
zScale = 100

class HeightmapInfo:
    def __init__(self, numXVertices, numYVertices, heightmap):
        self.NumXVertices = numXVertices
        self.NumYVertices = numYVertices
        self.Heightmap = heightmap
     
def IsApproxZero(val, epsilon = 0.001):
    if val < epsilon and val > -epsilon:
        return True
    return False
        
def MeshToHeightmap(obj):
    # Get the bounding box of the mesh
    mesh = obj.data
    minX = 99999999
    maxX = -99999999
    minY = 99999999
    maxY = -99999999
    minZ = 99999999
    maxZ = -99999999
    
    for v in mesh.vertices:
        if (v.co.x < minX): minX = v.co.x
        if (v.co.x > maxX): maxX = v.co.x
        if (v.co.y < minY): minY = v.co.y
        if (v.co.y > maxY): maxY = v.co.y
        if (v.co.z < minZ): minZ = v.co.z
        if (v.co.z > maxZ): maxZ = v.co.z

    # Determine the number of vertices in each direction
    numXVertices = 0
    numYVertices = 0
    for v in mesh.vertices:
        xDiff = v.co.x - minX
        yDiff = v.co.y - minY
        if IsApproxZero(xDiff):
            numXVertices += 1
        if IsApproxZero(yDiff):
            numYVertices += 1
            
    # Determine the resolution
    xRange = maxX - minX
    yRange = maxY - minY
    xDelta = (xRange) / (numXVertices - 1)
    yDelta = (yRange) / (numYVertices - 1)
    
    # Create the heightmap
    heightmap = [0] * numXVertices * numYVertices

    # When importing into UE, a 1 in the heightmap with a z scale of 1 will be at +255.992cm
    # and a 0 in the heightmap will be at -256cm
    heightmapWorldRangeCm = 511.992
    
    for v in mesh.vertices:
        x = int(round((v.co.x - minX) / xDelta))
        y = int(round((v.co.y - minY) / yDelta))
        
        zUnscaledCm = v.co.z * 100.0 / zScale
        
        # Normalize between 0 and 1
        zNormalized = (zUnscaledCm / heightmapWorldRangeCm) + 0.5
        heightmap[x + y * numXVertices] = zNormalized

    return HeightmapInfo(numXVertices, numYVertices, heightmap)

def SaveHeightmap(heightmapInfo, filepath):
    heightmap = heightmapInfo.Heightmap
    numXVertices = heightmapInfo.NumXVertices
    numYVertices = heightmapInfo.NumYVertices
    
    image = bpy.data.images.new("Heightmap", width=numXVertices, height=numYVertices, alpha=True, float_buffer=True)
    image.file_format = 'PNG'
    image.colorspace_settings.name = 'Non-Color'
    
    pixels = [0] * numYVertices * numXVertices * 4
    for y in range(numYVertices):
        for x in range(numXVertices):
            height = heightmap[x + y * numXVertices]
            index = (x + y * numXVertices) * 4
            pixels[index] = height
            pixels[index + 1] = height
            pixels[index + 2] = height
            pixels[index + 3] = 1
        
    image.pixels = pixels
    image.update()
    
    image.save(filepath=filepath)
        

obj = bpy.context.active_object
if obj and obj.type == 'MESH':
    heightmapInfo = MeshToHeightmap(obj)
    SaveHeightmap(heightmapInfo, outputPath)