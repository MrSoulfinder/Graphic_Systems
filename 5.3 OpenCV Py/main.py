import cv2
import numpy as np

color_image = cv2.imread('Lily_bmp')
color_image_rgb = cv2.cvtColor(color_image, cv2.COLOR_BGR2RGB)
cv2.imshow('Color RGB', color_image_rgb)
cv2.imshow('Color BGR', color_image)
cv2.waitKey(0)

color_spaces = ('RGB','GRAY','HSV','LAB','XYZ','YUV')
color_images = {color : cv2.cvtColor(color_image, getattr(cv2,'COLOR_BGR2' + color))
    for color in color_spaces}
for color in color_images:
    cv2.imshow(color, color_images[color])
cv2.waitKey(0)