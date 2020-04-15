import time
from absl import app, logging
import cv2
import numpy as np
import tensorflow as tf
from yolov3_tf2.models import (
    YoloV3, YoloV3Tiny
)
from yolov3_tf2.dataset import transform_images, load_tfrecord_dataset
from yolov3_tf2.utils import draw_outputs
from flask import Flask, flash, render_template, url_for,request,redirect, Response, jsonify, send_from_directory, abort,session
import os 
import datetime
import shutil 
import pandas as pd

# customize your API through the following parameters
classes_path = './data/labels/coco.names'
weights_path = './weights/yolov3.tf'
tiny = False                    # set to True if using a Yolov3 Tiny model
size = 416                      # size images are resized to for model
output_path = './static/images/'   # path to output folder where images with detections are saved
num_classes = 9                # number of classes in model

# load in weights and classes
physical_devices = tf.config.experimental.list_physical_devices('GPU')
if len(physical_devices) > 0:
    tf.config.experimental.set_memory_growth(physical_devices[0], True)

if tiny:
    yolo = YoloV3Tiny(classes=num_classes)
else:
    yolo = YoloV3(classes=num_classes)

yolo.load_weights(weights_path).expect_partial()
print('weights loaded')

class_names = [c.strip() for c in open(classes_path).readlines()]
print('classes loaded')

# Initialize Flask application
app = Flask(__name__)

beer_folder = os.path.join('static', 'images')
uploadFolder = os.path.join('detections')

app.config['download_folder'] = beer_folder
app.config['upload_folder'] = uploadFolder

app.config["ALLOWED_IMAGE_EXTENSIONS"] = ["PNG","JPG","JPEG","GIF","png","jpg","jpeg","gif"]
app.config["MAX_IMAGE_FILESIZE"] = 0.5*1024*1024
app.secret_key = "secretKey"

def allowed_image(filename):
    if not "." in filename:
        return False
    ext= filename.rsplit(".",1)[1]
    if ext in app.config["ALLOWED_IMAGE_EXTENSIONS"]:
        return True
    else:
        return False
input_filename = "upload_picture.jpeg"
output_filename = "result_picture.jpeg"

inputfilename = "upload_picture.jpeg"
outputfilename = "result_picture.jpeg"
lastInputName = "upload_picture.jpeg"
lastOutputName = "result_picture.jpeg"
allowed_extension = False
message = "please upload your file below"
caps_found = 0
@app.route('/', methods=['GET', 'POST'])
def upload_file():
    global inputfilename
    global imagePath
    global outputfilename
    global lastInputName
    global lastOutputName
    global allowed_extension
    global message
    global caps_found
    global CapsAndAmount
    global total_price

    caps_found_default = 0
    CapsAndAmount_default = []
    total_price_default = 0

    if request.method == 'POST':
        if request.files:
            allowed_extension, inputfilename, outputfilename, CapsAndAmount, caps_found, total_price= get_image(request.files["image"],lastInputName,lastOutputName) 
            print(allowed_extension)
            if (allowed_extension == False):
                message = "please upload a file with the folowing extension : " + str(app.config["ALLOWED_IMAGE_EXTENSIONS"])
                return redirect(request.url) 
            else:
                message = "succesfully uploaded your file"
                if lastInputName != "upload_picture.jpeg":
                    try:
                        os.remove("static\\images\\" + lastInputName)
                    except:
                        print( "this file is not in folder ", lastInputName)
                if lastOutputName != "result_picture.jpeg":
                    try:
                        os.remove("static\\images\\"+ lastOutputName)
                    except:
                        print( "this file is not in folder ", lastOutputName)
                        
                lastInputName = inputfilename
                lastOutputName = outputfilename
                for brand in range(len(class_names)):      
                    print(class_names[brand]," ",class_calculation.count(class_names[brand]))
                input_picture = os.path.join(app.config['download_folder'], inputfilename)
                output_picture = os.path.join(app.config['download_folder'], outputfilename)
                return render_template('upload.html', valid_file = message,total_price = total_price, input_image = input_picture, output_image = output_picture,CapsAndAmount =CapsAndAmount, caps_found = caps_found)  

    input_picture = os.path.join(app.config['download_folder'], input_filename)
    output_picture = os.path.join(app.config['download_folder'], output_filename)
    return render_template('upload.html', valid_file = message,total_price = total_price_default, input_image = input_picture, output_image = output_picture,CapsAndAmount =CapsAndAmount_default, caps_found = caps_found_default)

# API that returns JSON with classes found in images
@app.route('/detections', methods=['POST'])
def get_detections():
    raw_images = []
    images = request.files.getlist("images")
    image_names = []
    for image in images:
        image_name = image.filename
        image_names.append(image_name)
        image.save(os.path.join(os.getcwd(), image_name))
        img_raw = tf.image.decode_image(
            open(image_name, 'rb').read(), channels=3)
        raw_images.append(img_raw)
        
    num = 0
    
    # create list for final response
    response = []

    for j in range(len(raw_images)):
        # create list of responses for current image
        responses = []
        raw_img = raw_images[j]
        num+=1
        img = tf.expand_dims(raw_img, 0)
        img = transform_images(img, size)

        t1 = time.time()
        boxes, scores, classes, nums = yolo(img)
        t2 = time.time()
        print('time: {}'.format(t2 - t1))

        print('detections:')
        for i in range(nums[0]):
            print('\t{}, {}, {}'.format(class_names[int(classes[0][i])],
                                            np.array(scores[0][i]),
                                            np.array(boxes[0][i])))
            responses.append({
                "class": class_names[int(classes[0][i])],
                "confidence": float("{0:.2f}".format(np.array(scores[0][i])*100))
            })
        response.append({
            "image": image_names[j],
            "detections": responses
        })
        img = cv2.cvtColor(raw_img.numpy(), cv2.COLOR_RGB2BGR)
        img = draw_outputs(img, (boxes, scores, classes, nums), class_names)
        cv2.imwrite(output_path + 'detection' + str(num) + '.jpg', img)
        print('output saved to: {}'.format(output_path + 'detection8' + str(num) + '.jpg'))

    #remove temporary images
    for name in image_names:
        os.remove(name)
    try:
        return jsonify({"response":response}), 200
    except FileNotFoundError:
        abort(404)

# API that returns image with detections on it
class_calculation = []
golden_array = []
caps_found = 0
CapsAndAmount = []
total_price = 0
def get_image(image,last_input_name,last_output_name):
    global class_calculation
    global golden_array
    global caps_found
    global class_names 
    global CapsAndAmount
    global total_price
    original_filename = image.filename
    goodExtension = allowed_image(original_filename)
    image_name = last_input_name
    filename = last_output_name
    if goodExtension == True:
        image_name = "inputpicture_" + str( datetime.datetime.now().date()) + '_' + str(datetime.datetime.now().time()).replace(':', '.')+".jpg"
        image.save(os.path.join(os.getcwd(), image_name))
        shutil.copy2(image_name,"static\\images\\"+image_name)
        img_raw = tf.image.decode_image(
            open(image_name, 'rb').read(), channels=3)
        img = tf.expand_dims(img_raw, 0)
        img = transform_images(img, size)

        t1 = time.time()
        boxes, scores, classes, nums = yolo(img)
        t2 = time.time()
        print('time: {}'.format(t2 - t1))

        print("Number of founded caps: ",int(nums[0]))
        caps_found = int(nums[0])
        class_calculation=[]
        golden_array=[]
    
        for i in range(nums[0]):
            class_calculation.append(class_names[int(classes[0][i])])

        for brand in range(len(class_names)):      
            golden_array.append(class_calculation.count(class_names[brand]))
        
        print(golden_array)

        print('detections:')
        for i in range(nums[0]):
            print('\t{}, {}, {}'.format(class_names[int(classes[0][i])],
                                            np.array(scores[0][i]),
                                            np.array(boxes[0][i])))
        img = cv2.cvtColor(img_raw.numpy(), cv2.COLOR_RGB2BGR)
        img = draw_outputs(img, (boxes, scores, classes, nums), class_names)
        filename = "output" + str(datetime.datetime.now().date()) + '_' + str(datetime.datetime.now().time()).replace(':', '.')+".jpg"
        cv2.imwrite(output_path + filename, img)
        print('output saved to: {}'.format(output_path + 'detection.jpg'))
        
        # prepare image for response
        _, img_encoded = cv2.imencode('.png', img)
        response = img_encoded.tostring()
        
        #remove temporary image
        os.remove(image_name)

        prices=[0.63,0.71,1.48,0.75,0.88,0.90,0.70,0.67,1.02]
        temp_price=0
        total_price=0

        for sum_price in range(len(golden_array)):
            temp_price=prices[sum_price]*golden_array[sum_price]
            total_price=total_price+temp_price

        print(total_price)
        total_price = str(round(total_price, 2))

        CapsAndAmount=[]
        i= 0
        while i < len(golden_array):
            CapsAndAmount.append({'Number':i+1,'Brand':class_names[i],'Amount':golden_array[i],'Price':prices[i],'TotalPrice': total_price })
            i=i+1
        print(CapsAndAmount)
        try:
            return (goodExtension, image_name, filename,CapsAndAmount,caps_found,total_price)
        except FileNotFoundError:
            abort(404)
    else:
        return (goodExtension, image_name, filename,CapsAndAmount,caps_found,total_price)
if __name__ == '__main__':
    app.run(debug=True, host = '0.0.0.0', port=5000)