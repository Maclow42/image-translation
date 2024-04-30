#generate images of digits from 0 to 9 of size 24x24
#save them in a folder named "GenratedDigits"
import numpy as np
import cv2
import os
from numpy import random

PATH = "./test"

s1 = 48
s2 = 24

def main(num_nbr):
    #create a folder to store the generated digits
    if not os.path.exists(PATH):
        os.makedirs(PATH)

    while(num_nbr > 0):

        #loop over all the digits
        for i in range(48,58):
            img = np.zeros((s1,s1), np.uint8)
            #draw the digit on the image with random font
            if i != 0:
            	font = random.choice([cv2.FONT_HERSHEY_SIMPLEX, cv2.FONT_HERSHEY_DUPLEX, cv2.FONT_HERSHEY_COMPLEX, cv2.FONT_HERSHEY_TRIPLEX, cv2.FONT_HERSHEY_SCRIPT_SIMPLEX, cv2.FONT_HERSHEY_SCRIPT_COMPLEX])
            	font_scale = random.uniform(0.7, 1.7)
            	thickness = random.randint(1, 4)
            	cv2.putText(img, chr(i), (14,38), font, font_scale, (255,255,255), thickness)


            #add some noise
            for t in range(random.randint(0, 50)):
                img[random.randint(0,s1-1)][random.randint(0,s1-1)] = random.randint(0, 150)

            #add some lines
            if random.random() > 1 :
            	for t in range(1):
            		cv2.line(img, (random.randint(0,s1-1), random.randint(0,s1-1)), (random.randint(0,s1-1), random.randint(0,s1-1)), (255,255,255), 1)            
            
            #resize the image
            img = cv2.resize(img, (s2,s2))
            #img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)[1] / 255.0


            #save the image
            name = PATH + str(i) + "_" + str(num_nbr) + ".png"
            cv2.imwrite(name, img)
            #print("wrote :", name)
        num_nbr -= 1

main(8)
