# WA-Challenge-Jeffrey-Liu

![answer](https://user-images.githubusercontent.com/21164705/215352465-c41d2750-cd15-4b80-8de8-2bc543815359.png)

## Methodology

1. Red.png is first changed from RGB to HSV, because HSV is easier to use as the color can be controlled through hue, how strong the color is shown through saturation, and how dark or white it is through value. 
2. Using the inRange() function, the pixels were filtered through HSV values to more easily find the contours that are similar in HSV as the orange cones.
3. The findContours() is then used to find the outline of where orange pixels lie. 
4. sizeFilter() is then used to filter out contours to filter out contours that were too small to be cones and contours that were too large too be cones.
5. roadFilter() is used to filter out the contours on the sides of the picture as the only area of the picture that matters are the ones in the middle.
6. leftContours() and rightContours() is then used to separate the contours on the left line and the right line.
7. Then by using fitLine() and drawLine() the line of best fit that goes throught the left and right points are drawn.
8. Lastly, imwrite is used to save the picture with the line of best fit drawn.

### What did you try?
I initially wanted to use the slope between two points to separate the contours of the left and right line. I thought it would be really slow. As of now thinking about it again, I think I could have made it do less operations by not randomly choosing points, but choosing points that are close to this particular point. However, this still would have been somewhat slow.
In the very beginning, I also tried a little with black and white. However, it was a little hard to filter, so I moved to a different direction.
In the beginning, I also wanted to try RGB. However, after researching a little, I think people prefered to use HSV as RGB is a little hard to control as humans.

### Libraries used
I just used the OpenCV library.
