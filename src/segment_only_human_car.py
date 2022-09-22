import cv2

RGB_input_DIR = '/your/input/RGB/image/folder/directory'
Depth_input_DIR = '/your/input/Depth/image/folder/directory'
RGB_output_DIR = '/your/output/RGB/image/folder/directory'
Depth_output_DIR = '/your/output/Depth/image/folder/directory'

# All_duration = [[each duration of fisrt image],[],...,[each duration of last image]]
# Each_duation = [duration_read_color, duration_read_depth, duation_seg, duration_save_color, duration_save_depth, duration_process]

def save_images_from_path_folder(RGB_input_DIR, Depth_input_DIR, RGB_output_DIR, Depth_output_DIR):
    s = 0 # start image index
    All_duation = [] # save all processing duation
    for filename in os.listdir(RGB_input_DIR): # for loop image
        Each_duration = [] # save each processing duation

        # time start processing
        time_start_process = cv2.getTickCount()

        # time duration read color image
        time_start_read_color = cv2.getTickCount() # start time read color image
        image = cv2.imread(os.path.join(RGB_input_DIR+'/'+os.listdir(RGB_input_DIR)[s]), cv2.IMREAD_UNCHANGED) # read RGB image
        # image = cv2.resize(image,(640,480)) # resize RGB image
        time_stop_read_color = cv2.getTickCount() # stop time read color image
        duration_read_color = (time_stop_read_color - time_start_read_color)/cv2.getTickFrequency() # time duration read color image 
        Each_duration.append(duration_read_color) # save time duration read color image 

        # time duration read depth image
        time_start_read_depth = cv2.getTickCount() # start time read depth image
        depth = cv2.imread(os.path.join(Depth_input_DIR+'/'+os.listdir(Depth_input_DIR)[s]),cv2.IMREAD_UNCHANGED) # read Depth image
        # depth = cv2.resize(image,(640,480)) # resize Depth image
        time_stop_read_depth = cv2.getTickCount() # stop time read depth image
        duration_read_depth = (time_stop_read_depth - time_start_read_depth)/cv2.getTickFrequency() # time duration read depth image 
        Each_duration.append(duration_read_depth) # save time duration read color image 

        time_start_seg = cv2.getTickCount() # start time segmentation
        results = model.detect([image], verbose=1) # detect object
        r = results[0] # save class_ids
        indices_to_remove  = np.where((r['class_ids'] != 1) & (r['class_ids'] != 3)) # save output index that is not human and car
        new_class_ids = np.delete(r['class_ids'], indices_to_remove, axis=0) # remove class_ids which is not human and car
        new_rois = np.delete(r['rois'], indices_to_remove, axis=0) # remove rois which is not human and car
        new_scores = np.delete(r['scores'], indices_to_remove, axis=0) # remove scores which is not human and car
        new_masks = np.delete(r['masks'], indices_to_remove, axis=2) # remove masks which is not human and car
        y_image_axis = new_masks.shape[0] # save image y axis
        x_image_axis = new_masks.shape[1] # save image x axis
        All_instance = new_masks.shape[2] # save all instance segmentation
        image_rgba = cv2.cvtColor(image,cv2.COLOR_RGB2RGBA) # convert RGB to RGBA

        if All_instance > 0: # there is an object detection
            for i in range(y_image_axis): # y-axis
                for j in range(x_image_axis): # x-axis
                    for k in range(All_instance): # each instance
                        if new_masks[i,j,k] == True: #i - vertical , j - horizontal
                            image_rgba[i,j,3] = 0 # at this index will be transparent
                            depth[i,j] = 0 # at this index will be black
            check = 1
        else : 
            print('No require instance') # there is no instance
            check = 0
        
        time_stop_seg = cv2.getTickCount() # stop time segmentation
        duration_seg = (time_stop_seg - time_start_seg)/cv2.getTickFrequency() # time duration segmentation
        Each_duration.append(duration_seg) # save time segmentation 

        # time duration save color image
        time_start_save_color = cv2.getTickCount() # start time save color image
        rgb_out = os.path.join(RGB_output_DIR+'/'+os.listdir(RGB_input_DIR)[s][0:len(os.listdir(RGB_input_DIR)[s])-4])+'.png' # directory to save color image #filename[0:len(filename)-4] to remove .jpg
        # rgb_out = os.path.join(RGB_output_DIR+'/'+str(s))+'.png' #filename[0:len(filename)-4] to remove .jpg
        cv2.imwrite(rgb_out,image_rgba) # output RGB image
        time_stop_save_color = cv2.getTickCount() # stop time save depth image
        duration_save_color = (time_stop_save_color - time_start_save_color)/cv2.getTickFrequency() # time duration save color image 
        Each_duration.append(duration_save_color) # save time save color image 

        # time duration save color image
        time_start_save_depth = cv2.getTickCount() # start time save color image
        depth_out = os.path.join(Depth_output_DIR+'/'+os.listdir(Depth_input_DIR)[s][0:len(os.listdir(Depth_input_DIR)[s])-4])+'.png' # directory to save depth image
        # depth_out = os.path.join(Depth_output_DIR+'/'+str(s))+'.png'        
        cv2.imwrite(depth_out,depth) # output Deph image
        time_stop_save_depth = cv2.getTickCount() # stop time save depth image
        duration_save_depth = (time_stop_save_depth - time_start_save_depth)/cv2.getTickFrequency() # time duration save depth image
        Each_duration.append(duration_save_depth) # save time save depth image 

        s  = s + 1 # next image index

        # time stop processing
        time_stop_process = cv2.getTickCount() 

        # time duration processing
        duration_process = (time_stop_process - time_start_process)/cv2.getTickFrequency() # time duration processing
        Each_duration.append(duration_process) # save time processing 

        # save all duration
        All_duation.append(Each_duration)

    return  All_duation # duration output 
    #return rgb_out, depth_out

All_time_process = save_images_from_path_folder(RGB_input_DIR, Depth_input_DIR, RGB_output_DIR, Depth_output_DIR)