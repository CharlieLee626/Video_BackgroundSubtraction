# Video background subtraction
The program can run at the command line as: xxx.exe C:/myDir/foreGroundVideo C:/myDir/backGroundVideo mode
1. foreGroundVideo is the base name for a video, which has a foreground element (actor, object) captured in front of a green screen or without a green screen.
2. backGroundVideo is any normal video.
3. mode is a mode that can take values 1 or 0. 1 indicating that the foreground video has a green screen, and 0 indicating there is no green screen.

## Imput format
Two input videos are in the same rgb format – each video will be a 640x480 video that plays at 24 fps for a total of 20 seconds (480 frames). The frames are named with a basename and indexed by frame number eg basename.0001.rgb, basename.0002.rgb ... basename.0600.rgb.

## Output format
Display a sequence of images at the frame rate OF 24 fps.

## Implementation details for mode 1: with green screen
1. In this mode we have a green screen used in the foreground video.
2. The actual RGB pixel values of the green screen can vary depending on lighting conditions, shadows cast, noise and quantization in the capture process etc. I applied thresholds by analyzing videos to decide how to detect green screen pixels.

## Implementation details for mode 0: without green screen
1. In this mode the foreground video does not have any constant colored green screen, and the foreground element (actor, object) are moving in every frame while the camera is static.
2. "Background subtraction": Comparing corresponding pixels in consecutive frame1 and frame2 (for each x,y), we should be able to assess pixels that have “not changed” and hence can serve as "green screen" pixels.
3. Then proceed to composite the other video’s corresponding frame with this extracted green screen.