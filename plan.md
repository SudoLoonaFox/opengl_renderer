# Goal
To make a simple game engine in c using vulkan

# Skills I will learn
This project will require me to learn how to program code to run on a gpu, will make me learn linear algebra, game mechanics engine mechanics such as delta time, rendering concepts and related matrices, quaternions, forward and inverse kinematics, physics calculations for rigid bodies, data serialization, large project management skills.

# Game stuff I intend to make
This is written before I have properally looked into how vulkan works so it may be a bad approch. I am basing this off when I wrote a renderer in the past
The first thing I will do is render a triagle to the screen. This ensures that I have learned how to open a window and draw to it. After this I will hard code in a triagulated cube and display it as wireframe. I will use the cpu initally to ensure what I will rewrite in the gpu works first. I would rather debug cpu code than gpu stuff as I have not written code for the gpu before. This step is very simple to do as it is just applying the perspective projection matrix to the points, checking if they are in front of the near clipping plane then drawing the triagles as projected.  I will then apply backface culling math. This is easy to do as you can calculate it by using the dot product of the surface normal and the camera to triangle vector and discard values greater than or equal to 0. Doing this in the gpu, displaying them in wireframe and rotating the model will show me if it is working properally as I will see the faces that point towards the camera but the ones that don't won't be drawn. 

After looking at vulkan stuff a bit I realized I don't have to do everything from scratch like I expected. I possibly might split this project into one where I do everything from scratch and use vuklan just for gpu parellization and a more complete engine that will use vulkan as intended
