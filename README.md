# cad-mapping-master-thesis

# cad-mapping-master-thesis

Abstract—This thesis presents a method for estimating the 3D pose of an object in a picture captured by recent structured-light sensors, such as the PrimeSense sensor commercialized under the name Kinect. In particular the document is focused on the estimation of the pose of a target represented by a CAD model without passing through the transformation of the model to a point cloud dataset, as required by the traditional approach. We study an iterative method which uses the geometrical entities which a CAD model is composed of and which works on the distance point-face rather that on the distance point-point as the traditional ICP does. For this reason we will refer to this method as "Iterative Closest Face" (ICF). The work demonstrates that the algorithm, in contrast with the traditional approach, is able to converge to the solution without any initial knowledge on the pose of the target although

its efficiency could be stepped up by the use of rough- alignment algorithms. In order to achieve the goal of

finding the pose of an object in a scene composed of several objects, we adopt an algorithm to extract the points belonging to different objects and associate them to different clusters, proposed by the Point Cloud Library. For each different cluster, the ICF maximizes an objective function whose value is proportional to the quality of the match. The presented pose-estimation method is intended to create an alternative to the ICP algorithm when the geometry of the CAD-model representing the target is easy enough to render the ICF more efficient than the traditional approach.
