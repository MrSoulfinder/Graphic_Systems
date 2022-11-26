import cv2
from matplotlib import pyplot as plt

image = cv2.imread('Lily.bmp')
window_name = 'Image'
k_size=(3,3)
img_blur_3 = cv2.blur(image,k_size)
plt.imshow(img_blur_3)
plt.show()

color_spaces = ('RGB','GRAY','HSV','LAB','XYZ','YUV')
color_images = {color : cv2.cvtColor(image, getattr(cv2,'COLOR_BGR2' + color))
    for color in color_spaces}
for color in color_images:
    cv2.imshow(color, color_images[color])
cv2.waitKey(0)