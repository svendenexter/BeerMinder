import cv2
import numpy as np;
from matplotlib import pyplot as plt
#import tensorflow as tf
#from tensorflow import keras


# READ IMAGE
IMG="caps7.jpg"

img = cv2.imread(IMG,1)
img_copy = cv2.imread(IMG,1)

# GRAY IMG
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)








# FINDING CAPS FROM IMAGE
def finding_caps(image):

     detector = cv2.CascadeClassifier("myhaar.xml")
     rects = detector.detectMultiScale(image, scaleFactor=1.1,
	     minNeighbors=5, minSize=(40, 40))
    
    
     return rects
 
    
 
# DRAW A RECTANGLE FOR EACH CAP
def show_founded_caps(caps,image):
    
    for (i, (x, y, w, h)) in enumerate(caps):
        
        R=int((w/2)+((w/2)*0.1))
        r=int(w/2)
        
        cv2.circle(image,(x+r,y+r),R,(174,110,10),2)
        
        cv2.putText(image, "Cap #{}".format(i + 1), (x, y - 10),
    	 	cv2.FONT_HERSHEY_SIMPLEX, 0.30, (240, 175, 10), 1)
        
    	             
    return image



    
 
    
# EXTRACTING CAPS FROM IMAGE
def extract_caps(caps,image):
    
    extracted=[0]

    for (i, (x, y, w, h)) in enumerate(caps):
        r=int(w/2)        
        extracted.append(image[y:(y+2*r), x:(x+2*r)])
    
    extracted=extracted[1:]
        
    return extracted











# APPLYING MASK
def mask_applying(caps,image):
    
    # height,width,x = image.shape
    # mask = np.zeros((height,width), np.uint8)
    
    masked_caps=[0]

    for (i, (x, y, w, h)) in enumerate(caps): 
        
        caps=image[i]
                       
        height,width,x = caps.shape        
        mask = np.zeros((height,width), np.uint8)
        
        r=int((w/2)-(0.1*(w/2)))
        x=int(width/2)
        y=int(height/2)
        
        cv2.circle(mask,(x,y),r,(255,255,255),thickness=-1)
                                  
        masked_caps.append(cv2.bitwise_and(caps, caps, mask=mask))
        
    masked_caps=masked_caps[1:]
    
    return  masked_caps



# # COLOR HISTOGRAM
# def color_histogram(caps,for_loop):
     
    
#     #for (i, (x, y, w, h)) in enumerate(for_loop):
        
#     caps=caps[0]
    
    
#     color = ('b','g','r')
#     for i,col in enumerate(color):
#         histr = cv2.calcHist([caps],[i],None,[256],[0,256])
#         plt.plot(histr,color = col)
#         plt.ylim(0,500)
#         plt.xlim([0,256])
#         #print(i,sum(histr[1:50]))
        
#     plt.show() 
             
 
#     pass











# CALLING FUNCTIONS
    

# finding caps -> returns coordinates of founded caps
founded_caps = finding_caps(gray)

# extracting caps -> returns extracted images
extracted_caps = extract_caps(founded_caps,img_copy)

# draws rectangles foo founded caps -> returns image of caps with rectangles
caps_with_rectangle = show_founded_caps(founded_caps,img)

# applies mask for extracted caps -> returns images of caps with mask
caps_with_mask = mask_applying(founded_caps,extracted_caps)

# color histogram
# color_histogram = color_histogram(caps_with_mask,founded_caps)







cv2.imshow("FOUNDED CAPS",caps_with_rectangle)

cv2.imshow("0",extracted_caps[0])
# cv2.imshow("1",extracted_caps[1])
# cv2.imshow("2",extracted_caps[2])
# cv2.imshow("3",extracted_caps[3])

cv2.imshow("0.",caps_with_mask[0])
# cv2.imshow("1.",caps_with_mask[1])
# cv2.imshow("2.",caps_with_mask[2])
# cv2.imshow("3.",caps_with_mask[3])


cv2.waitKey(0)
cv2.destroyAllWindows()






