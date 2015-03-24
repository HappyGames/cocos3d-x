#ifndef _CC3_MESH_COMMON_H_
#define _CC3_MESH_COMMON_H_

NS_COCOS3D_BEGIN

/**
 * This enum defines the components of a bitwise-OR of flags enumerating the types of vertex
 * content contained within a mesh. Valid components of this type include biwise-OR combinations
 * of the the values in this enum.
 *
 * An absence of any bitwise-OR components in a CC3VertexContent is represented by kCC3VertexContentNone.
 * 
 * All valid meshes must contain a kCC3VertexContentLocation, and a meshes will raise
 * an assertion if an attempt is made to set the vertex content without that component.
 */
typedef enum {
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * an absence of any vertex content.
	 */
	kCC3VertexContentNone				= 0,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex contains location information.
	 *
	 * Vertices must always include location info.
	 */
	kCC3VertexContentLocation			= 1 << 0,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex contains normal information.
	 *
	 * This component is required if and only if the mesh is to interact with light sources.
	 */
	kCC3VertexContentNormal				= 1 << 1,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex contains tangent information.
	 *
	 * This component is required if and only if the mesh is using tangent-space bump-mapping.
	 */
	kCC3VertexContentTangent			= 1 << 2,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex contains bitangent information.
	 *
	 * This component is optionally included when the mesh is using tangent-space bump-mapping.
	 */
	kCC3VertexContentBitangent			= 1 << 3,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex contains color information.
	 *
	 * This component is required if and only if each vertex on the mesh will have its own color.
	 * If this component is not included, all vertices in the mesh will have the color specified
	 * by the coloring properties of the material of the mesh node.
	 */
	kCC3VertexContentColor				= 1 << 4,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex maps to a position on a texture.
	 *
	 * This component is required if and only if the mesh is covered by a texture.
	 */
	kCC3VertexContentTextureCoordinates	= 1 << 5,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates
	 * each vertex contains point size information.
	 *
	 * This component is required if and only if the vertices are part of a point pariticle mesh,
	 * and each particle will have its own size. If this component is not included in a point
	 * particle mesh, all particles will have the same size.
	 */
	kCC3VertexContentPointSize			= 1 << 6,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates each vertex contains
	 * a series of weights to allow the vertex to be manipulated by a series of weighted bones.
	 *
	 * This component is required if and only if the mesh is a vertex skinned mesh.
	 * This component requires that the kCC3VertexContentBoneIndices also be specified.
	 */
	kCC3VertexContentBoneWeights			= 1 << 7,
	
	/**
	 * Bitwise-OR component of CC3VertexContent variables that indicates each vertex contains a
	 * series of bone indices to allow the vertex to be manipulated by a series of weighted bones.
	 *
	 * This component is required if and only if the mesh is a vertex skinned mesh.
	 * This component requires that the kCC3VertexContentBoneWeights also be specified.
	 */
	kCC3VertexContentBoneIndices		= 1 << 8,
} CC3VertexContent;

/** Returns a string description of the specified vertex content components. */
static inline std::string stringFromCC3VertexContent(CC3VertexContent vtxContent);
	
/** Indicates that a face has no neighbour over a particular edge. */
#define kCC3FaceNoNeighbour  ((GLuint)~0)

/**
 * For each edge in a face, contains an index to the adjacent face,
 * or kCC3FaceNoNeighbour if the face has no neighbour over that edge.
 */
typedef struct {
	GLuint edges[3];		/**< Indices to the 3 neighbouring faces, in winding order. */
} CC3FaceNeighbours;

/** Returns a string description of the specified CC3FaceNeighbours struct. */
static inline std::string stringFromCC3FaceNeighbours(CC3FaceNeighbours faceNeighbours) 
{
	return stringWithFormat( (char*)"(%u, %u, %u)",
			faceNeighbours.edges[0], faceNeighbours.edges[1], faceNeighbours.edges[2] );
}

/**
 * Represents a point of intersection on the mesh.
 * 
 * This might represent an intersection point between this mesh and a ray or another mesh.
 */
typedef struct {
	GLuint faceIndex;		/**< The index of the face that was intersected. */
	CC3Face face;			/**< The face that was intersected. */
	CC3Plane facePlane;		/**< The plane equation of the face that was intersected. */
	CC3Vector location;		/**< The location of the intersection, in local cartesian coordinates. */
	GLfloat distance;		/**< The distance from the startLocation of the ray to the intersection location. */
	CC3BarycentricWeights barycentricLocation;	/**< The location of the intersection, in barycentric coordinates. */
	bool wasBackFace : 1;	/**< Indicates whether the intersected originated on the back side of the face. Useful for ray intersections. */
} CC3MeshIntersection;

/** Returns a string description of the specified CC3MeshIntersection struct. */
static inline std::string stringFromCC3MeshIntersection( const CC3MeshIntersection& mi )
{
	/*return stringWithFormat( "(%@ face intersection at location %@ (%@ barycentric) in face %@ at index %i and plane %@ at ray distance %.3f)",
	(mi.wasBackFace ? @"Back" : @"Front"), NSStringFromCC3Vector(mi.location),
	NSStringFromCC3BarycentricWeights(mi.barycentricLocation), NSStringFromCC3Face(mi.face),
	mi.faceIndex, NSStringFromCC3Plane(mi.facePlane), mi.distance );
	*/
	return "";
}

/**
 * Returns a pointer to the CC3MeshIntersection structure contained within the specified array of
 * CC3MeshIntersection structures that has the smallest distance component. The hitCount parameter
 * indicates the size of the meshHits array. Returns NULL if hitCount is zero.
 */
static inline CC3MeshIntersection* CC3NearestMeshIntersection(CC3MeshIntersection* meshHits, unsigned int hitCount) 
{
	CC3MeshIntersection* nearestHit = NULL;
	for (unsigned int hitIdx = 0; hitIdx < hitCount; hitIdx++)
	{
		if (!nearestHit || meshHits[hitIdx].distance < nearestHit->distance) 
			nearestHit = &meshHits[hitIdx];
	}
	return nearestHit;
}


NS_COCOS3D_END

#endif

